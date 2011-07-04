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

#ifndef __GSTL_GRID__
#define __GSTL_GRID__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/grid/geovalue.h>
#include <GsTL/grid/flag_server.h>
#include <GsTL/grid/grid_iterators.h>

#include <GsTL/math/random_number_generators.h>


#include <string>

/** The following class implements a grid, i.e. a set of geovalues.
 * It can have different geometries, since it can accept different types 
 * of Geometry Servers. A Geometry Server is an object that knows how the
 * nodes of the grid are organized in space.
 * If the geometry is isomorphic to a cartesian geometry, we can work
 * with two types of coordinates: the "real-world" coordinates, or coordinates
 * corresponding to the indexes in the isomorphic cartesian geometry. 
 * We choose to preferentially work with this latter type of coordinates, 
 * because it is more convenient and is common practice in traditional 
 * geostatistics. 
 *
 * The grid keeps track of a certain number of flags (whether a node is in a region
 * or not, whether it contains a hard datum,...). These flags are represented by 
 * booleans and stored into a property_server<bool>. However, a flag requires only
 * one bit (a bool is 8 bits), and memory could be saved by using bitsets, instead
 * of arrays of bool. A FlagServer is currently defined as a property_server<bool>.
 *
 * Implementation considerations:
 * -------------------------------
 * There are three main types of binary properties that a grid has to keep track of:
 *   - whether, for a given property, a node is informed (i.e. does not contain a 
 *     default, random property value). This can be achieved by storing a flag for
 *     each node and for each proprety, or by assuming that if the property value 
 *     is equal to some default value, the node is not informed.
 *     The first solution is the one retained in this implementation. 
 *     IMPORTANT: In order to gain speed, it is assumed that to a given property name,
 *     its name_id in the property server is the SAME as in the "is-informed" flag server.
 *   - whether, for a given property, a node contains a "hard datum". Since the number
 *     of hard data is generally much lower than the total number of nodes, it was
 *     decided to store all the indexes of the nodes containing a hard datum, instead
 *     of keeping a flag for every single node.
 *   - whether a node belongs to a given region.
 * 
 */


template<class PropertyServer, class GeometryServer>
class grid{

 public:

  typedef PropertyServer property_server_type;
  typedef GeometryServer geometry_server_type;
  
  typedef typename PropertyServer::property_type property_type;
  typedef typename GeometryServer::real_location_type real_location_type;
  typedef typename GeometryServer::grid_location_type grid_location_type;
  
  typedef geovalue< grid<PropertyServer,GeometryServer> > geovalue_type;

  typedef sequential_grid_iterator<grid<PropertyServer,GeometryServer> > iterator;
  typedef random_grid_iterator<grid<PropertyServer,GeometryServer> > random_path_iterator;


  grid();

  /** Constructs a grid with a given geometry. The grid does not own the geometry
   * server, hence is not reponsible for the memory used by the geometry server.
   */
  grid(GeometryServer* gserver);
  
  ~grid();

  void set_geometry(GeometryServer* gserver);

  inline int size() const;

  inline bool new_property(std::string name="default");
  inline bool new_property(std::string name, property_type initial_value);

  inline bool has_property(std::string name) const;
  inline bool has_property(int name_id) const;

  inline bool contains(int index) const;
  inline bool contains(const real_location_type& loc) const;
  inline bool contains(const grid_location_type& loc) const;
  
  inline property_type get_property_value(int index, const std::string& name) const ;
  inline property_type get_property_value(int index, int property_name_id) const ;
  inline property_type get_property_value(const grid_location_type& loc,
					  const std::string& name) const ;
  inline property_type get_property_value(const real_location_type& loc,
					  const std::string& name) const ;
  
  inline void set_property_value(const property_type& property_value,
				 int index, int property_name_id);
  inline void set_property_value(const property_type& property_value,
				 int index, const std::string& name);
  inline void set_property_value(const property_type& property_value,
				 const grid_location_type& loc, const std::string& name);
  inline void set_property_value(const property_type& property_value,
				 const real_location_type& loc, const std::string& name);

