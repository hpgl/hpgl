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

#ifndef ___MULTIGRID_VIEW__
#define ___MULTIGRID_VIEW__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/math/random_number_generators.h>

#include <algorithm>


/**
 */

template<class CartesianGrid>
class multigrid_view{

 public:
  
  typedef typename CartesianGrid::property_type property_type;
  typedef typename CartesianGrid::real_location_type real_location_type;
  typedef typename CartesianGrid::grid_location_type grid_location_type;

  typedef geovalue<multigrid_view<CartesianGrid> > geovalue_type;
  
  typedef sequential_grid_iterator<multigrid_view<CartesianGrid> > iterator;
  typedef random_grid_iterator<multigrid_view<CartesianGrid> > random_path_iterator;
  typedef near_data_grid_iterator<multigrid_view<CartesianGrid> > near_data_path_iterator;


  multigrid_view(CartesianGrid* grid, int coarseness_level);

  ~multigrid_view();

  inline grid_location_type coordinates_in_fine_grid(const grid_location_type& loc) const;
  inline int index_in_fine_grid(int index) const;

  inline int size() const;
  inline int coarseness_level() const;

  inline bool contains(int index) const;
  inline bool contains(const grid_location_type& loc) const;
  inline bool contains(const real_location_type& loc) const;
  
  inline grid_location_type grid_location(int index) const ;

  inline int index(const grid_location_type& loc) const;
  
  inline geovalue_type operator()(int index, const std::string& name);
  inline geovalue_type operator()(int index, int property_name_id);
  inline geovalue_type operator()(const grid_location_type& loc, const std::string& name);
  inline geovalue_type operator()(const grid_location_type& loc, int property_name_id);


  inline property_type get_property_value(int index, int property_name_id) const ;
  inline void set_property_value(const property_type& property_value,
				 int index, int property_name_id);

  inline void unset_property_value(int index, int property_name_id);

  inline std::string property_name(int name_id) const;
  inline int property_name_id(std::string name) const;

  inline bool is_informed(int index, int name_id) const;
  inline bool is_informed(const grid_location_type& loc, int name_id) const;

  inline bool has_property(std::string name) const ;
  inline bool has_property(int name_id) const;


  //=======================
  // Paths and iterators
  
  /** for the requirements on a RandomNumberGenerator, see STL concept Random 
   * Number Generator. (e.g. Matthew Austern, Generic Programming and the STL, p.122)
   */
  template<class RandomNumberGenerator>
  void initialize_random_path(RandomNumberGenerator& generator);

  /** Returns the index of the i-th node in the random path
   */
  inline int random_path_node_index(int i_th);

  inline iterator begin(std::string property_name);
  inline iterator end(std::string property_name);

  inline random_path_iterator random_path_begin(std::string property_name);
  inline random_path_iterator random_path_end(std::string property_name);

  template<class WindowGeometry>
  inline near_data_path_iterator near_data_path_begin(std::string property_name,
						      WindowGeometry& window);

 template<class WindowGeometry>
 inline near_data_path_iterator near_data_path_end(std::string property_name,
						      WindowGeometry& window);

 /** for the requirements on a RandomNumberGenerator, see STL concept Random 
  * Number Generator. (e.g. Matthew Austern, Generic Programming and the STL, p.122)
  */
  template<class WindowGeometry, class RandomNumberGenerator>
  void initialize_near_data_path(WindowGeometry& window, 
				 RandomNumberGenerator& generator,
				 int name_id);

  inline int near_data_path_node_index(int i_th);


 private:
  
  cartesian_geometry coarse_geometry_;
  CartesianGrid* grid_;
  int coarseness_level_;
  int scale_factor_;

  int* random_path_;

  std::vector<int> near_data_path_;

}; // end of class multigrid_view






//=======================================================
// Inline functions


template<class T>
inline typename multigrid_view<T>::grid_location_type
multigrid_view<T>::coordinates_in_fine_grid(const grid_location_type& loc) const {
  
  int fine_x = loc[0] * scale_factor_;
  int fine_y = loc[1] * scale_factor_;
  int fine_z = loc[2] * scale_factor_;
  
  return grid_location_type(fine_x, fine_y, fine_z);
}


template<class T>
inline int multigrid_view<T>::index_in_fine_grid(int index) const {
  return grid_->index(coordinates_in_fine_grid(coarse_geometry_.get_grid_location(index)));
}


template<class T>
inline int multigrid_view<T>::size() const {
  return coarse_geometry_.size();
}

template<class T>
inline int multigrid_view<T>::coarseness_level() const {
  return coarseness_level_;
}

template<class T>
inline bool multigrid_view<T>::contains(int index) const {
  return coarse_geometry_.contains_node(index);
}

  
template<class T>
inline bool multigrid_view<T>::contains(const grid_location_type& loc) const {
  return coarse_geometry_.contains_node(loc);
}

template<class T>
inline bool multigrid_view<T>::contains(const real_location_type& loc) const {
  return coarse_geometry_.contains_node(loc);
}
    

template<class T>
inline typename multigrid_view<T>::grid_location_type 
multigrid_view<T>::grid_location(int index) const {
  return coarse_geometry_.get_grid_location(index);
}
  

