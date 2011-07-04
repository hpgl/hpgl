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

#ifndef __GSTL_RANDOM_NUMBER_GENERATORS_H__
#define __GSTL_RANDOM_NUMBER_GENERATORS_H__

#include <boost/random/mersenne_twister.hpp>

#include <cmath>
#include <stdlib.h>


struct Rand48_generator {
  typedef boost::mt11213b::result_type IntType;
  typedef double return_type;

  Rand48_generator() {}
  Rand48_generator( long int seed ) : gen( static_cast<IntType>(seed) ) {}
  inline void seed( long int seed ) { gen.seed( static_cast<IntType>(seed) ); }
  inline double operator() () { return double( gen() ) / double( gen.max BOOST_PREVENT_MACRO_SUBSTITUTION () ); }

private:
	boost::mt11213b gen;
	
};



/*
struct Rand48_generator{
 
  typedef double return_type;
  Rand48_generator() {}
  Rand48_generator(long int seed) {srand48(seed); }
  inline void seed( long int seed ) {  srand48(seed); }
  inline double operator()() { return drand48() ; };

};
*/


//=====================================
/** This class is an adaptor that turns a model of GSTL Random Number Generator
 * into a model of STL Random Number Generator. 
 * Objects of this class are conveniently created using helper function 
 * STL_generator_adaptor.
 */

template <class GsTLGenerator>
class STL_generator_adaptor_t{
 public:
  typedef long int return_type; 
  typedef long int argument_type;

  STL_generator_adaptor_t(GsTLGenerator& gen) 
    : gen_(gen) {}
 
  inline return_type operator()(argument_type N) {
    return static_cast<return_type>(gen_()*N);
  }


 private:
  GsTLGenerator gen_;
};



// helper function
template <class GsTLGenerator>
inline STL_generator_adaptor_t<GsTLGenerator>
STL_generator_adaptor(GsTLGenerator& gen) {

  return STL_generator_adaptor_t<GsTLGenerator>(gen);
}


#endif