  /** Reset the property value at a given location.
   */
  inline void unset_property_value(int index, int property_name_id);
  inline void unset_property_value(geovalue_type& gval);

  inline grid_location_type grid_location(int index) const ;
  inline real_location_type real_location(int index) const ;

  inline int index(const grid_location_type& loc) const;
  inline int index(const real_location_type& loc) const;
  

  /** All these functions return geovalues. The geovalues created by these
   * function are not necessarily valid: the geovalue's location can be 
   * outside the grid. In this case, the geovalue is not informed, and 
   * the location returned by geovalue::get_location() is meaningless, i.e.
   * it is an ARBITRARY location that does not belong to the grid.
   * A call to geovalue::get_property_value() will fail.
   */
  inline geovalue_type operator()(int index, const std::string& property_name);
  inline geovalue_type operator()(int index, int property_name_id);
  inline geovalue_type operator()(const grid_location_type& loc, const std::string& name);
  inline geovalue_type operator()(const grid_location_type& loc, int property_name_id);
  inline geovalue_type operator()(const real_location_type& loc, const std::string& name);
  
  inline bool is_harddata(int index, int name_id) const;
  inline bool is_informed(int index, int name_id) const;
  inline bool is_informed(const grid_location_type& loc, int name_id) const;

  inline std::string property_name(int name_id) const;
  inline int property_name_id(std::string name) const;

  inline geometry_server_type* geometry_server();
  

  //=======================
  // Paths and iterators

  /** Builds a random path visiting all the nodes of the grid.
   * For the requirements on a RandomNumberGenerator, see STL concept Random 
   * Number Generator. (e.g. Matthew Austern, Generic Programming and the STL, p.122)
   * @param generator is a model of the STL concept Random Number Generator.
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



 protected:

  PropertyServer* prop_server_;
  GeometryServer* geom_server_;
  
  flag_server* is_informed_;
  flag_server* regions_;
  
  // Add a vector of sorted vectors containing the indexes of the hard data
  // for each property.  

  int* random_path_; 

}; // end of class grid.








//=============================================
// Inline functions


template<class T, class U>
inline int grid<T,U>::size() const {
  return geom_server_->size();
}


template<class T, class U>
inline bool grid<T,U>::new_property(std::string name) {
  bool ok = is_informed_->new_property(name, false);
  if(ok) 
    return prop_server_->new_property(name);
  else
    return false;
}



template<class T, class U>
inline bool grid<T,U>::new_property(std::string name, property_type initial_value){
  bool ok = is_informed_->new_property(name, true);
  if(ok)
    return prop_server_->new_property(name, initial_value);
  else
    return false;
}



template<class T, class U>
inline bool grid<T,U>::has_property(std::string name) const {
  return prop_server_->property_exists(name);
}

template<class T, class U>
inline bool grid<T,U>::has_property(int name_id) const {
  return prop_server_->property_exists(name_id);
}


template<class T, class U>
inline bool grid<T,U>::contains(int index) const {
  return geom_server_->contains_node(index);
}

template<class T, class U>
inline bool grid<T,U>::contains(const grid_location_type& loc) const {
  return geom_server_->contains_node(loc);
}


template<class T, class U>
inline bool grid<T,U>::contains(const real_location_type& loc) const {
  return geom_server_->contains_node(loc);
}


  


template<class T, class U>
inline typename grid<T,U>::property_type
grid<T,U>::get_property_value(int index, const std::string& name) const {
  
  /* Three things to check:
   *   - the property exists
   *   - index is valid
   *   - the property is informed
   * The last two checks are actually performed by a single call to 
   * is_informed(...) : if the index is not valid, then is_informed()
   * returns false.
   */

  
  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }
  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = name_id
  assert(is_informed_->property_value(index, name_id));

  return prop_server_->property_value(index, name_id);
}


