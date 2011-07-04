#ifndef __GSTL_ITERATOR_ITERATORS_H__
#define __GSTL_ITERATOR_ITERATORS_H__

#include <iterator>

template < class Iterator_, class Predicate >
class Skip_iterator {

protected:
  Iterator_ current_;
  Iterator_ end_;
  Predicate skip_condition_;

  typedef Skip_iterator<Iterator_> Self_;
  typedef std::iterator_traits<Iterator> Traits;


public:
  typedef Traits::value_type         value_type;
  typedef Traits::difference_type    difference_type;
  typedef Traits::reference          reference;
  typedef Traits::pointer            pointer;
  typedef Traits::iterator_category  iterator_category;

  typedef Iterator_                  iterator_type;


  void make_valid( iterator_type& it ) {
    if( it == end_ ) return;
    while( skip_condition_( it ) && it != end_ ) 
      ++it;
  }


public:
  Skip_iterator() {}

  explicit Skip_iterator( iterator_type current ) 
    : current_( current ), end_( current ) {}

  explicit Skip_iterator( iterator_type current, iterator_type end ) 
    : current_( current ), end_( end ) {
    make_valid( current_ );
  }

  explicit Skip_iterator( iterator_type current, iterator_type end, 
                          Predicate cond ) 
    : current_( current ), end_( end ), skip_condition_( cond ) {
    make_valid( current_ );
  }

  iterator_type base() const { return current_; }
  iterator_type& base() { return current_; }

  reference operator * () {
    return *current_;
  }

  pointer operator -> () {
    return &( operator*() );
  }

  Self_& operator ++ () {
    if( current_ == end_ ) return *this;
    ++current_;

    make_valid( current_ );
    return *this;
  }

  Self_ operator ++ (int) {
    if( current_ == end_ ) return *this;
    current_++;

    make_valid( current_ );
    return *this;
  }

  bool operator == ( const Self_& rhs ) { return current_ == rhs.current_; }
  bool operator != ( const Self_& rhs ) { return current_ != rhs.current_; }
};



#endif