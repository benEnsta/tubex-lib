/** 
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_AbstractDomain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Interval AbstractDomain::m_dump_i = Interval();
  IntervalVector AbstractDomain::m_dump_iv = IntervalVector(1);
  Slice AbstractDomain::m_dump_s = Slice(Interval(0.,1.));
  Tube AbstractDomain::m_dump_t = Tube(Interval(0.,1.));
  TubeVector AbstractDomain::m_dump_tv = TubeVector(Interval(0.,1.));

  AbstractDomain::AbstractDomain(const AbstractDomain& ad)
    : m_type(ad.m_type), m_i(ad.m_i), m_iv(ad.m_iv), m_s(ad.m_s), m_t(ad.m_t), m_tv(ad.m_tv), m_v_ctc(ad.m_v_ctc)
  {

  }

  AbstractDomain::AbstractDomain(ibex::Interval& i)
    : m_type(DomainType::INTERVAL), m_i(i), m_iv(m_dump_iv), m_s(m_dump_s), m_t(m_dump_t), m_tv(m_dump_tv)
  {

  }

  AbstractDomain::AbstractDomain(ibex::IntervalVector& iv)
    : m_type(DomainType::INTERVAL_VECTOR), m_i(m_dump_i), m_iv(iv), m_s(m_dump_s), m_t(m_dump_t), m_tv(m_dump_tv)
  {

  }

  AbstractDomain::AbstractDomain(tubex::Slice& s)
    : m_type(DomainType::SLICE), m_i(m_dump_i), m_iv(m_dump_iv), m_s(s), m_t(m_dump_t), m_tv(m_dump_tv)
  {
    m_s &= Interval(-99999.,99999.); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::Tube& t)
    : m_type(DomainType::TUBE), m_i(m_dump_i), m_iv(m_dump_iv), m_s(m_dump_s), m_t(t), m_tv(m_dump_tv)
  {
    m_t &= Interval(-99999.,99999.); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::TubeVector& tv)
    : m_type(DomainType::TUBE_VECTOR), m_i(m_dump_i), m_iv(m_dump_iv), m_s(m_dump_s), m_t(m_dump_t), m_tv(tv)
  {
    m_tv &= IntervalVector(m_tv.size(), Interval(-99999.,99999.)); // todo: remove this
  }

  AbstractDomain::~AbstractDomain()
  {
    
  }

  DomainType AbstractDomain::type() const
  {
    return m_type;
  }

  double AbstractDomain::volume() const
  {
    switch(m_type)
    {
      case DomainType::INTERVAL:
        if(m_i.is_empty())
          return 0.;
        
        else if(m_i.is_unbounded())
          return 999999.; // todo: manager the unbounded case for fixed point detection
        
        else
          return m_i.diam();

      case DomainType::INTERVAL_VECTOR:
        return m_iv.volume();

      case DomainType::SLICE:
      {
        double vol = m_s.volume();

        // todo: clean the following:

        if(m_s.input_gate().is_empty())
          vol += 0.;
        else if(m_s.input_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += m_s.input_gate().diam();

        if(m_s.output_gate().is_empty())
          vol += 0.;
        else if(m_s.output_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += m_s.output_gate().diam();

        return vol;
      }

      case DomainType::TUBE:
        return m_t.volume();

      case DomainType::TUBE_VECTOR:
        return m_tv.volume();

      default:
        assert(false && "unhandled case");
    }
  }

  bool AbstractDomain::is_empty() const
  {
    switch(m_type)
    {
      case DomainType::INTERVAL:
        return m_i.is_empty();

      case DomainType::INTERVAL_VECTOR:
        return m_iv.is_empty();

      case DomainType::SLICE:
        return m_s.is_empty();

      case DomainType::TUBE:
        return m_t.is_empty();

      case DomainType::TUBE_VECTOR:
        return m_tv.is_empty();

      default:
        assert(false && "unhandled case");
    }
  }
  
  bool AbstractDomain::operator==(const AbstractDomain& x) const
  {
    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case DomainType::INTERVAL:
        return &m_i == &x.m_i;

      case DomainType::INTERVAL_VECTOR:
        return &m_iv == &x.m_iv;

      case DomainType::SLICE:
        return &m_s == &x.m_s;

      case DomainType::TUBE:
        return &m_t == &x.m_t;

      case DomainType::TUBE_VECTOR:
        return &m_tv == &x.m_tv;

      default:
        assert(false && "unhandled case");
        return false;
    }
  }

  bool AbstractDomain::is_component_of(const AbstractDomain* x) const
  {
    if(x->type() != DomainType::INTERVAL_VECTOR || type() != DomainType::INTERVAL)
      return false;
    if(x->type() != DomainType::TUBE_VECTOR || type() != DomainType::TUBE)
      return false;

    for(int i = 0 ; i < x->m_iv.size() ; i++)
      if(&x->m_iv[i] == &m_i)
        return true;

    return false;
  }

  ostream& operator<<(ostream& str, const AbstractDomain& x)
  {
    switch(x.m_type)
    {
      case DomainType::INTERVAL:
        str << "Interval: " << x.m_i << flush;
        break;

      case DomainType::INTERVAL_VECTOR:
        str << "IntervalVector: " << x.m_iv << flush;
        break;

      case DomainType::SLICE:
        str << "Slice: " << x.m_s << flush;
        break;

      case DomainType::TUBE:
        str << "Tube: " << x.m_t << flush;
        break;

      case DomainType::TUBE_VECTOR:
        str << "TubeVector: " << x.m_tv << flush;
        break;

      default:
        assert(false && "unhandled case");
        break;
    }

    return str;
  }
}