template<class T, class U>
inline typename grid<T,U>::property_type 
grid<T,U>::get_property_value(int index, int property_name_id) const {

  //  assert(contains(index));

  if(!prop_server_->property_exists(property_name_id)) {
    std::cerr << "No property with id " << property_name_id << std::endl;
    abort();
  }

  // This assumes that is_informed_->property_name_id(name) = property_name_id
  assert(is_informed_->property_value(index, property_name_id));

  return prop_server_->property_value(index, property_name_id);
}
  

template<class T, class U>
inline typename grid<T,U>::property_type 
grid<T,U>::get_property_value(const grid_location_type& loc,
			      const std::string& name) const {
  //assert(contains(loc));
  int index = geom_server_->get_index(loc);

  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }

  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = name_id
  assert(is_informed_->property_value(index, name_id));

  return prop_server_->property_value(index, name_id);
}



template<class T, class U>
inline typename grid<T,U>::property_type 
grid<T,U>::get_property_value(const real_location_type& loc,
			      const std::string& name) const {

  //assert(contains(loc));
  int index = geom_server_->get_index(loc);

  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }

  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = name_id
  assert(is_informed_->property_value(index, name_id));

  return prop_server_->property_value(index, name);
}
  



template<class T, class U>
inline void 
grid<T,U>::set_property_value(const property_type& property_value,
			      int index, int property_name_id) {

  assert(contains(index));

  if(!prop_server_->property_exists(property_name_id)) {
    std::cerr << "No property with id " << property_name_id << std::endl;
    abort();
  }
  
  // This assumes that is_informed_->property_name_id(name) = property_name_id
  is_informed_->property_value(index, property_name_id) = true;
  prop_server_->property_value(index, property_name_id) = property_value;
}



template<class T, class U>
inline void 
grid<T,U>::set_property_value(const property_type& property_value,
			      int index, const std::string& name) {
  
  assert(contains(index));

  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }

  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = property_name_id
  is_informed_->property_value(index, name_id) = true;
  prop_server_->property_value(index, name_id) = property_value;
}



template<class T, class U>
inline void 
grid<T,U>::set_property_value(const property_type& property_value,
			      const grid_location_type& loc,
			      const std::string& name) {
  
  assert(contains(loc));
  int index = geom_server_->get_index(loc);
  
  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }

  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = property_name_id
  is_informed_->property_value(index, name_id) = true;
  prop_server_->property_value(index, name_id) = property_value;
}



template<class T, class U>
inline void 
grid<T,U>::set_property_value(const property_type& property_value,
			      const real_location_type& loc,
			      const std::string& name) {
  assert(contains(loc));
  int index = geom_server_->get_index(loc);

  if(!prop_server_->property_exists(name)) {
    std::cerr << "Property " << name << " does not exist" << std::endl;
    abort();
  }

  int name_id = prop_server_->property_name_id(name);

  // This assumes that is_informed_->property_name_id(name) = property_name_id
  is_informed_->property_value(index, name_id) = true;
  prop_server_->property_value(index, name_id) = property_value;
}



template<class T, class U>
inline void grid<T,U>::unset_property_value(int index, int property_name_id) {
  is_informed_->property_value(index, property_name_id) = false;
}

template<class T, class U>
inline void grid<T,U>::unset_property_value(geovalue_type& gval) {
  is_informed_->property_value(gval.index(), gval.get_property_name_id()) = false;
}

template<class T, class U>
inline typename grid<T,U>::grid_location_type 
grid<T,U>::grid_location(int index) const {
  return geom_server_->get_grid_location(index);
}

template<class T, class U>
inline typename grid<T,U>::real_location_type 
grid<T,U>::real_location(int index) const {
  return geom_server_->get_real_location(index);
}

template<class T, class U>
inline int grid<T,U>::index(const grid_location_type& loc) const {
  return geom_server_->get_index(loc);
}