template<class T>
inline int multigrid_view<T>::index(const grid_location_type& loc) const {
  return coarse_geometry_.get_index(loc);
}
  
   
template<class T> 
inline typename multigrid_view<T>::geovalue_type 
multigrid_view<T>::operator()(int index, const std::string& name) {
  assert( grid_->has_property(name) );
  return geovalue_type(this, grid_->property_name_id(name), index);
}
 

template<class T> 
inline typename multigrid_view<T>::geovalue_type 
multigrid_view<T>::operator()(int index, int name_id) {
  assert( grid_->has_property(name_id) );
  return geovalue_type(this, name_id, index);
}
 

template<class T>
inline typename multigrid_view<T>::geovalue_type 
multigrid_view<T>::operator()(const grid_location_type& loc, const std::string& name) {
  assert( grid_->has_property(name) );

  int index;
  if(contains(loc))
    index = coarse_geometry_.get_index(loc);
  else
    index = size()+1;  //an arbitrary out-of-bound index

  return geovalue_type(this, grid_->property_name_id(name), index(loc) );
}

template<class T>
inline typename multigrid_view<T>::geovalue_type 
multigrid_view<T>::operator()(const grid_location_type& loc, int name_id) {
  assert( grid_->has_property(name_id) );

  int index;
  if(contains(loc))
    index = coarse_geometry_.get_index(loc);
  else
    index = size()+1;  //an arbitrary out-of-bound index

  return geovalue_type(this, name_id, index );
}


template<class T>
inline int multigrid_view<T>::random_path_node_index(int i_th){
  if(i_th >= 0 && i_th < size() )
    return random_path_[i_th];
  else
    return size();
}


template<class T>
inline int multigrid_view<T>::near_data_path_node_index(int i_th){
  if(i_th >= 0 && i_th < size() )
    return near_data_path_[i_th];
  else
    return size();
}


template<class T>
inline typename multigrid_view<T>::iterator 
multigrid_view<T>::begin(std::string property_name) {
  return iterator(this, 0, property_name);
}

template<class T>
inline typename multigrid_view<T>::iterator 
multigrid_view<T>::end(std::string property_name) {
  return iterator(this, size(), property_name);
}


template<class T>
inline typename multigrid_view<T>::random_path_iterator 
multigrid_view<T>::random_path_begin(std::string property_name) {
  if(random_path_ == 0) {
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_random_path(rando);
  }
  return random_path_iterator(this, 0, property_name);
}

template<class T>
inline typename multigrid_view<T>::random_path_iterator 
multigrid_view<T>::random_path_end(std::string property_name) {

  if(random_path_ == 0) {
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_random_path(rando);
  }
  return random_path_iterator(this, size(), property_name);
}



template<class T>
template<class WindowGeometry>
inline typename multigrid_view<T>::near_data_path_iterator 
multigrid_view<T>::near_data_path_begin(std::string property_name,
					WindowGeometry& window) {
  if(static_cast<int>(near_data_path_.size()) != size()) {
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_near_data_path(window, rando, property_name_id(property_name));
  }  
  return near_data_path_iterator(this, 0, property_name);
}

template<class T>
template<class WindowGeometry>
inline typename multigrid_view<T>::near_data_path_iterator 
multigrid_view<T>::near_data_path_end(std::string property_name,
				      WindowGeometry& window) {

  if(static_cast<int>(near_data_path_.size()) != size()){
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_near_data_path(window, rando, property_name_id(property_name));
  }
  return near_data_path_iterator(this, size(), property_name);
}




template<class T>
inline typename multigrid_view<T>::property_type 
multigrid_view<T>::get_property_value(int index, int property_name_id) const {
  assert( is_informed(index, property_name_id) );
  return grid_->get_property_value(index_in_fine_grid(index), property_name_id);
}


template<class T>
inline void 
multigrid_view<T>::set_property_value(const property_type& property_value,
				      int index, int property_name_id) {
  assert(contains(index));
  grid_->set_property_value(property_value,
			    index_in_fine_grid(index), property_name_id);
}


template<class T>
inline void multigrid_view<T>::unset_property_value(int index, int property_name_id) {
  assert(contains(index));
  grid_->unset_property_value(index_in_fine_grid(index), property_name_id);
}


template<class T>
inline std::string multigrid_view<T>::property_name(int name_id) const {
  return grid_->property_name(name_id);
}


template<class T>
inline int multigrid_view<T>::property_name_id(std::string name) const{
  return grid_->property_name_id(name);
}


template<class T>
inline bool multigrid_view<T>::is_informed(int index, int name_id) const {
  if( contains(index) )
    return grid_->is_informed(index_in_fine_grid(index), name_id);
  else
    return false;
}


template<class T>
inline bool multigrid_view<T>::is_informed(const grid_location_type& loc,
					   int name_id) const {
  if( contains(loc) )
    return grid_->is_informed(index_in_fine_grid(this->index(loc)), name_id);
  else
    return false;
}


template<class T>
inline bool multigrid_view<T>::has_property(std::string name) const {
  return grid_->has_property(name);
}


template<class T>
inline bool multigrid_view<T>::has_property(int name_id) const {
return grid_->has_property(name_id);
}


#if defined(__GNUC__) || defined(WIN32)
#include "multigrid_view.cc"
#elif defined(__xlC__)
#pragma implementation("multigrid_view.cc")
#endif

#endif
