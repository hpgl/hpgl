/* GsTL: the Geostatistics Template Library
 * 
 * Author: Nicolas Remy
 * Copyright (c) 2000 The Board of Trustees of the Leland Stanford Junior University
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *   1.Redistributions of source code must retain the above copyright notice, this 
 *     list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright notice, this 
 *     list of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution. 
 *   3.The name of the author may not be used to endorse or promote products derived 
 *     from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ___GRID_ITERATORS__
#define ___GRID_ITERATORS__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/grid/grid.h>

#include <iterator>


template 
<
  class DerivedType,
  class Grid
>
class base_grid_iterator {
 public:

  // STL iterator requirements
  // These could be omitted by inheriting base_grid_iterator from :
  //    std::iterator<std::forward_iterator_tag,
  //                  typename Grid::geovalue_type>
  // but g++ 2.95-3 complains that there is no class "iterator" in std ...
  typedef std::forward_iterator_tag iterator_category;
  typedef typename Grid::geovalue_type value_type;
  typedef ptrdiff_t difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  // end of requirements

  typedef typename Grid::geovalue_type GeoValue;

  base_grid_iterator(Grid* grid, int index, std::string property_name);

  DerivedType& asDerived() {
    return static_cast<DerivedType&>(*this);
  }

  inline base_grid_iterator& operator++(int);
  inline base_grid_iterator operator++() ;

  inline bool operator!=(const base_grid_iterator& it) const;
  inline bool operator==(const base_grid_iterator& it) const;
  inline GeoValue& operator*() ;
  inline GeoValue* operator->();
  inline int operator-(const base_grid_iterator& it) const;


 protected:
  int index_;
  Grid* grid_;
  GeoValue g_;
  
  void init_geovalue();
};



template<class U, class T>
inline base_grid_iterator<U,T>& base_grid_iterator<U,T>::operator++(int) {
  return asDerived()++;
}

template<class U, class T>
inline base_grid_iterator<U,T> base_grid_iterator<U,T>::operator++(){
  return ++ asDerived();
}


template<class U, class T>
inline bool 
base_grid_iterator<U, T>::operator!=(const base_grid_iterator<U, T>& it) const {
  return index_ != it.index_;
}

template<class U, class T>
inline bool 
base_grid_iterator<U, T>::operator==(const base_grid_iterator<U, T>& it) const {
  return index_ == it.index_;
}


template<class U, class T>
inline int 
base_grid_iterator<U, T>::operator-(const base_grid_iterator<U, T>& it) const {
  return g_.index() - it.g_.index();
}


template<class U, class T>
inline typename base_grid_iterator<U, T>::GeoValue &
base_grid_iterator<U, T>::operator*()  {
  return g_;
}


template<class U, class T>
inline typename base_grid_iterator<U, T>::GeoValue*
base_grid_iterator<U, T>::operator->(){
  return &g_;
}







//======================================================
// Sequential iterator
// 
/** The path defined by a sequential iterator follows the order in which the 
 * grid nodes are numbered.
 */

template<class Grid>
class sequential_grid_iterator : 
public base_grid_iterator<sequential_grid_iterator<Grid>, Grid> {

 public:
  sequential_grid_iterator(Grid* grid, int index, std::string property_name) ;

  inline sequential_grid_iterator& operator++(int);
  inline sequential_grid_iterator operator++();

 protected:
  void init_geovalue();
};



template<class T>
inline sequential_grid_iterator<T>&
sequential_grid_iterator<T>::operator++(int) {
  index_ ++;
  g_.index() ++;
  return *this;
}

template<class T>
inline sequential_grid_iterator<T>
sequential_grid_iterator<T>::operator++() {
  index_++;
  g_.index() ++;
  return *this;
}



//======================================================
// Random path iterator
// 

template<class Grid>
class random_grid_iterator : 
public base_grid_iterator<random_grid_iterator<Grid>, Grid> {

 public:
  random_grid_iterator(Grid* grid, int index, std::string property_name) ;

  inline random_grid_iterator& operator++(int);
  inline random_grid_iterator operator++();

 protected:
  void init_geovalue();
};



template<class T>
inline random_grid_iterator<T>&
random_grid_iterator<T>::operator++(int) {
  index_ ++;
  g_.index() = grid_->random_path_node_index(index_);
  return *this;
}

template<class T>
inline random_grid_iterator<T>
random_grid_iterator<T>::operator++() {
  index_++;
  g_.index() = grid_->random_path_node_index(index_);
  return *this;
}





//======================================================
// Near_Data path iterator
// 

template<class Grid>
class near_data_grid_iterator : 
public base_grid_iterator<near_data_grid_iterator<Grid>, Grid> {

 public:
  near_data_grid_iterator(Grid* grid,
			  int index, std::string property_name) ;

  inline near_data_grid_iterator& operator++(int);
  inline near_data_grid_iterator operator++();

 protected:
  void init_geovalue();
};



template<class T>
inline near_data_grid_iterator<T>&
near_data_grid_iterator<T>::operator++(int) {
  index_ ++;
  g_.index() = grid_->near_data_path_node_index(index_);
  return *this;
}

template<class T>
inline near_data_grid_iterator<T>
near_data_grid_iterator<T>::operator++() {
  index_++;
  g_.index() = grid_->near_data_path_node_index(index_);
  return *this;
}



#if defined(__GNUC__) || defined(WIN32)
#include "grid_iterators.cc"
#elif defined(__xlC__)
#pragma implementation("grid_iterators.cc")
#endif

#endif
