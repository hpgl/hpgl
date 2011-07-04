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



#ifndef __GTL_GAUSSIAN_CDF_KESTIMATOR__
#define __GTL_GAUSSIAN_CDF_KESTIMATOR__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/kriging.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>

#include <vector>


template <
          class Covariance,
          class Neighborhood,
          class KrigingConstraints, 
          class MatrixLibrary=GSTL_TNT_lib
         >
class Gaussian_cdf_Kestimator{
  private:
  typedef Kriging_combiner< std::vector<double>::const_iterator,
                            Neighborhood >                  KrigingCombiner;

  public:

  Gaussian_cdf_Kestimator(
			  const Covariance& cov,
			  const KrigingConstraints& Kconstraints,
			  const KrigingCombiner& combiner
			  )
  : cov_(cov), Kconstr_(Kconstraints), combiner_(combiner), weights_(30) {
  }

  Gaussian_cdf_Kestimator(const Gaussian_cdf_Kestimator& rhs)
  : cov_(rhs.cov_), Kconstr_(rhs.Kconstr_), combiner_(rhs.combiner_), weights_(30) {
  }
			 
  Gaussian_cdf_Kestimator& operator=(const Gaussian_cdf_Kestimator& rhs) {
    cov_=rhs.cov_;
    Kconstr_=rhs.Kconstr_;
    combiner_ = rhs.combiner_;
    weights_.reserve(rhs.weights_.size());

    return *this;
  }

  template <
            class Geovalue_,
            class GaussianCdf
           >
  inline int operator()(
			const Geovalue_& u,
			const Neighborhood& neighbors,
			GaussianCdf& ccdf
			) const ;


  private:
 
  Covariance cov_ ;
  KrigingConstraints Kconstr_ ;
  KrigingCombiner combiner_;
  mutable std::vector<double> weights_;
  
}; // end of class Gaussian_cdf_Kestimator



//====================================================
/** A Gaussian_cdf_Kestimator_rec is essentially similar to the 
 * Gaussian_cdf_Kestimator. The only difference is that it records the
 * kriging variance (hence the "rec" suffix).
 */
template <
          class Covariance,
          class KrigingConstraints, 
          class Combiner,
          class ColocatedNeighborhood,
          class MatrixLibrary=GSTL_TNT_lib
         >
class Gaussian_cdf_Kestimator_rec{
  public:

  Gaussian_cdf_Kestimator_rec(
			  const Covariance& cov,
			  const KrigingConstraints& Kconstraints,
			  const Combiner& combiner,
			  ColocatedNeighborhood* krig_variance_output = 0
			  )
  : cov_(cov), Kconstr_(Kconstraints), combiner_(combiner), weights_(30),
  krig_variance_output_( krig_variance_output ) {
  }

  Gaussian_cdf_Kestimator_rec(const Gaussian_cdf_Kestimator_rec& rhs)
  : cov_(rhs.cov_), Kconstr_(rhs.Kconstr_), combiner_(rhs.combiner_), weights_(30),
  krig_variance_output_( rhs.krig_variance_output_ ) {
  }
			 
  Gaussian_cdf_Kestimator_rec& operator=(const Gaussian_cdf_Kestimator_rec& rhs) {
    cov_=rhs.cov_;
    Kconstr_=rhs.Kconstr_;
    combiner_ = rhs.combiner_;
    weights_.reserve(rhs.weights_.size());
    krig_variance_output_ = rhs.krig_variance_output_ ;

    return *this;
  }

  template <
            class Geovalue_,
            class Neighborhood,
            class GaussianCdf
           >
  inline int operator()(
			const Geovalue_& u,
			const Neighborhood& neighbors,
			GaussianCdf& ccdf
			) const ;


  private:
 
  Covariance cov_ ;
  KrigingConstraints Kconstr_ ;
  Combiner combiner_;
  mutable std::vector<double> weights_;
  
  mutable ColocatedNeighborhood* krig_variance_output_;

}; // end of class Gaussian_cdf_Kestimator_rec



//=======================================
// Definition of inline functions

template <
          class Covariance,
          class Neighborhood,
          class KrigingConstraints,
          class MatrixLibrary
         >
template <
          class Geovalue_,
          class GaussianCdf
         >
inline int 
Gaussian_cdf_Kestimator<Covariance,
			                  Neighborhood,
                        KrigingConstraints,
                        MatrixLibrary>
::operator()(
	     const Geovalue_& u,
	     const Neighborhood& neighbors,
	     GaussianCdf& ccdf
	     ) const {

  double variance;
  int status = kriging_weights<MatrixLibrary>(weights_, variance,
					      u.location(), neighbors,
					      cov_, Kconstr_);

  if(status != 0) {
    gstl_warning( "unable to solve kriging system" );
    return status;
  }
    

  double mean = combiner_(weights_.begin(), weights_.end(),
			  neighbors);  
  ccdf.mean() = mean;
  ccdf.variance() = variance;

  return status;
}











template <
          class Covariance,
          class KrigingConstraints,
          class Combiner,
          class ColocatedNeighborhood,
          class MatrixLibrary
         >
template <
          class Geovalue_,
          class Neighborhood,
          class GaussianCdf
         >
inline int 
Gaussian_cdf_Kestimator_rec<Covariance,
			    KrigingConstraints,
			    Combiner,
			    ColocatedNeighborhood,
			    MatrixLibrary>
::operator()(
	     const Geovalue_& u,
	     const Neighborhood& neighbors,
	     GaussianCdf& ccdf
	     ) const {

  double variance;
  int status = kriging_weights<MatrixLibrary>(weights_, variance,
					      u.location(), neighbors,
					      cov_, Kconstr_);

  if(status != 0) {
    gstl_warning( "unable to solve kriging system" );
    krig_variance_output_->find_neighbors( neighbors.center() );
    krig_variance_output_->begin()->set_property_value( 1.0 );
    return status;
  }
    

  double mean = combiner_(weights_.begin(), weights_.end(),
			  neighbors);  
  ccdf.mean() = mean;
  ccdf.variance() = variance;

  // record the kriging variance 
  krig_variance_output_->find_neighbors( neighbors.center() );
  krig_variance_output_->begin()->set_property_value( variance );

  return status;
}



//==============================
// Helper Function
//==============================

template <
          class Covariance,
          class Neighborhood,
          class KrigingConstraints,
          class MatrixLibrary
         >
Gaussian_cdf_Kestimator<Covariance,Neighborhood, KrigingConstraints, MatrixLibrary>
gaussian_cdf_krig_estimator( Covariance* cov, 
			     const KrigingConstraints& Kconstraints,
			     const Kriging_combiner<std::vector<double>::iterator,
			                            Neighborhood>& combiner ) {

  typedef Gaussian_cdf_Kestimator<Covariance,
                                  Neighborhood,
                                  KrigingConstraints,
                                  MatrixLibrary>        Estimator;

  return Estimator( cov, Kconstraints, combiner );
}



#endif
