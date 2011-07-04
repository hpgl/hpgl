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

 

#ifndef __GSTL_GAUSSIAN_CDF_COKESTIMATOR_H__
#define __GSTL_GAUSSIAN_CDF_COKESTIMATOR_H__


#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/kriging.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>



template <
          class CovarianceSet,
          class KrigingConstraints, 
          class Combiner,
          class MatrixLibrary=GSTL_TNT_lib 
         >
class Gaussian_cdf_coKestimator{
  private:
    typedef CovarianceSet C;
    typedef KrigingConstraints K;
    typedef Combiner Co;
    typedef MatrixLibrary M;
    typedef Gaussian_cdf_coKestimator<C,K,Co,M> Self;

  public:
  
  /** Constructor.
   * To avoid resizing the kriging weights vector each time operator() is called,
   * the vector is stored inside the class and can contain 50 (why not) weights
   * by default. 
   */
  Gaussian_cdf_coKestimator(
			    const CovarianceSet& cov_set,
			    const KrigingConstraints& Kconstraints,
			    const Combiner& combiner
			    ) 
  : cov_set_(cov_set), Kconstr_(Kconstraints), combiner_(combiner), weights_(50) {
    
  }

  Gaussian_cdf_coKestimator( const Self& rhs ) 
  : cov_set_(rhs.cov_set_), Kconstr_(rhs.Kconstr_), 
    combiner_(rhs.combiner_), weights_(50) {
  }

  Gaussian_cdf_coKestimator& operator=(const Self& rhs) {
    if( rhs != *this ) {
      cov_set_ = rhs.cov_set_;
      Kconstr_ = rhs.Kconstr_; 
      combiner_ = rhs.combiner_;
      weights_.reserve(50);
    }
    return *this;
  }

  template <
            class Geovalue_,
            class InputIterator,
            class GaussianCdf
           >
  inline int operator()(
			const Geovalue_& u,
			InputIterator first_neigh, InputIterator last_neigh,
			GaussianCdf& ccdf
			) const ;


 private:

  CovarianceSet cov_set_ ;
  KrigingConstraints Kconstr_ ;
  Combiner combiner_;
  mutable std::vector<double> weights_;
 
}; // end of class gaussian_cdf_Kestimator



template <
          class CovarianceSet,
          class KrigingConstraints,
          class Combiner, 
          class MatrixLibrary
         >
template <
          class Geovalue_,
          class InputIterator,
          class GaussianCdf
         >
int 
Gaussian_cdf_coKestimator<
                          CovarianceSet,
                          KrigingConstraints,
                          Combiner,
                          MatrixLibrary
                         >
::operator()(
	     const Geovalue_& u,
	     InputIterator first_neigh, InputIterator last_neigh,
	     GaussianCdf& ccdf
	     ) const  {

  double kriging_variance = 0.0 ;
  int status = 
    cokriging_weights<MatrixLibrary>(
				  weights_, kriging_variance,
				  u.location(), 
				  first_neigh, last_neigh,
				  cov_set_, Kconstr_
				  );

  if(status != 0) {
    gstl_warning( "unable to solve kriging system" );
    return status;
  }

  double mean = combiner_( weights_.begin(), weights_.end(),
	                  		   first_neigh, last_neigh );
  
  ccdf.mean() = mean;
  ccdf.variance() = kriging_variance;

  return status;
}


#endif
