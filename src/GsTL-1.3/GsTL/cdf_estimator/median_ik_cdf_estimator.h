#ifndef __GSTL_CDF_ESTIMATOR_MEDIAN_IK_CDF_ESTIMATOR_H__
#define __GSTL_CDF_ESTIMATOR_MEDIAN_IK_CDF_ESTIMATOR_H__

#include <GsTL/kriging.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/cdf/cdf_basics.h>

#include <vector>
#include <iterator>
#include <algorithm>

/** MedianIK_cdf_estimator is a model of CdfEstimator. It estimates a cdf
* using median indicator kriging (i.e. indicator kriging, using the same
* covariance and same data configuration for each indicator). 
* MedianIK_cdf_estimator will only estimate the probabilities of the cdf
* that are not known. Ex: the cdf is defined by 4 probability values,
* corresponding to 4 indicator variables. Because of some prior knowledge
* of the problem, we know the 2 first probabilities: p1=0.1, p2=0.3.
* The cdf passed to MedianIK_cdf_estimator should then be: 
* { 0.1 , 0.3 , ? , ? }. MedianIK_cdf_estimator will only estimate p3 and p4,
* using the first 4 neighborhood in range [first_neigh, last_neigh).
*/
template < 
          class KrigingConstraints,
          class Combiner_,
          class CovarianceSet,
          class MatrixLibrary=GSTL_TNT_lib
         >
class MedianIK_cdf_estimator {
public:
  template< class InputIterator, class InputIterator2 >
  MedianIK_cdf_estimator( InputIterator2 marginal_begin, 
                          InputIterator2 marginal_end,
                          const CovarianceSet& covarset,
                          InputIterator first_combiner, 
                          InputIterator last_combiner,
                          const KrigingConstraints& kconstraints )
    : weights_( 50 ),
      combiners_( first_combiner, last_combiner ),
      kconstraints_( kconstraints ),
      covariance_set_( covarset ),
      marginal_( marginal_begin, marginal_end ) {

    gstl_assert( is_valid_cdf( marginal_begin, marginal_end, 
                               GsTL::continuous_variable_tag() ) ||
                 is_valid_cdf( marginal_begin, marginal_end, 
                               GsTL::discrete_variable_tag() ) );
  }



  template< class GeoValue, class InputIterator, class NonParametricCdf >
  int operator() ( const GeoValue& u, //const typename std::iterator_traits<InputIterator>::value_type::value_type& u,
                   InputIterator first_neigh, 
				   InputIterator last_neigh,
                   NonParametricCdf& ccdf ) const {
  
    typedef typename NonParametricCdf::p_iterator CdfIterator;

    gstl_assert( ccdf.size() == int(marginal_.size()) );

    // First check that there are conditioning data. If not, return the
    // marginal
    InputIterator soft_neigh = first_neigh;
    soft_neigh++;
    if( first_neigh->is_empty() && soft_neigh->is_empty() ) {
      std::copy( marginal_.begin(), marginal_.end(), ccdf.p_begin() );
      return 0;
    }


    weights_.clear();

    // Compute the cokriging weights
    InputIterator current_plus_2 = first_neigh;
    std::advance( current_plus_2, 2 );

    int status =
      cokriging_weights<MatrixLibrary>( weights_, 
                                        u.location(), first_neigh, current_plus_2,
                                        covariance_set_, kconstraints_ );
    if( status != 0 ) {
      std::copy( marginal_.begin(), marginal_.end(), ccdf.p_begin() );      
      return status;
    }

    int k=0;
    for( CdfIterator p_it = ccdf.p_begin() ;
         p_it != ccdf.p_end() ;
         ++p_it, ++k ) {

      if( *p_it != NonParametricCdf::NaN ) continue;

      // the probability for the current threshold is not known: estimate it
      // by cokriging
      current_plus_2 = first_neigh;
      std::advance( current_plus_2, 2 );

      *p_it = combiners_[k]( weights_.begin(), weights_.end(),
                             first_neigh, current_plus_2 );

      std::advance( first_neigh, 2 );
    }

    bool ok = ccdf.make_valid();
    if( !ok ) 
      std::copy( marginal_.begin(), marginal_.end(), ccdf.p_begin() );

    return !ok;
  }


private:
  mutable std::vector<double> weights_;
  std::vector< Combiner_ > combiners_;
  KrigingConstraints kconstraints_;
  CovarianceSet covariance_set_;
  std::vector<double> marginal_;
};

#endif
