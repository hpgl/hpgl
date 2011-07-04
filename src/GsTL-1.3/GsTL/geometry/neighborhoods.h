#ifndef __GSTL_GEOMETRY_NEIGHBORHOODS_H__
#define __GSTL_GEOMETRY_NEIGHBORHOODS_H__

//#include <GsTL/common.h>

#include <iterator>

//BEGIN_GSTL_NAMESPACE

/** Compare two ranges of geovalues. The function returns the number of
* differences between the two ranges. Only the property values are compared,
* the location of each geovalue is ignored.
* Pre-condition: if n is the number of elements in range 1, there must be
* at least n elements in range 2.
*/
template < class ForwardIterator1, class ForwardIterator2 >
unsigned int differences( ForwardIterator1 first1, ForwardIterator1 last1,
                      ForwardIterator2 first2 ) {
  int diff = 0;
  for( ; first1 != last1 ; first1++, first2++ ) {
    if( !( first1->is_informed() && first2->is_informed() ) ) {
      if( first1->is_informed() || first2->is_informed() )
      diff++;
      continue;
    }

    if( first1->property_value() != first2->property_value() ) 
      diff++;
  } 

  return diff;
}


/** Compare two ranges of geovalues. In this version, the comparison is
* performed up to a \c max number of differences. If there are less differences
* than \c max, the functions returns the actual number of differences. However,
* if the number of differences exceeds \c max, \c max+1 is returned.
* Pre-condition: if n is the number of elements in range 1, there must be
* at least n elements in range 2.
*/
template < class ForwardIterator1, class ForwardIterator2 >
unsigned int differences( ForwardIterator1 first1, ForwardIterator1 last1,
                      ForwardIterator2 first2, 
                      unsigned int max ) {
  int diff = 0;
  for( ; first1 != last1 ; first1++, first2++ ) {
    if( diff > max ) return (max+1);

    if( !( first1->is_informed() && first2->is_informed() ) ) {
      if( first1->is_informed() || first2->is_informed() )
      diff++;
      continue;
    }

    if( first1->property_value() != first2->property_value() ) 
      diff++;

  } 

  return diff;
}


/** Compute the difference between two ranges of geovalues. If two geovalues
* have a different property value, the difference is updated by an amount 
* determined by UnaryFunction \c kernel. kernel is a functor that takes a 
* geovalue as input and return a weight. 
* weighted_differences allows to give more weight to a difference that occur
* for some geovalues than for others.
*/
template < 
  class ForwardIterator1, 
  class ForwardIterator2, 
  class UnaryFunction 
>
double weighted_differences( ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2, 
                             UnaryFunction kernel, double max ) {
  double diff = 0;
  for( ; first1 != last1 ; first1++, first2++ ) {
    if( diff > max ) return diff;

    if( !( first1->is_informed() && first2->is_informed() ) ) {
      if( first1->is_informed() || first2->is_informed() )
      diff += kernel( *first1 );
      continue;
    }

    if( first1->property_value() != first2->property_value() ) 
      diff += kernel( *first1 );

  } 

  return diff;
}



/** Extracts the geometry defined by the informed geovalues in range [first,last),
* ie the vectors CU, where C is the center of the neighborhood, and
* U is an informed node in the neighborhood.
* This function can be used to define a window neighborhood from
* the neighbors retrieved by an ellipsoid neighborhood, or by another window.
* The vectors defining the geometry are written to \c out. 
* Pre-condition: the output container is large enough to hold all the geometry
* vectors. 
*/
template < class ForwardIterator, class Location_, class OutputIterator >
void extract_geometry( ForwardIterator first, ForwardIterator last,
                       const Location_& center, //const std::iterator_traits<ForwardIterator>::value_type::location_type center,
                       OutputIterator out ) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type::location_type::difference_type EuclideanVector;
  for( ; first != last; ++first, ++out ) {
    if( first->is_informed() ) {
      *out = first->location() - center;
    }
  }
}




//END_GSTL_NAMESPACE


#endif
