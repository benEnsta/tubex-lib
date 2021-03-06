/** 
 *  CtcDeriv class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcDeriv.h"
#include "tubex_ConvexPolygon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv() : Ctc()
  {

  }

  void CtcDeriv::contract(vector<AbstractDomain*>& v_domains)
  {
    assert(v_domains.size() == 2);

    // Slice case:
    if(v_domains[0]->type() == DomainType::SLICE && v_domains[1]->type() == DomainType::SLICE)
      contract(v_domains[0]->m_s, v_domains[1]->m_s);

    // Tube scalar case:
    else if(v_domains[0]->type() == DomainType::TUBE && v_domains[1]->type() == DomainType::TUBE)
      contract(v_domains[0]->m_t, v_domains[1]->m_t);

    // Tube vector case:
    else if(v_domains[0]->type() == DomainType::TUBE_VECTOR && v_domains[1]->type() == DomainType::TUBE_VECTOR)
      contract(v_domains[0]->m_tv, v_domains[1]->m_tv);

    else
      assert(false && "unhandled case");
  }

  void CtcDeriv::contract(Tube& x, const Tube& v, TimePropag t_propa)
  {
    assert(x.domain() == v.domain());
    assert(Tube::same_slicing(x, v));
    
    if(t_propa & TimePropag::FORWARD)
    {
      Slice *s_x = x.first_slice();
      const Slice *s_v = v.first_slice();

      while(s_x != NULL)
      {
        assert(s_v != NULL);
        contract(*s_x, *s_v, t_propa);
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }
    }
    
    if(t_propa & TimePropag::BACKWARD)
    {
      Slice *s_x = x.last_slice();
      const Slice *s_v = v.last_slice();

      while(s_x != NULL)
      {
        assert(s_v != NULL);
        contract(*s_x, *s_v, t_propa);
        s_x = s_x->prev_slice();
        s_v = s_v->prev_slice();
      }
    }
  }

  void CtcDeriv::contract(TubeVector& x, const TubeVector& v, TimePropag t_propa)
  {
    assert(x.size() == v.size());
    assert(x.domain() == v.domain());
    assert(TubeVector::same_slicing(x, v));

    for(int i = 0 ; i < x.size() ; i++)
      contract(x[i], v[i], t_propa);
  }

  void CtcDeriv::contract(Slice& x, const Slice& v, TimePropag t_propa)
  {
    assert(x.domain() == v.domain());
    double volume = x.volume() + v.volume();

    if(!x.domain().intersects(m_restricted_domain))
    {
      // todo: Thin contraction with respect to tube's slicing:
      // the contraction should not be optimal on purpose if the
      // restricted domain does not cover the slice's domain
      return;
    }

    Interval envelope = x.codomain(), ingate = x.input_gate(), outgate = x.output_gate();

    if(m_fast_mode) // Faster contraction without polygons
    {
      if(t_propa & TimePropag::FORWARD)
      {
        x.set_envelope(envelope & (ingate + Interval(0.,x.domain().diam()) * v.codomain()));
        x.set_output_gate(outgate & (ingate + x.domain().diam() * v.codomain()));
      }

      if(t_propa & TimePropag::BACKWARD)
      {
        x.set_envelope(envelope & (outgate - Interval(0.,x.domain().diam()) * v.codomain()));
        x.set_input_gate(ingate & (outgate - x.domain().diam() * v.codomain()));
      }
    }

    else // Optimal contraction
    {
      if(outgate == Interval::ALL_REALS) // Direct evaluation, polygons not needed
      {
        envelope &= ingate + Interval(0.,x.domain().diam()) * v.codomain();
        outgate &= ingate + x.domain().diam() * v.codomain();
      }

      else if(ingate == Interval::ALL_REALS) // Direct evaluation, polygons not needed
      {
        envelope &= outgate - Interval(0.,x.domain().diam()) * v.codomain();
        ingate &= outgate - x.domain().diam() * v.codomain();
      }

      else // Using polygons to compute the envelope
      {
        // todo: remove this: (or use Polygons with truncature)
        envelope &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY);

        x.set_envelope(envelope);

        // Gates contraction
        outgate &= ingate + x.domain().diam() * v.codomain();
        ingate &= outgate - x.domain().diam() * v.codomain();

        // Gates needed for polygon computation
        x.set_input_gate(ingate);
        x.set_output_gate(outgate);

        // Optimal envelope
        envelope &= x.polygon(v).box()[1];

        // todo: remove this: (or use Polygons with truncature)
        if(envelope.ub() == BOUNDED_INFINITY) envelope = Interval(envelope.lb(),POS_INFINITY);
        if(envelope.lb() == -BOUNDED_INFINITY) envelope = Interval(NEG_INFINITY,envelope.ub());
        if(ingate.ub() == BOUNDED_INFINITY) ingate = Interval(ingate.lb(),POS_INFINITY);
        if(ingate.lb() == -BOUNDED_INFINITY) ingate = Interval(NEG_INFINITY,ingate.ub());
        if(outgate.ub() == BOUNDED_INFINITY) outgate = Interval(outgate.lb(),POS_INFINITY);
        if(outgate.lb() == -BOUNDED_INFINITY) outgate = Interval(NEG_INFINITY,outgate.ub());
      }

      x.set_envelope(envelope);
      x.set_input_gate(ingate);
      x.set_output_gate(outgate);
    }

    assert(volume >= x.volume() + v.volume() && "contraction rule not respected");
  }

  void CtcDeriv::contract_gates(Slice& x, const Slice& v)
  {
    assert(x.domain() == v.domain());
    
    Interval in_gate = x.input_gate(), out_gate = x.output_gate();

    Interval in_gate_proj = in_gate + x.domain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    x.set_output_gate(out_gate);

    Interval out_gate_proj = out_gate - x.domain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    x.set_input_gate(in_gate);
  }
}