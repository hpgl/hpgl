#ifndef __GSTL_PROPERTY_ARRAY_H__
#define __GSTL_PROPERTY_ARRAY_H__


#include <cassert>

/** A property_array is simply an array of values, with some basic functionnalities.
 */


template <class T>
class property_array{

public:
  typedef T property_type;
  typedef T* iterator;
  typedef const T* const_iterator;
  
  property_array(int size=0);
  property_array(T initial_value, int size);

  ~property_array();
  
  inline const property_type& operator[](int index) const;
  inline property_type& operator[](int index);

  inline int size() const ;
  
  inline iterator begin();
  inline const_iterator begin() const;
  inline iterator end();
  inline const_iterator end() const;
  
private:
  int size_;
  property_type* values_;

};




template<class T>
property_array<T>::property_array(int size) 
    : size_(size) {
  values_ = new property_type[size];
}


template<class T>
property_array<T>::property_array(T initial_value, int size) 
    : size_(size) {

  values_ = new property_type[size];

  for(int i=0; i<size; i++)
    values_[i] = initial_value;

}


template<class T>
property_array<T>::~property_array(){
  delete [] values_;
}

template<class T>
inline const typename property_array<T>::property_type&
property_array<T>::operator[](int index) const {

  assert(index >=0 && index < size_);
  return values_[index];
}


template<class T>
inline typename property_array<T>::property_type&
property_array<T>::operator[](int index) {

  assert(index >=0 && index < size_);
  return values_[index];
}



template<class T>
inline int property_array<T>::size() const {

  return size_;
}




template<class T>
inline typename property_array<T>::iterator 
property_array<T>::begin() {

  return values_;
}



template<class T>
inline typename property_array<T>::const_iterator 
property_array<T>::begin() const {

  return values_;
}


template<class T>
inline typename property_array<T>::iterator 
property_array<T>::end() {

  return values_+size_;
}



template<class T>
inline typename property_array<T>::const_iterator 
property_array<T>::end() const {

  return values_+size_;
}


#endif
