/** 
 *  Arithmetic operations on trajectories
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \bugfixes   Bertrand Neveu
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_traj_arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  const Trajectory operator+(const Trajectory& x)
  {
    return x;
  }

  const Trajectory operator-(const Trajectory& x)
  {
    assert(x.function() == NULL
      && "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = -it->second;

    return Trajectory(map_y);
  }
    
  #define macro_scal_unary(f) \
    \
    const Trajectory f(const Trajectory& x) \
    { \
      assert(x.function() == NULL \
        && "not supported yet for trajectories defined by a Function"); \
      \
      map<double,double> map_y = x.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(it->second); \
      \
      return Trajectory(map_y); \
    } \
    \

  macro_scal_unary(cos);
  macro_scal_unary(sin);
  macro_scal_unary(abs);
    
  const Trajectory sqr(const Trajectory& x)
  {
    assert(x.function() == NULL
      && "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::pow(it->second,2);

    return Trajectory(map_y);
  }

  macro_scal_unary(sqrt);
  macro_scal_unary(exp);
  macro_scal_unary(log);
  macro_scal_unary(tan);
  macro_scal_unary(acos);
  macro_scal_unary(asin);
  macro_scal_unary(atan);
  macro_scal_unary(cosh);
  macro_scal_unary(sinh);
  macro_scal_unary(tanh);
  macro_scal_unary(acosh);
  macro_scal_unary(asinh);
  macro_scal_unary(atanh);

  #define macro_scal_unary_param(f, p) \
    \
    const Trajectory f(const Trajectory& x, p param) \
    { \
      assert(x.function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      \
      map<double,double> map_y = x.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = std::f(it->second, param); \
      \
      return Trajectory(map_y); \
    } \
    \
  
  macro_scal_unary_param(pow, int);
  macro_scal_unary_param(pow, double);

  const Trajectory root(const Trajectory& x, int p)
  {
    assert(x.function() == NULL &&
      "not supported yet for trajectories defined by a Function");

    map<double,double> map_y = x.sampled_map();
    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::pow(it->second, 1. / p);

    return Trajectory(map_y);
  }

  #define macro_scal_binary_arith(f) \
    \
    const Trajectory operator f(const Trajectory& x1, const Trajectory& x2) \
    { \
      assert(x1.domain() == x2.domain()); \
      assert(x1.function() == NULL && x2.function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      /* todo: x1 or x2 could be defined by a Function, in fact */ \
      \
      Trajectory x1_sampled(x1), x2_sampled(x2); \
      x1_sampled.sample(x2); \
      x2_sampled.sample(x1); \
      map<double,double> map_x1 = x1_sampled.sampled_map(), map_x2 = x2_sampled.sampled_map(); \
      \
      map<double,double>::iterator it_x1 = map_x1.begin(); \
      map<double,double>::iterator it_x2 = map_x2.begin(); \
      \
      while(it_x1 != map_x1.end()) \
      { \
        it_x1->second = it_x1->second f it_x2->second; \
        it_x1++; it_x2++; \
      } \
      \
      return Trajectory(map_x1); \
    } \
    \
    const Trajectory operator f(const Trajectory& x1, double x2) \
    { \
      assert(x1.function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      \
      Trajectory y(x1); \
      map<double,double> map_y = y.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = it->second f x2; \
      \
      return Trajectory(map_y); \
    } \
    \
    const Trajectory operator f(double x1, const Trajectory& x2) \
    { \
      assert(x2.function() == NULL && \
        "not supported yet for trajectories defined by a Function"); \
      \
      Trajectory y(x2); \
      map<double,double> map_y = y.sampled_map(); \
      \
      for(map<double,double>::iterator it = map_y.begin() ; \
        it != map_y.end() ; it++) \
        it->second = x1 f it->second; \
      \
      return Trajectory(map_y); \
    } \
    \

  macro_scal_binary_arith(+);
  macro_scal_binary_arith(-);
  macro_scal_binary_arith(*);
  macro_scal_binary_arith(/);

  const Trajectory atan2(const Trajectory& x1, const Trajectory& x2)
  {
    assert(x1.domain() == x2.domain());
    assert(x1.function() == NULL && x2.function() == NULL &&
      "not supported yet for trajectories defined by a Function");
    /* todo: x1 or x2 could be defined by a Function, in fact */

    Trajectory x1_sampled(x1), x2_sampled(x2);
    x1_sampled.sample(x2);
    x2_sampled.sample(x1);
    map<double,double> map_x1 = x1.sampled_map(), map_x2 = x2.sampled_map();

    map<double,double>::iterator it_x1 = map_x1.begin();
    map<double,double>::iterator it_x2 = map_x2.begin();

    while(it_x1 != map_x1.end())
    {
      it_x1->second = std::atan2(it_x1->second, it_x2->second);
      it_x1++; it_x2++;
    }

    return Trajectory(map_x1);
  }

  const Trajectory atan2(const Trajectory& x1, double x2)
  {
    assert(x1.function() == NULL &&
      "not supported yet for trajectories defined by a Function");

    Trajectory y(x1);
    map<double,double> map_y = y.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::atan2(it->second, x2);

    return Trajectory(map_y);
  }

  const Trajectory atan2(double x1, const Trajectory& x2)
  {
    assert(x2.function() == NULL &&
      "not supported yet for trajectories defined by a Function");

    Trajectory y(x2);
    map<double,double> map_y = y.sampled_map();

    for(map<double,double>::iterator it = map_y.begin() ;
      it != map_y.end() ; it++)
      it->second = std::atan2(x1, it->second);

    return Trajectory(map_y);
  }
}