# -*- mode: cmake -*-
# -------------------------------------------------------------
# file: CMakeLists.txt
# -------------------------------------------------------------
# -------------------------------------------------------------
# Created May  3, 2013 by William A. Perkins
# Last Change: 2013-08-01 07:53:37 d3g096
# -------------------------------------------------------------

cmake_minimum_required (VERSION 2.6.4)
project (GridPACK)

# This is a C++ project exclusively
enable_language(CXX)

# where to look for special .cmake files
list (APPEND CMAKE_MODULE_PATH "${GridPACK_SOURCE_DIR}/cmake-modules")
include(GridPACK)

# turn testing on
enable_testing()

# -------------------------------------------------------------
# MPI compiler
# -------------------------------------------------------------
message(STATUS "Checking MPI ...")
find_package (MPI REQUIRED)
include_directories(AFTER ${MPI_INCLUDE_PATH})

# Perkins found out that this was exactly the wrong thing to do:
# 
# set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
# set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
# set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

# -------------------------------------------------------------
# Boost
#
# Find the Boost library. It needs to use the same compiler as
# MPI. How can I check that?
# -------------------------------------------------------------
message(STATUS "Checking Boost ...")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.52 COMPONENTS mpi serialization REQUIRED)
include_directories(AFTER ${Boost_INCLUDE_DIRS})

# -------------------------------------------------------------
# PETSc
#
# We need a very recent version of PETSc. It needs to parallel and
# compiled with C++ complex support.
# -------------------------------------------------------------
message(STATUS "Checking PETSc ...")
find_package (PETSc COMPONENTS CXX REQUIRED)

if (NOT EXISTS "${PETSC_DIR}/${PETSC_ARCH}/conf/PETScConfig.cmake")
  message(FATAL_ERROR "PETSc found, but CMake configuration for PETSc installation not found?")
endif()

include("${PETSC_DIR}/${PETSC_ARCH}/conf/PETScConfig.cmake")

# checks 

if (NOT PETSC_HAVE_MPI)
  message(FATAL_ERROR "PETSc installation is not parallel (--with-mpi=1)")
endif()

if (NOT PETSC_USE_COMPLEX) 
  message (FATAL_ERROR "PETSc installation does not use complex (--with-scalar-type=complex)")
endif()

# This is probably redundant
if (NOT PETSC_CLANGUAGE_Cxx)
  message (FATAL_ERROR "PETSc installation does not use C++ (--with-clanguage=c++)")
endif()

# -------------------------------------------------------------
# ParMETIS
# 
# Only versions > 4.0 have been used
# -------------------------------------------------------------
message(STATUS "Checking ParMETIS ...")
# Eventually this needs to be REQUIRED
find_package(ParMETIS REQUIRED)
# -------------------------------------------------------------
# GA
# 
# Only versions > 5.2 have been used
# -------------------------------------------------------------
message(STATUS "Checking GA ...")
find_package(GA REQUIRED)

# -------------------------------------------------------------
# Doxygen
# -------------------------------------------------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
endif(DOXYGEN_FOUND)

# -------------------------------------------------------------
# Options for all Programs/Libraries
# -------------------------------------------------------------

# remove annoying warnings produced by the GNU compiler about
# deprecated variables/functions; some are caused by GridPACK code
# (these, particularly string constants should be fixed) and some are
# caused by Boost::MPI

if (CMAKE_COMPILER_IS_GNUCXX) 
    add_definitions(
        -Wno-deprecated 
        -Wno-deprecated-declarations
        -Wno-write-strings
    )
endif()


include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------------------------------------------
# Subdirectories
# -------------------------------------------------------------
add_subdirectory(utilities)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(component)
add_subdirectory(partition)
add_subdirectory(network)
add_subdirectory(factory)
add_subdirectory(mapper)
add_subdirectory(applications/powerflow)
add_subdirectory(parser)
add_subdirectory(serialIO)
add_subdirectory(configuration)