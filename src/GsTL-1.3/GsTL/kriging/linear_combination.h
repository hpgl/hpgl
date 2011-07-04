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

 

#ifndef __GSTL_LINEAR_COMBINATION_H__
#define __GSTL_LINEAR_COMBINATION_H__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/utils/gstl_error_messages.h>

/** This function computes the linear combination between the
 * scalars in range [begin_weights; end_weights) and the elements
 * of the neighborhood that neighbors points to. 
 */

template
<
  class InputIterator,
  class Neighborhood
>
double
linear_combination(InputIterator begin_weights, InputIterator end_weights,
		   const Neighborhood& neighbors)
{
  //double result=0.0;
	typedef typename Neighborhood::value_type t;
	typename t::property_type result(0);
  
  typename Neighborhood::const_iterator it= neighbors.begin();
  typename Neighborhood::const_iterator end = neighbors.end();
  for( ; it != end ; ++it ) {
    gstl_assert(begin_weights != end_weights);
    result += it->get_value() * (*begin_weights);
    begin_weights++;
  }

  return result;
}


#endif
