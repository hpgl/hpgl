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

 

#ifndef __GTL_KT_CONSTRAINTS__
#define __GTL_KT_CONSTRAINTS__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/utils/gstl_error_messages.h>

#include <iterator>

template<class ForwardIterator>
class KT_constraints{

 public:

  /** constructor.
   * KT assumes the mean is expressed as: m= sum(Ai * Fi(u)), i=1,...,K. 
   * The constructor thus requires a range on the container [F1 ... FK].
   * The Fi are functors ( require an operator() )
   * @param [begin,end) is a range on container [F1 ... FK].
   */ 
  KT_constraints(
		 ForwardIterator begin,
		 ForwardIterator end
		 )
    : mean_first_(begin), mean_last_(end){
    mean_size_ = static_cast<int>( std::distance(begin, end) );
  }
  
  
  /** This operator, common to all kriging engines, sets the 
   * kriging system up. It takes the kriging matrix, the second
   * member, sets their dimensions, and fill in the lagrangian
   * part of the system.
   * Kriging with Trend (KT) is usually not used for cokriging.
   * This engine hence only works for one variable kriging.
   * However, as all the kriging engines share the same interface,
   * a range of neighborhood is passed.
   *
   * @param A is the kriging matrix. 
   * @param b is a Vector, the RHS of the kriging system. It will be resized
   * by KT constraints.
   * @param center is the location being krigged
   * @param [first_neigh,last_neigh) is a range of Neighborhoods.
   * @return the total number of conditioning data
   */ 
  template <
            class InputIterator,
            class Location,
            class SymmetricMatrix,
            class Vector
           >
  unsigned int operator() (
			   SymmetricMatrix& A,
			   Vector& b,
			   const Location& center,
			   InputIterator first_neigh, InputIterator last_neigh
			   ) const ;
  

  template <
            class Neighborhood_,
            class Location_,
            class SymmetricMatrix,
            class Vector_
           >
  unsigned int operator() (
			   SymmetricMatrix& A,
			   Vector_& b,
			   const Location_& center,
			   const Neighborhood_& neighbors
			   ) const {
    return this->operator()( A,b,center, &neighbors, &neighbors+1 );
  }



  /** Compute the contribution to the kriging variance of the Lagrange
   * parameters.
   * \c center is the location for which the kriging system is solved,
   * [begin_weights,end_weights) is a range of lagrange weights. 
  */ 
  template< class Location_, class InputIterator > 
  double kriging_variance_contrib( const Location_& center, 
                                   InputIterator begin_weights,  
                                   InputIterator end_weights ) const {
    double contrib = 0.0;
    for( ForwardIterator mean_it = mean_first_; mean_it != mean_last_ ; 
         ++mean_it, ++begin_weights ) {
      gstl_assert( begin_weights != end_weights );
      contrib += (*mean_it)( center ) * (*begin_weights);
    }
    return contrib;
  }  


 private:
  ForwardIterator mean_first_;
  ForwardIterator mean_last_;
  int mean_size_;

}; // end of KT_engine class



#if defined(__GNUC__) || defined(WIN32)
#include "KT_constraints.cc"
#elif defined(__xlC__)
#pragma implementation("KT_constraints.cc")
#endif

#endif
