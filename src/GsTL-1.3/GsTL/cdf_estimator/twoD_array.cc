/*
template<class T>
twoD_array<T>::twoD_array(int nx, int ny) : nx_(nx), ny_(ny) {
    array_ = new value_type*[ny_];
    array_[0] = new value_type[nx_];
    already_allocated_=1;
    size_=0;
  }


template<class T>
twoD_array<T>::~twoD_array(){
  for(int i=0; i<already_allocated_; i++){
    delete [] array_[i];
  }
  delete [] array_;
}




template<class T>
int twoD_array<T>::size(){
  return (already_allocated_-1)*nx_+size_;
}
*/

/*
template<class T>
twoD_array<T>::twoD_array(int nx, int ny) : nx_(nx), ny_(ny), array_( ny, 0 ) {
    array_[0] = new value_type[nx_];
    already_allocated_=1;
    size_=0;
  }


template<class T>
twoD_array<T>::~twoD_array(){
  for(int i=0; i<already_allocated_; i++){
    delete [] array_[i];
	array_[i] = 0;
  }
}




template<class T>
int twoD_array<T>::size(){
  return (already_allocated_-1)*nx_+size_;
}


#ifdef __GNUC__
#pragma implementation
#endif
#include "twoD_array.h"
*/