/** 
 *  Fnc class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_FNC_H__
#define __TUBEX_FNC_H__

#include "ibex_IntervalVector.h"

namespace tubex
{
  class Tube;
  class TubeVector;
  
  class Fnc
  {
    public:

      Fnc(int n, int m, bool is_intertemporal);
      ~Fnc();
      const Fnc& operator=(const Fnc& f);

      int nb_vars() const;
      int image_dim() const;
      bool is_intertemporal() const;

      virtual const Tube eval(const TubeVector& x) const;
      virtual const ibex::Interval eval(const ibex::IntervalVector& x) const = 0;
      virtual const ibex::Interval eval(int slice_id, const TubeVector& x) const = 0;
      virtual const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const = 0;
      
      virtual const TubeVector eval_vector(const TubeVector& x) const;
      virtual const ibex::IntervalVector eval_vector(const ibex::IntervalVector& x) const = 0;
      virtual const ibex::IntervalVector eval_vector(int slice_id, const TubeVector& x) const = 0;
      virtual const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const = 0;

    protected:
      
      Fnc();

      int m_nb_vars = -1, m_img_dim = -1;
      bool m_is_intertemporal = false;
  };
}

#endif