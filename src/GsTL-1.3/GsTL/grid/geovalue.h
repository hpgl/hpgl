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

#ifndef ___GEOVALUE__
#define ___GEOVALUE__
#ifdef __GNUC__
#pragma interface
#endif

#include <iostream>
#include <cassert>
#include <string>


template<class Grid>
class geovalue{
 public:
  typedef typename Grid::property_type property_type;
  typedef typename Grid::grid_location_type location_type;

  
  geovalue(Grid* grid=0, int name_id=0, int index=0);
  geovalue(Grid* grid, const std::string& property_name, int index);
  geovalue(const geovalue& rhs) {
    grid_= rhs.grid_;
    property_name_id_ = rhs.property_name_id_;
    index_ = rhs.index_;
  }
  
  geovalue& operator=(const geovalue& rhs) {
    if(this != &rhs) {
      grid_= rhs.grid_;
      property_name_id_ = rhs.property_name_id_;
      index_ = rhs.index_;
    }
    return *this;
  }

  inline int& index();
  inline const int& index() const;
  
  inline std::string get_property_name() const;
  inline int get_property_name_id() const;
  inline bool set_property_name(const std::string& name);
  inline bool set_property_name(int name_id);
  




  inline bool is_informed() const;
  
  inline property_type get_property_value() const;
  inline property_type property_value() const;
  inline void set_property_value(const property_type& val);

  inline location_type get_location() const;
  inline location_type location() const;
  inline void set_location(const location_type& loc);


 private:
  Grid* grid_;
  int property_name_id_;
  int index_;
 
}; // end of class geovalue.






//=====================================================
// Inline functions

template<class Grid>
inline int& geovalue<Grid>::index() {
  return index_;
}

template<class Grid>
inline const int& geovalue<Grid>::index() const {
   return index_;
}

template<class Grid>
inline std::string geovalue<Grid>::get_property_name() const {
  return grid_->property_name(property_name_id_);
}

template<class Grid>
inline int geovalue<Grid>::get_property_name_id() const {
  return property_name_id_;
}

template<class Grid>
inline bool geovalue<Grid>::set_property_name(const std::string& name) {
  if( grid_->has_property(name) ) {
    property_name_id_ = grid_->property_name_id(name);
    return true;
  }
  return false;
}


template<class Grid>
inline bool geovalue<Grid>::set_property_name(int name_id) {
  if( grid_->has_property(name_id) ) {
    property_name_id_ = name_id;
    return true;
  }
  return false;
}
  


template<class Grid>
inline bool geovalue<Grid>::is_informed() const {
  return grid_->is_informed(index_, property_name_id_);
}
  
template<class Grid>
inline typename geovalue<Grid>::property_type 
geovalue<Grid>::get_property_value() const {
  return grid_->get_property_value(index_, property_name_id_);
}


template<class Grid>
inline typename geovalue<Grid>::property_type 
geovalue<Grid>::property_value() const {
  return grid_->get_property_value(index_, property_name_id_);
}



template<class Grid>
inline void geovalue<Grid>::
set_property_value(const typename geovalue<Grid>::property_type& val) {
  grid_->set_property_value(val, index_, property_name_id_);
}

template<class Grid>
inline typename geovalue<Grid>::location_type 
geovalue<Grid>::get_location() const{
  return grid_->grid_location(index_);
}


template<class Grid>
inline typename geovalue<Grid>::location_type 
geovalue<Grid>::location() const{
  return grid_->grid_location(index_);
}


template<class Grid>
inline void geovalue<Grid>::
set_location(const typename geovalue<Grid>::location_type& loc){
  index_ = grid_->index(loc);
}







#if defined(__GNUC__) || defined(WIN32)
#include "geovalue.cc"
#elif defined(__xlC__)
#pragma implementation("geovalue.cc")
#endif

#endif
