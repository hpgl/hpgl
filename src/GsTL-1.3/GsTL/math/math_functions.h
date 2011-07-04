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

#ifndef __GSTL_MATH_FUNCTIONS_H__
#define __GSTL_MATH_FUNCTIONS_H__

#include <cmath>

#undef INFINITY
#undef PI
#undef EPSILON

namespace GsTL
{

  // Definition of some useful constants
  
  static const double EPSILON = 0.00000001;
  static const double INFINITY = 1.e16;
  static const double PI = 3.1415926535897932;


  // Redefinition of some classical math functions

  template<class T>
  inline int floor(T t) {
    return static_cast<int>(t);
  }

  template<class T>
  inline int ceil(T t) {
    return static_cast<int>(::ceil(static_cast<float>(t)));
  }
 
  template< class T >
  inline int round( T t ) {
    return (t < 0) ? static_cast<int>( t - 0.5 ) : static_cast<int>( t + 0.5 ) ;
  }

  template< class T >
  inline T abs( T t ) {
    return ( t >= 0 ) ? t : -t;
  }

  inline bool equals( double d1, double d2, double precision=EPSILON ) {
    if( d1 == float(0) ) {
      if( d2 == float(0) ) return true;
      return false;
    }
    return ( abs( d1-d2 )/abs(d1) < precision );
  }
  
  inline bool equals( float d1, float d2, float precision=0.00001 ) {
    if( d1 == double(0) ) {
      if( d2 == double(0) ) return true;
      return false;
    }
    return ( abs( d1-d2 )/abs(d1) < precision );
  }
} // end of namespace GsTL



#endif
