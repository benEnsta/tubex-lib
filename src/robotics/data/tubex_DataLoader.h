/* ============================================================================
 *  tubex-lib - DataLoader class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_DATALOADER_H__
#define __TUBEX_DATALOADER_H__

#include <string>
#include <fstream>
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"

namespace tubex
{
  class DataLoader
  {
    public:

      DataLoader(const std::string& file_path);
      ~DataLoader();

      virtual void load_data(TubeVector *&x,
                             TrajectoryVector *&truth,
                             const ibex::Interval& domain = ibex::Interval::ALL_REALS) = 0;

    protected:

      std::ifstream *m_datafile;
  };
}

#endif