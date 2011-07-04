#ifndef __GSTL_CDF_ESTIMATOR_EFROS_CDF_ESTIMATOR_H__
#define __GSTL_CDF_ESTIMATOR_EFROS_CDF_ESTIMATOR_H__

#include <GsTL/geometry/neighborhoods.h>

#include <algorithm>
#include <fstream>

template< class ForwardIterator, class WindowNeighborhood >
class Efros_cdf_estimator {
public:
  Efros_cdf_estimator( ForwardIterator first, ForwardIterator last,
                       WindowNeighborhood& window ) 
    : first_( first ), last_( last ),
      window_( window ) {
//    dbgstream_ = new std::ofstream( "efrosim.dbg" );
  }


  /** This only doesn't work with window-neighborhoods because it assumes
  * that all geovalues in \c neighbors are informed, which is not garanted by
  * window neighborhoods. It is assumed that the values of the training image
  * range from 0 to K-1, where K is the total number of categories.
  */
  template< class Neighborhood_, class CategNonParametricCdf >
  int operator()(
      const typename Neighborhood_::value_type& u,
			const Neighborhood_& neighbors,
			CategNonParametricCdf& ccdf
      ) const {
    
    
    typedef typename Neighborhood_::value_type::location_type::
                        difference_type EuclideanVector;
    std::vector<EuclideanVector> extracted_geometry;
    extract_geometry( neighbors.begin(), neighbors.end(),
                      neighbors.center().location(), 
                      std::back_inserter( extracted_geometry ) );
    window_.clear_geometry();
 
//    (*dbgstream_) << "data event geometry:  "; 
    typename std::vector<EuclideanVector>::iterator geom_it = extracted_geometry.begin();
    for( ; geom_it != extracted_geometry.end(); ++geom_it ) {
//      (*dbgstream_) << "(" << geom_it->x() << "," << geom_it->y() << "," << geom_it->z() << ")  ";
      window_.add_geometry_vector( *geom_it );
    }
//    (*dbgstream_) << std::endl;

/*
    (*dbgstream_) << "data event: ";
    for( typename Neighborhood_::iterator neighit = neighbors.begin();
      neighit != neighbors.end(); ++neighit ) {
        (*dbgstream_) << neighit->property_value() << " ";
      }
    (*dbgstream_) << std::endl;
*/
    unsigned int max_differences = neighbors.size();
    std::vector< int > cpdf( ccdf.size(), 0 );

    // scan the training image, looking for approximate replicates
    for( ForwardIterator it = first_; it != last_; ++it ) {
      window_.find_neighbors( *it );
      if( window_.size() != neighbors.size() ) continue;
/*     
      for( typename WindowNeighborhood::iterator winit = window_.begin();
      winit != window_.end(); ++winit ) {
        (*dbgstream_) << winit->property_value() << " ";
      }
      (*dbgstream_) << std::endl;
*/
      unsigned int diff = differences( window_.begin(), window_.end(),
                                       neighbors.begin(), max_differences );
      if( diff > max_differences ) continue;

      if( diff < max_differences ) {
        cpdf.assign( cpdf.size(), 0 );
        max_differences = diff;
      }

      unsigned int categ = static_cast< unsigned int >( it->property_value() );
      gstl_assert( categ >= 0 && categ < cpdf.size() );
      cpdf[ categ ] ++;
    }

    // now turn the recorded numbers into proportions and assign the result to ccdf
    int sum = std::accumulate( cpdf.begin(), cpdf.end(), 0 );

    gstl_assert( sum );
//    (*dbgstream_) << "Found " << sum << "dev, max difference: " << max_differences << std::endl;
//    (*dbgstream_) << "cpdf: " << cpdf[0] <<" " << cpdf[1] << std::endl ;
    std::transform( cpdf.begin(), cpdf.end(), 
                    ccdf.p_begin(), 
                    std::bind2nd( std::divides<double>(), double( sum ) ) );
//    (*dbgstream_) << "ccdf: " << ccdf.prob( 0 ) << " " << ccdf.prob( 1 ) << std::endl ;

    return 0;
  }


private:
  ForwardIterator first_, last_;
  WindowNeighborhood& window_;
//  std::ofstream* dbgstream_;
};

#endif
