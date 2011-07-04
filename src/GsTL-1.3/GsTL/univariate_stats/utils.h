#ifndef __GSTL_UNIVARIATE_STATS_UTILS_H__
#define __GSTL_UNIVARIATE_STATS_UTILS_H__

namespace GsTL {

/** This function computes the mean of a range of values
 * If the range is empty, the function returns 0.
*/
template <class InputIterator>
double mean( InputIterator begin, InputIterator end ) {
  double res = 0.0;
  double count = 0.0;
  for( ; begin != end ; ++begin, count++ ) 
    res += *begin;

  if( count == 0 ) return 0;

  return res / count ;
}


/** This function computes the variance of a range of values.
* If pointer \c mean is not null, the mean of the range is stored
* in *mean.
*/
template <class InputIterator, class T>
double variance( InputIterator begin, InputIterator end, T* mean = 0 ) {
  double res = 0;
  double mean_ = 0;
  double count = 0;
  for( ; begin != end ; ++begin, count++ ) {
    res += (*begin) * (*begin);
    mean_ += *begin;
  }

  if( count == 0 ) return 0;
  mean_ /= count;
  if ( mean ) *mean = static_cast<T>(mean_);
  
  if( count == 1 ) return 0;
 
  return res / ( count - 1 ) - count/(count-1)*mean_*mean_;
}

/** This function computes the variance of a range of values.
*/
template <class InputIterator>
double variance( InputIterator begin, InputIterator end ) {
  return variance( begin, end, (double*) 0 );
}




/** This function computes the covariance of a range of values.
* If pointer \c means is not null, the means of both ranges are stored
* in *means.
*/
template <class InputIterator, class InputIterator2>
double covariance( InputIterator begin, InputIterator end, 
                   InputIterator2 begin2, 
		   std::pair<double,double>* means = 0 ) {
  double res = 0;
  double mean1_ = 0;
  double mean2_ = 0;
  double count = 0;
  for( ; begin != end ; ++begin, count++, ++begin2 ) {
    res += (*begin) * (*begin2);
    mean1_ += *begin;
    mean2_ += *begin2;
  }

  if( count == 0 ) return 0;

  mean1_ /= count;
  mean2_ /= count;
  if ( means ) *means = std::make_pair(mean1_, mean2_);

  if( count == 1 ) return 0;

  return res / ( count - 1 ) - count/(count-1)*mean1_*mean2_;
}

} // end of GsTL namespace

#endif
