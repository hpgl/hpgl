#ifndef __GSTL_UTILS_HELPER_FUNCTIONS_H__
#define __GSTL_UTILS_HELPER_FUNCTIONS_H__


namespace GsTL {
  
  /** Return true if range [first,last) is sorted in ascending order
   */
  template < class ForwardIterator >
  bool is_sorted( ForwardIterator first, ForwardIterator last ) {
    if( first == last ) return true;

    ForwardIterator next = first;
    for( ++next ; next != last; first=next, ++next ) {
      if( *next < *first )
	return false;
    }
    
    return true;
  }
}

#endif
