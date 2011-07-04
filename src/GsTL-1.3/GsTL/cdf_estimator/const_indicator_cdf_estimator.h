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

 

#ifndef __GSTL_INDICATOR_CDF_ESTIMATOR_H__
#define __GSTL_INDICATOR_CDF_ESTIMATOR_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/kriging.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/univariate_stats/build_cdf.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <iterator>
#include <vector>




/** This model of Multiple Variables Cdf Estimator estimates a non-parametric
 * cdf using indicator kriging. Contrary to Indicator_cdf_estimator, a 
 * Const_indicator_cdf_estimator requires that the conditioning data are already
 * encoded in indicator values: the conditioning data are not coded on the fly as
 * Indicator_cdf_estimator does (hence the "Const" prefix). A 
 * Const_indicator_cdf_estimator requires as many variables as the number of 
 * thresholds defining the non-parametric cdf to be estimated. Each variable is the
 * indicator code of the conditioning data for a given threshold.
 * The indicator code of the conditioning values can be any number in [0,1] (see 
 * Goovaerts 1997, Geostatistics for Natural Resources Evaluation, pp. 285-292, for 
 * more information on indicator coding of information). 
 * Since the conditioning data are not coded into indicator values on the fly, 
 * a Const_indicator_cdf_estimator can not be used in sequential simulation. 
 * It's main use is in indicator kriging.
 *
 * A helper function "const_indicator_cdf_estimator" is provided to ease the 
 * instantiation of Const_indicator_cdf_estimator.
 */

template
<
  class InputIterator,
  class KrigingConstraint,
  class MatrixLibrary=GSTL_TNT_lib
>
class Const_indicator_cdf_estimator{
  
  public:
  
  Const_indicator_cdf_estimator(
				InputIterator first_covar, InputIterator last_covar,
				KrigingConstraint& Kconstraints 
				) 
  : covariances_( std::distance(first_covar, last_covar) ),
    Kconstraints_(Kconstraints),
    weights_(50) {
    
      // make sure at least one covariance function is supplied
      gstl_assert(first_covar!=last_covar);
      // copy the covariances into internal storage.
      std::copy( first_covar, last_covar, covariances_.begin() );
    }
		
  
  Const_indicator_cdf_estimator(const Const_indicator_cdf_estimator& rhs) 
  : covariances_(rhs.covariances_),
    Kconstraints_(rhs.Kconstraints_),
    weights_(rhs.weights_.size()) {
  }

  Const_indicator_cdf_estimator& operator=(const Const_indicator_cdf_estimator& rhs) {
    covariances_  = rhs.covariances_;
    Kconstraints_ = rhs.Kconstraints_;
    weights_.reserve(rhs.weights_.size());
  }
  

  /** Estimates the cdf.
   * The kriging weights are computed for each neighborhood. If it can be ensured
   * that all the neighborhoods have the exact same geometry, the kriging weights
   * need only be computed once, and this operator is not efficient. 
   * However, a Const_indicator_cdf_estimator is of interest only when complex
   * indicator coding is required, such as coding inequality constraints. When
   * coding inequality constraints, some indicator values can not be informed,
   * hence preventing from ensuring that all neighborhoods have the same geometry.
   * 
   * @param [first_neigh,last_neigh) is a range of Neighborhoods.
   */
  template
  <
    class Geovalue_,
    class InputIterator2,
    class NonParametricCdf
  >
  inline int operator()(
			const Geovalue_& u,
			InputIterator2 first_neigh, InputIterator2 last_neigh,
			NonParametricCdf& ccdf
			) ;



 private:

  typedef typename std::iterator_traits<InputIterator>::value_type Covariance;
  typedef typename std::vector<Covariance>::const_iterator Covariance_iterator;
  std::vector<Covariance> covariances_;
  KrigingConstraint Kconstraints_;

  /* This is here to avoid allocating a vector of weights each time
   * an indicator kriging is used. When an indicator kriging estimator is
   * copied, the content of weights is not copied. 
   * weights_ can contain 50 weights by default (it is automatically resized
   * if more weights are needed).
   */
  mutable std::vector<double> weights_;
  

}; // end of class Const_indicator_cdf_estimator.








template
<
  class InputIterator,
  class KrigingConstraint,
  class MatrixLibrary
>
template
<
  class Geovalue_,
  class InputIterator2,
  class NonParametricCdf
>
inline int 
Const_indicator_cdf_estimator<
                              InputIterator,
                              KrigingConstraint,
                              MatrixLibrary
                             >::
operator()(
	   const Geovalue_& u,
	   InputIterator2 first_neigh, InputIterator2 last_neigh,
	   NonParametricCdf& ccdf
	   ) {
			  
  Covariance_iterator current_covar = covariances_.begin();
 Covariance_iterator end_covar = covariances_.end();

  int current_neigh_id = -1;
  int neighborhoods_nb = std::distance(first_neigh, last_neigh);
  gstl_assert( neighborhoods_nb == ccdf.size() );

  
  typename NonParametricCdf::p_iterator p_it=ccdf.p_begin();

  for(typename NonParametricCdf::z_iterator z_it=ccdf.z_begin();
      z_it != ccdf.z_end(); ++z_it, ++p_it, ++first_neigh) {
    
    int status = kriging_weights<MatrixLibrary>(weights_,
						u.location(), **first_neigh,
						*(*current_covar)->raw_ptr(), Kconstraints_);
      
    // If the kriging system could not be solved, exit.
    if(status!=0)
      return status;
    
    // If there are covariances left, select the next one
    if(current_covar+1 != end_covar)
      current_covar++;
      
    // Combine the kriging weights and the indicator values of the conditioninge
    // data.
    typedef std::iterator_traits<InputIterator2>::value_type temp_type1;
	typedef temp_type1::pointed_t NeighborhoodType;
    typedef typename NeighborhoodType::const_iterator const_iterator;
    
    int ind = 0;
    double prob = 0;
    for(const_iterator neigh_it = (*first_neigh)->begin(); 
	neigh_it != (*first_neigh)->end() ; ++neigh_it ) {
      gstl_assert( ind < (int)weights_.size() );
      prob += (*neigh_it).get_value() * weights_[ind++];
    }
    *p_it = prob;
    
  }

  // Now ccdf has been estimated, correct order relation problems
  bool ok = make_cdf_valid(ccdf);
  
  
  return !ok ;
}



//=========================================================

/** Helper function. This function creates an instance of 
 * Const_indicator_cdf_estimator. 
 */

template
<
  class InputIterator,
  class KrigingConstraint,
  class MatrixLibrary
>
Const_indicator_cdf_estimator<
                              InputIterator,
                              KrigingConstraint,
                              MatrixLibrary
                             >
const_indicator_cdf_estimator(
			      InputIterator first_covar, InputIterator last_covar,
			      KrigingConstraint& Kconstraints 
			      )
{
  return Const_indicator_cdf_estimator<
             InputIterator,
             KrigingConstraint,
             MatrixLibrary
           >(first_covar, last_covar, Kconstraints) ;
} 



template
<
  class InputIterator,
  class KrigingConstraint
>
Const_indicator_cdf_estimator<
                              InputIterator,
                              KrigingConstraint,
                              GSTL_TNT_lib
                             >
const_indicator_cdf_estimator(
			      InputIterator first_covar, InputIterator last_covar,
			      KrigingConstraint& Kconstraints 
			      )
{
  return Const_indicator_cdf_estimator<
             InputIterator,
             KrigingConstraint,
             GSTL_TNT_lib
           >(first_covar, last_covar, Kconstraints) ;
} 


#endif
