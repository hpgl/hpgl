#ifndef __GSTL_KRIGING_COKRIGING_COMBINER_H__
#define __GSTL_KRIGING_COKRIGING_COMBINER_H__

#include <GsTL/kriging/multi_linear_combination.h>

#include <numeric>
#include <vector>


/** A CoKriging_combiner is a Functor that combines kriging weights
 * and property values to obtain a kriging estimate.
 *  CoKriging_combiner is the base class to many different cokriging combiners.
 */
template
<
  class InputIterator,
  class InputIterator2
>
class CoKriging_combiner_impl {

 private:
  typedef CoKriging_combiner_impl<InputIterator,
                                  InputIterator2> CoKrigingCombinerImpl;
  typedef InputIterator  InIter;
  typedef InputIterator2 InIter2;


 public:
  virtual ~CoKriging_combiner_impl() {}

  virtual CoKrigingCombinerImpl* clone() const {
    return new CoKrigingCombinerImpl;
  }

  virtual double operator()( InIter begin_weights, InIter , 
			     InIter2 first_neigh, InIter2 last_neigh) const {
    return multi_linear_combination( begin_weights,
				     first_neigh, last_neigh);
  }
};



/** This class is a handle for a CoKriging combiner that complies with the 
 * Functor requirements, ie it has a copy constructor, an assignement operator.
 * CoKriging_combiner_impl does not have (a meaningful) copy constructor or 
 * assignement operator, hence does not comply with the Functor requirements. 
 */
template
<
  class InputIterator,
  class InputIterator2
>
class CoKriging_combiner {
 protected:
  typedef CoKriging_combiner<InputIterator,InputIterator2> CoKrigingCombiner;
  typedef CoKriging_combiner_impl<InputIterator,
                                  InputIterator2> CoKrigingCombinerImpl;
  typedef InputIterator  InIter;
  typedef InputIterator2 InIter2;


 public:
  CoKriging_combiner( const CoKrigingCombinerImpl* combiner = 0 ) {
    if( combiner ) 
      combiner_ = combiner->clone();
    else
      combiner_ = new CoKrigingCombinerImpl;
  }

  ~CoKriging_combiner() { delete combiner_; }

  inline double operator()( InIter begin_weights, InIter end_weights,
			    InIter2 first_neigh, InIter2 last_neigh) const {
    return (*combiner_)( begin_weights, end_weights, first_neigh, last_neigh );
  }

 public:
  // Satisfy the Functor requirements, ie Assignable
  CoKriging_combiner( const CoKrigingCombiner& rhs ) {
    if( rhs.combiner_ )
      combiner_ = rhs.combiner_->clone();
    else
      combiner_ = 0;
  }
  const CoKrigingCombiner& operator = ( const CoKrigingCombiner& rhs ) {
    if( rhs.combiner_ == combiner_ ) return *this;
    delete combiner_;
    if( combiner_ )
      combiner_ = rhs.combiner_->clone();
    else
      combiner_ = 0;
    return *this;
  }


 private:
  CoKrigingCombinerImpl* combiner_;
}; 






/** CoSK_combiner combines the kriging weights and the data values to find
 * the simple kriging estimate. The mean of the estimated attribute and 
 * secondary attributes are known and constant.
 */
template
<
  class InputIterator,
  class InputIterator2
>
class CoSK_combiner : public CoKriging_combiner_impl<InputIterator,InputIterator2> {

 private:
  typedef CoKriging_combiner_impl<InputIterator,
                                  InputIterator2> CoKrigingCombinerImpl;
  typedef InputIterator  InIter;
  typedef InputIterator2 InIter2;

 public:

  // constructor
  CoSK_combiner( std::vector<double> means ) : means_( means ) {}

  // clone 
  virtual CoKrigingCombinerImpl* clone() const {
    return new CoSK_combiner<InputIterator,InputIterator2>( means_ );
  }

  // operator ()
  virtual double operator()( InIter begin_weights, InIter ,
			     InIter2 first_neigh, InIter2 last_neigh ) const {

    double val = multi_linear_combination(  begin_weights,
					    first_neigh, last_neigh);
    double mean_contribution = 0.0;
    int attribute = 0;
    for( ; first_neigh != last_neigh; ++first_neigh, ++attribute ) {
      // Sum all the weights corresponding to a single attribute and multiply
      // the sum by the mean of the attribute
      InIter end = begin_weights;
      std::advance( end, first_neigh->size() );
      double sum = 
	std::accumulate( begin_weights, end, 0.0 );
      sum *= means_[attribute];
      mean_contribution += sum;

      std::advance( begin_weights, first_neigh->size() );
    }
    mean_contribution = means_[0] - mean_contribution;
    return val + mean_contribution;
  }

 protected:
  std::vector<double> means_;
};





#endif
