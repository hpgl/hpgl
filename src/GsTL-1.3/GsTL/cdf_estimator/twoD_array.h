#ifndef _GTL_ALLOCATOR_TWO_D_ARRAY_
#define _GTL_ALLOCATOR_TWO_D_ARRAY_
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/utils/gstl_error_messages.h>

/** This class is designed solely to be used by the pool_allocator
 * of a search tree. It does not provide any access function, nor
 * does it perform any check on bounds...
 */
/*
template<class T>
class twoD_array{

 public:
  
  typedef T value_type;
  typedef T* iterator;

  twoD_array(int nx, int ny);
  ~twoD_array();

  inline value_type* push_back(value_type val);

  int size();


 private:
  
  inline void resize();

  value_type** array_;
  int nx_, ny_;
  int already_allocated_;
  int size_;
  int nb_of_resize_;

};





template<class T>
inline void twoD_array<T>::resize(){
  //  gstl_assert(already_allocated_ < ny_);
  array_[already_allocated_] = new value_type[nx_];
  already_allocated_++;
  size_ = 0;
  //  nb_of_resize_++;
}

template<class T>
inline typename twoD_array<T>::value_type*
twoD_array<T>::push_back( typename twoD_array<T>::value_type val ){
  if(size_ == nx_)
    resize();

  //  gstl_assert(size_>=0 && size_ < nx_);

  array_[already_allocated_-1][size_]=val;
  size_++;
  
  return &array_[already_allocated_-1][size_-1];
}
*/

#include <vector>

template<class T>
class twoD_array{

 public:
  
  typedef T value_type;
  typedef T* iterator;

  twoD_array(int nx, int ny);
  ~twoD_array();

  inline value_type* push_back(value_type val);

  int size();


 private:
  
  inline void resize();

  std::vector<value_type*> array_;
  int nx_, ny_;
  int already_allocated_;
  int size_;
  int nb_of_resize_;

};


template<class T>
twoD_array<T>::twoD_array(int nx, int ny) : nx_(nx), ny_(ny) {
    array_.push_back( new value_type[nx_] );
    already_allocated_=1;
    size_=0;
    nb_of_resize_ = 0;
  }


template<class T>
twoD_array<T>::~twoD_array(){
  for( int i=0; i< array_.size() ; i++ ) {
    delete [] array_[i];
	  array_[i] = 0;
  }
}




template<class T>
inline void twoD_array<T>::resize(){
//  gstl_assert(already_allocated_ < ny_);
//  array_[already_allocated_] = new value_type[nx_];
  array_.push_back( new value_type[nx_] );
  already_allocated_++;
  size_ = 0;
  nb_of_resize_++;
  gstl_message( "# of resize: " << nb_of_resize_ << "  while ny_=" << ny_ );
}

template<class T>
inline typename twoD_array<T>::value_type*
twoD_array<T>::push_back( typename twoD_array<T>::value_type val ){
  if(size_ == nx_)
    resize();

  //  gstl_assert(size_>=0 && size_ < nx_);

  array_[already_allocated_-1][size_]=val;
  size_++;
  
  return &array_[already_allocated_-1][size_-1];
}



template<class T>
int twoD_array<T>::size(){
  return (already_allocated_-1)*nx_+size_;
}

/*
#if defined(__GNUC__) || defined(WIN32)
#include "twoD_array.cc"
#elif defined(__xlC__)
#pragma implementation("twoD_array.cc")
#endif
*/

#endif
