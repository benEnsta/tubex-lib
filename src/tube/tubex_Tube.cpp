/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Tube::Tube(const Interval& domain, const Interval& codomain) : TubeTree(domain, codomain)
  {

  }
  
  Tube::Tube(const Interval& domain, double timestep, const Interval& codomain) : TubeTree(domain, codomain)
  {
    if(timestep <= 0.)
      throw Exception("TubeSlice::chainSlices", "gate already existing");

    else if(timestep < domain.diam())
    {
      double lb, ub = domain.lb();
      vector<double> v_bounds; // a vector of slices is created only once
      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = lb + timestep;
        if(ub < domain.ub()) v_bounds.push_back(ub);
      } while(ub < domain.ub());

      sample(v_bounds);
    }
  }
}