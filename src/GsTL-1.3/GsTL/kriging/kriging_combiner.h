#ifndef __GSTL_KRIGING_KRIGING_COMBINER_H__
#define __GSTL_KRIGING_KRIGING_COMBINER_H__

#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/kriging/linear_combination.h>

#include <numeric>



/** A Kriging_combiner is a Functor that combines kriging weights
 * and property values to obtain a kriging estimate.
 *  Kriging_combiner is the base class to many different kriging combiners.
 */
template
<
  class InputIterator,
  class Neighborhood
>
class Kriging_combiner_impl {
 public:
  virtual ~Kriging_combiner_impl() {}

  virtual Kriging_combiner_impl<InputIterator,Neighborhood>* clone() const {
    return new Kriging_combiner_impl<InputIterator,Neighborhood>;
  }

  virtual double operator() ( InputIterator begin_weights, InputIterator end_weights,
			      const Neighborhood& neighbors ) const {
    return linear_combination( begin_weights, end_weights, neighbors );
  }
};



/** This class is a handle for a Kriging combiner that complies with the 
 * Functor requirements, ie it has a copy constructor, an assignement operator.
 * Kriging_combiner_impl does not have (a meaningful) copy constructor or assignement
 * operator, hence does not comply with the Functor requirements. 
 */
template
<
  class InputIterator,
  class Neighborhood
>
class Kriging_combiner {
 private:
  typedef InputIterator I;
  typedef Neighborhood N;
  
 public:
  // Satisfy the Functor requirements, ie Assignable
  Kriging_combiner( const Kriging_combiner<I,N>& rhs ) {
    if( rhs.combiner_ == 0 )
      combiner_ = 0;
    else
      combiner_ = rhs.combiner_->clone();
  }
  Kriging_combiner<I,N>& operator = ( const Kriging_combiner<I,N>& rhs ) {
    if( rhs.combiner_ == combiner_ ) return *this;
    delete combiner_;
    if( rhs.combiner_ == 0 )
      combiner_ = 0;
    else
      combiner_ = rhs.combiner_->clone();
    return *this;
  }

 public:
  Kriging_combiner( Kriging_combiner_impl<I,N>* combiner = 0 ) {
    if( combiner ) 
      combiner_ = combiner->clone();
    else
      combiner_ = new Kriging_combiner_impl<I,N>();
  }

  ~Kriging_combiner() { delete combiner_; }

  inline double operator() ( InputIterator begin_weights, 
			     InputIterator end_weights,
			     const Neighborhood& neighbors ) const {
    return (*combiner_)( begin_weights, end_weights, neighbors );
  }

 private:
  Kriging_combiner_impl<I,N>* combiner_;
}; 




/** SK_combiner combines the kriging weights and the data values to find
 * the simple kriging estimate. The mean of the estimated attribute is 
 * known and constant.
 */
template
<
  class InputIterator,
  class Neighborhood
>
class SK_combiner : public Kriging_combiner_impl<InputIterator,Neighborhood> {
 public:
  SK_combiner( double mean ) : mean_( mean ) {}

  virtual Kriging_combiner_impl<InputIterator,Neighborhood>* clone() const {
    return new SK_combiner<InputIterator,Neighborhood>( mean_ );
  }

  virtual double operator() ( InputIterator begin_weights, 
       			      InputIterator end_weights,
			      const Neighborhood& neighbors ) const {
    double val = linear_combination( begin_weights, end_weights, neighbors );
    double sum = std::accumulate( begin_weights, end_weights, 0.0 );
    val += (1-sum)*mean_;
    return val;
  }

 protected:
  double mean_;
};



/** SK_local_mean_combiner combines the kriging weights and the data values
 * to find the simple kriging estimate, accounting for a locally varying mean.
 * The constructor expects a Neighborhood that enables to retrieve the local 
 * mean. 
 */
template
<
  class InputIterator,
  class Neighborhood,
  class MeanNeighborhood
>
class SK_local_mean_combiner : public Kriging_combiner_impl<InputIterator,Neighborhood> {
 public:

  // Constructor
  SK_local_mean_combiner( const MeanNeighborhood& mean_neigh )
    : mean_neigh_( mean_neigh ) {}

  // Clone function
  virtual Kriging_combiner_impl<InputIterator,Neighborhood>* clone() const {
    return new 
      SK_local_mean_combiner<InputIterator,
                             Neighborhood,
                             MeanNeighborhood>( mean_neigh_ );
  }

  // operator()
  virtual double operator() ( InputIterator begin_weights,
                              InputIterator end_weights,
			      const Neighborhood& neighbors ) const {
    if( begin_weights == end_weights ) {
      gstl_warning( "No weights provided" );
      return -999;
    }
    typedef typename MeanNeighborhood::iterator mean_iterator;

    double result=0.0;

    typename Neighborhood::const_iterator it= neighbors.begin();
    typename Neighborhood::const_iterator end = neighbors.end();
    for( ; it != end ; ++it ) {
      mean_neigh_.find_neighbors( *it );
      mean_iterator mean_ptr = mean_neigh_.begin();
      result += 
	(*begin_weights) * ( it->property_value() - mean_ptr->property_value() );
      begin_weights++;
    }
    
    mean_neigh_.find_neighbors( neighbors.center() );
    mean_iterator center_mean_ptr = mean_neigh_.begin();
    result += center_mean_ptr->property_value();
    return result;
  }

 protected:
  mutable MeanNeighborhood mean_neigh_;
};


#endif
