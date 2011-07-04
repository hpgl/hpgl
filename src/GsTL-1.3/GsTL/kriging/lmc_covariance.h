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

 

#ifndef __GSTL_LMC_COVARIANCE_H__
#define __GSTL_LMC_COVARIANCE_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/kriging/covariance_set.h>
#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/utils/gstl_error_messages.h>


template < class Covariance_ >
class LMC_covariance : public Covariance_set_impl<Covariance_> {

 public:
  typedef typename Covariance_::result_type result_type;
  typedef typename Covariance_::first_argument_type first_argument_type;
  typedef typename Covariance_::second_argument_type second_argument_type;
  typedef first_argument_type Location;

  virtual ~LMC_covariance() {}
  
  template <class CovarianceMatrix>
  LMC_covariance(
		 const CovarianceMatrix& cov_mat,
		 unsigned int size
		 )
    : Covariance_set_impl<Covariance_>(),
      matrix_size_(size), cov_mat_(size,size) {

    for( unsigned int i=1; i <= matrix_size_; i++ )
      for( unsigned int j=1; j <= matrix_size_; j++ )
	cov_mat_(i,j) = cov_mat(i,j);
  };


  LMC_covariance( const LMC_covariance<Covariance_ >& rhs ) :
    Covariance_set_impl<Covariance_>(),
    matrix_size_( rhs.matrix_size_ ),
    cov_mat_( rhs.cov_mat_ ) {
  }


  virtual Covariance_set_impl<Covariance_>* clone() const {
    return new LMC_covariance<Covariance_>( *this );
  }

  /** Computes Cij( u1, u2 ). The first covariance is C11 (ie i and j
   * indexes both start at 1, not 0). 
   */
  virtual result_type operator()(unsigned int i, unsigned int j,
				 const Location& u1, const Location& u2) const {
 
      gstl_assert(i <= matrix_size_ && j <= matrix_size_);
      return (cov_mat_(i, j))(u1, u2);
    }


 private:
  
  unsigned int matrix_size_;
  TNT::Matrix<Covariance_> cov_mat_;
  
}; // end of class LMC_covariance



#endif
