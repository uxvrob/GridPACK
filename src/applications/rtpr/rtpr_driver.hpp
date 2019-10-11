/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
// -------------------------------------------------------------
/**
 * @file   rtpr_driver.hpp
 * @author Bruce Palmer
 * @date   October 9, 2019
 *
 * @brief
 *
 *
 */
// -------------------------------------------------------------

#ifndef _rtpr_driver_h_
#define _rtpr_driver_h_

#include "gridpack/include/gridpack.hpp"
#include "gridpack/applications/modules/powerflow/pf_app_module.hpp"

namespace gridpack {
namespace rtpr {

enum ContingencyType{Generator, Branch};

/* Defininition of contingency data structure (from powerflow module)
struct Contingency
{
  int p_type;
  std::string p_name;
  // Line contingencies
  std::vector<int> p_from;
  std::vector<int> p_to;
  std::vector<std::string> p_ckt;
  // Status of line before contingency
  std::vector<bool> p_saveLineStatus;
  // Generator contingencies
  std::vector<int> p_busid;
  std::vector<std::string> p_genid;
  // Status of generator before contingency
  std::vector<bool> p_saveGenStatus;
};
*/

// Calling program for real-time path rating application
class RTPRDriver
{
  public:
    /**
     * Basic constructor
     */
    RTPRDriver(void);

    /**
     * Basic destructor
     */
    ~RTPRDriver(void);

    /**
     * Get list of contingencies from external file
     * @param cursor pointer to contingencies in input deck
     * @return vector of contingencies
     */
    std::vector<gridpack::powerflow::Contingency> getContingencies(
        gridpack::utility::Configuration::ChildCursors contingencies);

    /**
     * Execute application
     * @param argc number of arguments
     * @param argv list of character strings
     */
    void execute(int argc, char** argv);

    private:
};

} // rtpr
} // gridpack
#endif