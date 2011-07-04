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

 

#ifndef __GSTL_MM1_COVARIANCE_H__
#define __GSTL_MM1_COVARIANCE_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/kriging/covariance_set.h>
#include <GsTL/geometry/geometry_algorithms.h>
#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/utils/gstl_error_messages.h>


template < class Covariance_ >
class MM1_covariance : public Covariance_set_impl<Covariance_> {

 public:

  typedef typename Covariance_::result_type result_type;
  typedef typename Covariance_::first_argument_type first_argument_type;
  typedef typename Covariance_::second_argument_type second_argument_type;
  typedef first_argument_type Location;

  template < class Matrix_ >
  MM1_covariance(
		 const Covariance_& C11,
		 const Matrix_& A,
		 unsigned int size
		 ) 
    : Covariance_set_impl<Covariance_>(),
      matrix_size_(size),
      cov_mat_(size,size),
      C11_(C11) {

    for( unsigned int i=1; i <= matrix_size_; i++ )
      for( unsigned int j=1; j <= matrix_size_; j++ )
	cov_mat_(i,j) = A(i,j);
  }

  MM1_covariance( const MM1_covariance<Covariance_ >& rhs ) :
    Covariance_set_impl<Covariance_>(),
    matrix_size_( rhs.matrix_size_ ),
    cov_mat_( rhs.cov_mat_ ),
    C11_( rhs.C11_ ) {
  }

  virtual ~MM1_covariance() {}

  virtual Covariance_set_impl<Covariance_>* clone() const {
    return new MM1_covariance<Covariance_>( *this );
  }

  virtual result_type operator()(unsigned int i, unsigned int j,
				 const Location& u1, const Location& u2) const {

      if(i==1 && j==1)
	return C11_(u1, u2);

      gstl_assert(i <= matrix_size_ && j <= matrix_size_);
      if(i==1 || j==1)
	return cov_mat_(i,j)/cov_mat_(1,1)*C11_(u1, u2);
      
      // Cij, i!=1 and j!=1 is to be computed. In the case of MM1, 
      // this covariance is only needed for distance 0 (MM1 is used in
      // collocated cokriging: all the secondary variables are collocated)
      if ( square_euclidean_distance(u1, u2) != 0 )
	{
	  gstl_warning( "trying to compute C" << i << j 
			<< " at distance different from 0" << std::endl
			<< "MM1 covariance set is only suitable for collocated "
			<< "cokriging, which does not " << std::endl
			<< "involve the covariance between secondary variables"
			<< " at distances different from 0" );
	  gstl_assert( square_euclidean_distance(u1,u2) != 0);
	}

      // if the function has not exited yet, it means the covariance to 
      // compute is Cij(0), i!=1 and j!=1. This covariance is contained in
      // the matrix of covariances.
      return cov_mat_(i,j);
    }


 private:
  
  unsigned int matrix_size_;
  TNT::Matrix<double> cov_mat_;
  Covariance_ C11_;

}; // end of class MM1_covariance


#endif
