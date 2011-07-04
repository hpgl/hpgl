#ifndef __GSTL_CDF_ESTIMATOR_VARIANCE_RECORDER_ADAPTER_H__
#define __GSTL_CDF_ESTIMATOR_VARIANCE_RECORDER_ADAPTER_H__


/** Variance_recorder_t is a "class adapter" that records the 
 * variance of the ccdf computed by a CdfEstimator. In the current
 * implementation, it is assumed that the estimated ccdf is a model
 * of concept GaussianCdf.
*/
template< class CdfEstimator, class ColocatedNeighborhood >
class Variance_recorder_t {
public:
  Variance_recorder_t( CdfEstimator& cdf_estimator,
                     ColocatedNeighborhood* coloc_neigh ) 
    : cdf_estimator_( cdf_estimator ),
      coloc_neigh_( coloc_neigh ) { }

  template< class Neighborhood_, class GaussianCdf >
  int operator( const typename Neighborhood_::value_type& u,
                const Neighborhood_& neighbors,
                GaussianCdf& ccdf ) {
    int status = cdf_estimator_( u, neighbors, ccdf );
    coloc_neigh_->find_neighbors( neighbors.center() );
    coloc_neigh_->begin()->set_property_value( ccdf.variance() );

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
                        ) const {
    int status = cdf_estimator_( u, first_neigh, last_neigh, ccdf );
    coloc_neigh_->find_neighbors( first_neigh->center() );
    coloc_neigh_->begin()->set_property_value( ccdf.variance() );
  }


private:
  CdfEstimator& cdf_estimator_;
  ColocatedNeighborhood* coloc_neigh_;
};


template< class CdfEstimator, class ColocatedNeighborhood >
Variance_recorder_t< CdfEstimator, ColocatedNeighborhood >
variance_recorder( CdfEstimator& cdf_estimator,
                   ColocatedNeighborhood* coloc_neigh ) {
  return Variance_recorder_t< CdfEstimator, 
                              ColocatedNeighborhood >( cdf_estimator, 
                                                       coloc_neigh );
}
#endif
