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

 

#ifndef __GTL_CDF_TRANSFORM__
#define __GTL_CDF_TRANSFORM__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/cdf/non_param_cdf.h>
#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/utils/gstl_error_messages.h>


#include <vector>
#include <iterator>
#include <algorithm>

/** Transforms range of values [first, last) by histogram identification.
 * Each element in range [first, last) is replaced by its transformed value.
 * If the initial values are still needed after the transformation, a copy of 
 * [first, last) should be made before being passed to function "cdf_transform".
 * Two versions of the algorithm exist. Version 2 builds the cdf of the data.
 * Both Version 1 and 2 are specialized for the case where the target cdf is 
 * gaussian, so that the min value in [first,last) is not transformed to 
 * minus infinity and the max to plus infinity. 
 *
 * \param [first,last) is the range of values to be transformed. The value in range
 * [first,last) are over-written by their transformed values.
 * \param from is the (original) cdf of the data to be transformed 
 * \param to is the cdf of the transformed values
 */
template<class TargetCdfType, class DataCdfType, class ForwardIterator>
void
cdf_transform(ForwardIterator first, ForwardIterator last,
	      const DataCdfType& from, const TargetCdfType& to) {
  
  for(ForwardIterator it=first; it!=last; it++) {
      double P = from.prob(it->get_value());
      it->set_value(to.inverse(P));
    }
}


/*
template<class LTI, class MI, class UTI, class ForwardIterator>
void
cdf_transform(ForwardIterator first, ForwardIterator last,
	      const Non_param_cdf<LTI,MI,UTI>& from, const Gaussian_cdf& to) {

  // Make a copy of the "from" cdf that we can modify
  Non_param_cdf<LTI,MI,UTI> from_copy(from);
  
  typedef typename Non_param_cdf<LTI,MI,UTI>::p_iterator p_iterator;
  p_iterator it = from_copy.p_begin();
  *it = *it / 2.0;
  ++it;
  for( ; it != from_copy.p_end(); ++it ) {
    *it = ( *it + *(it-1) ) / 2; 
  }

  // Now do the transform, using the modified cdf
  for(ForwardIterator it=first; it!=last; it++) {
    double P = from_copy.prob(*it);
    *it = to.inverse(P);
  }
  
}
*/

//________________________
// Version 2

template<class TargetCdfType, class ForwardIterator>
void
cdf_transform(ForwardIterator first, ForwardIterator last,
	      const TargetCdfType& to) {
  
  Non_param_cdf<> from;

  build_cdf_copy( first, last, from );

  cdf_transform(first, last, from, to);
}


/*
template<class ForwardIterator>
void
cdf_transform(ForwardIterator first, ForwardIterator last,
	      const Gaussian_cdf& to) {

  Non_param_cdf<> from;

  // Make a copy of range [first, last)
  typedef typename std::iterator_traits<ForwardIterator>::value_type value_type;  
  int size = static_cast<int>( std::distance(first, last) );

  std::vector<value_type> range_copy(size);
  std::copy(first, last, range_copy.begin() );
  
  // Lower the min value in range [first, last) and increase the max value.
  // This is because P(Z<min)=0, so the transformed value of min will 
  // be minus infinity (we are mapping to a gaussian distribution)
  // SImilarily, max will be transformed to plus infinity. 
  typedef std::vector<value_type>::iterator iterator;
  iterator min_elem = std::min_element(range_copy.begin(), range_copy.end());
  iterator max_elem = std::max_element(range_copy.begin(), range_copy.end());
  
  const float percentage = 0.04;
  value_type tolerance = (*max_elem - *min_elem)*percentage;
  *min_elem -= tolerance;
  *max_elem += tolerance;

  build_cdf(range_copy.begin(), range_copy.end(), from );

  cdf_transform(first, last, from, to);
}
*/



//________________________
// Version 3

/** This version of the algorithm applies a weighted transform: corresponding to
 * each value, there is a weight in [0,1] that says to what extent the original
 * value can be changed: if the weight is 0, the value is left unchanged, if it
 * is 1, the value is transformed normally.
 * @param Range [first_weight, last_weight) contains the weights associated to
 * each value in range [first,last). The 2 ranges must be of the same size.
 */
template < 
  class TargetCdfType,
  class DataCdfType, 
  class ForwardIterator,
  class ForwardIterator2
  >
void
cdf_transform( ForwardIterator first, ForwardIterator last,
	       ForwardIterator first_weight, ForwardIterator last_weight,
	       const DataCdfType& from, const TargetCdfType& to
	       ) {
  
  for( ; first != last; ++first, ++first_weight ) {
    gstl_assert( first_weight != last_weight );
    typename ForwardIterator::value_type trans = *first;
    cdf_transform( &trans, &(trans+1), from, to ); 
    *first = (*first) + (*first_weight) * ( trans - (*first) ) ;
  }
}



#endif
