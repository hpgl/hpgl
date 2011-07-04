/* GsTL: the Geostatistics Template Library
 * 
 * Author: Nicolas Remy
 * Copyright (c) 2000 The Board of Trustees of the Leland Stanford Junior University
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *   1.Redistributions of source code must retain the above copyright notice, this 
 *     list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright notice, this 
 *     list of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution. 
 *   3.The name of the author may not be used to endorse or promote products derived 
 *     from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GSTL_SERVO_SYSTEM_SAMPLER_H__
#define __GSTL_SERVO_SYSTEM_SAMPLER_H__

#include <GsTL/univariate_stats/build_cdf.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

class RGrid;
class GsTLGridProperty;

/** A servo system sampler draws realizations from a cdf so that the cdf of all the 
 * values drawn by the servo system is close to a target cdf.
 * A parameter in [0,1] is used to set how much modification the servo system is 
 * allowed to do on the input cdf. If it is equal to 0, the input cdf is not 
 * modified at all and the servo system sampler behaves as a classical Monte 
 * Carlo sampler. If the constraint parameter is set to 1, the cdf of all the 
 * values drawn by the servo system will exactly identify the taget cdf (for
 * a large number of drawings).
 * 
 * Servo system samplers can be conveniently created using helper function
 * Servo_system.   
 */

template < class RandNumberGenerator >
class Servo_system_sampler {

 public:
  
  /** Constructs a servo system
   * @param target is the target cdf 
   * @param constraint allows to make the servo system more or less lenient.
   * It varies between 0 and 1. If constraint=1, the servo system ensures that
   * the target cdf is exactly reproduced.
   * @param  [first, last) is a range of geovalues.
   */
  template<class ForwardIterator, class CategNonParamCdf>
  Servo_system_sampler( const CategNonParamCdf& target,
			double constraint,
			ForwardIterator first, ForwardIterator last,
			const RandNumberGenerator& rand );

  /** Constructs a servo system
   * @param target is the target cdf 
   * @param constraint allows to make the servo system more or less lenient.
   * It varies between 0 and 1. If constraint=1, the servo system ensures that
   * the target cdf is exactly reproduced.
   * @param initial_histogram is a vector containing the initial number of geovalue
   * in each class.  
   */
  template< class CategNonParamCdf >
  Servo_system_sampler( const CategNonParamCdf& target,
			double constraint,
			const std::vector<double>& initial_histogram,
			const RandNumberGenerator& rand );

  /** Copy constructor
   */
  Servo_system_sampler( const Servo_system_sampler< RandNumberGenerator >& rhs );

  
	
  /** Draws a realization from ccdf and assigns it to gval.
   * @return 0 if a value was successfully assigned to gval, 1 if
   * gval has not been changed 
   */
  template<class GeoValue, class CategNonParamCdf>
  int operator()(GeoValue& gval, const CategNonParamCdf& ccdf); 
  
    /*
     * function to unsign simulated value at some locations
     */
    void removeSimulatedNode( GsTLGridProperty* prop, vector<int>& grid_path );
    void removeSimulatedNode( RGrid* grid, GsTLGridProperty* prop, vector<int>& grid_path );

    // for testing purpose only
    void print();

 private:

  int nb_of_categories_;
  std::vector<double> target_pdf_;
  std::vector<double> current_histogram_;
  double nb_of_data_;
  
  double mu_;

  RandNumberGenerator gen_;

}; // end of class Servo_system





template 
<
  class CategNonParamCdf,
  class RandNumberGenerator,
  class ForwardIterator
>
Servo_system_sampler<RandNumberGenerator>
Servo_system(
	     const CategNonParamCdf& target,
	     double constraint,
	     ForwardIterator first, ForwardIterator last,
	     const RandNumberGenerator& rand
	     ) {
  return Servo_system_sampler<
           RandNumberGenerator
         >(
	   target, constraint,
	   first, last,
	   rand
	   );
}



#include "servo_system.cc"

#endif
