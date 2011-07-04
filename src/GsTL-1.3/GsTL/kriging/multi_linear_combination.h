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

 

#ifndef __GTL_MULTI_LINEAR_COMBINATION__
#define __GTL_MULTI_LINEAR_COMBINATION__
#ifdef __GNUC__
#pragma interface
#endif


/** This function computes the linear combination between the
 * scalars in range [begin_weights; end_weights) and the elements
 * of the neighborhood that neighbors points to.
 * @param [first_weight, last_weight) is a range of weights (double)
 * @param [first_neigh, last_neigh) is a range of Neighborhoods
 */

template <
          class InputIterator,
          class InputIterator2
         >
double
multi_linear_combination(
			 InputIterator first_weight,
			 InputIterator2 first_neigh, InputIterator2 last_neigh
			 ) {
  
  typedef typename std::iterator_traits<InputIterator2>::value_type Neighborhood;
  typedef typename Neighborhood::const_iterator const_iterator;


  double result=0.0;
  
  for( ; first_neigh != last_neigh; ++first_neigh) {
    const_iterator it = first_neigh->begin();
    const_iterator end = first_neigh->end();
    for( ; it != end ; it++) {
      result += it->property_value() * (*first_weight);
      ++first_weight;
    }
  }

  return result;
}


#endif