template<class T, class U>
inline int grid<T,U>::index(const real_location_type& loc) const {
  return geom_server_->get_index(loc);
}

template<class T, class U>
inline typename grid<T,U>::geovalue_type 
grid<T,U>::operator()(int index, const std::string& name) {
  assert(prop_server_->property_exists(name));
  return geovalue_type(this, prop_server_->property_name_id(name), index);
}

template<class T, class U>
inline typename grid<T,U>::geovalue_type 
grid<T,U>::operator()(int index, int property_name_id) {
  assert(prop_server_->property_exists(property_name_id));
  return geovalue_type(this, property_name_id, index);
}

template<class T, class U>
inline typename grid<T,U>::geovalue_type 
grid<T,U>::operator()(const grid_location_type& loc, const std::string& name){
  assert(prop_server_->property_exists(name));

  int index;
  if(contains(loc))
    index = geom_server_->get_index(loc);
  else
    index = size()+1;  //an arbitrary out-of-bound index

  return geovalue_type(this, prop_server_->property_name_id(name), index);
}

template<class T, class U>
inline typename grid<T,U>::geovalue_type 
grid<T,U>::operator()(const grid_location_type& loc, int property_name_id){
  assert(prop_server_->property_exists(property_name_id));

  int index;
  if(contains(loc))
    index = geom_server_->get_index(loc);
  else
    index = size()+1;  //an arbitrary out-of-bound index

  return geovalue_type(this, property_name_id, index);
}


template<class T, class U>
inline typename grid<T,U>::geovalue_type 
grid<T,U>::operator()(const real_location_type& loc, const std::string& name){
  assert(prop_server_->property_exists(name));

  int index;
  if(contains(loc))
    index = geom_server_->get_index(loc);
  else
    index = size()+1;  //an arbitrary out-of-bound index

  return geovalue_type(this, prop_server_->property_name_id(name), index);
}
  
template<class T, class U>
inline bool grid<T,U>::is_harddata(int index, int name_id) const{ 
  std::cerr << "is_harddata() : Not yet implemented" << std::endl;
}


template<class T, class U>
inline bool grid<T,U>::is_informed(int index, int name_id) const{
  if(contains(index))
    return is_informed_->property_value(index, name_id);
  else
    return false;
}

template<class T, class U>
inline bool grid<T,U>::is_informed(const grid_location_type& loc, int name_id) const{

  if(contains(loc))
    return is_informed_->property_value(index(loc), name_id);
  else
    return false;
}




template<class T, class U>
inline std::string grid<T,U>::property_name(int name_id) const{
  return prop_server_->property_name(name_id);
}


template<class T, class U>
inline int grid<T,U>::property_name_id(std::string name) const{
  return prop_server_->property_name_id(name);
}




template<class T, class U>
inline typename grid<T,U>::iterator
grid<T,U>::begin(std::string property_name){
  return iterator(this, 0, property_name);
}
 
template<class T, class U>
inline typename grid<T,U>::iterator
grid<T,U>::end(std::string property_name){
  return iterator(this, size() , property_name);
}

template<class T, class U>
inline int grid<T,U>::random_path_node_index(int i_th){
  if(i_th >= 0 && i_th < size() )
    return random_path_[i_th];
  else
    return size();
}


template<class T, class U>
inline typename grid<T,U>::random_path_iterator
grid<T,U>::random_path_begin(std::string property_name){
  if(random_path_ == 0) {
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_random_path(rando);
  }
  return random_path_iterator(this, 0, property_name);
}


template<class T, class U>
inline typename grid<T,U>::random_path_iterator 
grid<T,U>::random_path_end(std::string property_name){
  if(random_path_ == 0) {
    // use a default random number generator
    Rand48_generator rando(21111975);
    initialize_random_path(rando);
  }
  return random_path_iterator(this, size(), property_name);
}


#if defined(__GNUC__) || defined(WIN32)
#include "grid.cc"
#elif defined(__xlC__)
#pragma implementation("grid.cc")
#endif

#endif
