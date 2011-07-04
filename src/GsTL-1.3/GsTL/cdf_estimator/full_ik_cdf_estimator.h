#ifndef __GSTL_CDF_ESTIMATOR_FULL_IK_CDF_ESTIMATOR_H__
#define __GSTL_CDF_ESTIMATOR_FULL_IK_CDF_ESTIMATOR_H__


#include <GsTL/kriging.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/utils/debug_tools.h>

#include <vector>
#include <iterator>


template < 
          class KrigingConstraints,
          class Combiner_,
          class CovarianceSet,
          class MatrixLibrary=GSTL_TNT_lib
         >
class FullIK_cdf_estimator {
public:

  template< class InputIterator, class InputIterator2, class InputIterator3 >
  FullIK_cdf_estimator( InputIterator3 marginal_begin, InputIterator3 marginal_end,
                        InputIterator first_covarset, InputIterator last_covarset,
                        InputIterator2 first_combiner, InputIterator2 last_combiner,
                        const KrigingConstraints& kconstraints )
    : weights_( 50 ),
      combiners_( first_combiner, last_combiner ),
      kconstraints_( kconstraints ),
      covar_sets_( first_covarset, last_covarset ),
      marginal_( marginal_begin, marginal_end ) {
  }



  template< class GeoValue, class InputIterator, class NonParametricCdf >
  int operator () ( const GeoValue& u, //const typename std::iterator_traits<InputIterator>::value_type::value_type& u,
                    InputIterator first_neigh, InputIterator last_neigh,
                    NonParametricCdf& ccdf ) const {

    weights_.clear();

    typedef typename NonParametricCdf::p_iterator CdfIterator;
    int k=0 ;
    for( CdfIterator p_it = ccdf.p_begin();
         p_it != ccdf.p_end() ;
         ++p_it, ++k ) {
      if( *p_it != NonParametricCdf::NaN ) continue;

      // the probability for the current threshold is not known: estimate it
      // by cokriging if there are any conditioning data; otherwise use the
      // marginal value.
      InputIterator hard_neigh = first_neigh;
      InputIterator soft_neigh = first_neigh;  ++soft_neigh;
      if( hard_neigh->is_empty() && soft_neigh->is_empty() ) {
        *p_it = marginal_[k];
        continue;
      }

      InputIterator current_plus_2 = first_neigh;
      std::advance( current_plus_2, 2 );

      int status =
        cokriging_weights<MatrixLibrary>( weights_, 
                                          u.location(), first_neigh, current_plus_2,
                                          covar_sets_[k], kconstraints_ );
      if( status != 0 ) {
        std::copy( marginal_.begin(), marginal_.end(), ccdf.p_begin() );
        return status;
      }
      
      DEBUG_PRINT_RANGE( "weights: ", weights_.begin(), weights_.end() );

      *p_it = combiners_[k]( weights_.begin(), weights_.end(),
                             first_neigh, current_plus_2 );
      WRITE_TO_DEBUG_STREAM( "prob=" << *p_it << "\n" );

      std::advance( first_neigh, 2 );
    }

    bool ok = ccdf.make_valid();
    if( !ok ) {
      WRITE_TO_DEBUG_STREAM( "\nmake_cdf_valid failed\n\n"  );
      std::copy( marginal_.begin(), marginal_.end(), ccdf.p_begin() );
    }
    return !ok;
  }



private:
  mutable std::vector<double> weights_;
  std::vector< Combiner_ > combiners_;
  KrigingConstraints kconstraints_;
  std::vector<CovarianceSet> covar_sets_;
  std::vector<double> marginal_;
};


#endif
