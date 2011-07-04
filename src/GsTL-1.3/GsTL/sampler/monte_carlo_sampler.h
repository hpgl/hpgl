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

 

#ifndef __GSTL_MONTE_CARLO_SAMPLER_H__
#define __GSTL_MONTE_CARLO_SAMPLER_H__


#include <GsTL/math/random_number_generators.h>

#include <cmath>
#include <stdlib.h>

/** This model of Sampler simulates a node by Monte Carlo Simulation:
 * a random realization is drawn from the input cdf and assigned to the geovalue.
 * Objects of this type can be convieniently created using helper function
 * Monte_carlo_sampler.
 */

template 
<
  class RandNumberGenerator = Rand48_generator
>
class Monte_carlo_sampler_t{
  public:

  Monte_carlo_sampler_t()
    : rand_generator_(Rand48_generator(21111975)) {}

  
  Monte_carlo_sampler_t(const RandNumberGenerator& rand_generator)
  : rand_generator_(rand_generator) {};
  
  template<class Cdf, class GeoValue>
  inline int operator()(GeoValue& gval, const Cdf& f) {
    //gval.set_property_value( f.inverse( rand_generator_() ) );
    double p = rand_generator_();
    gval.set_value( f.inverse( p ) );
    return 0;
  }

  
  private:
  
  RandNumberGenerator rand_generator_; 
};



template <class RandNumberGenerator>
inline Monte_carlo_sampler_t<RandNumberGenerator>
Monte_carlo_sampler(const RandNumberGenerator& gen) {
  return Monte_carlo_sampler_t<RandNumberGenerator>(gen);
}

#endif
