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

#ifndef ___DATA_RELOCATOR__
#define ___DATA_RELOCATOR__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/grid/multigrid_view.h>


#include <iostream>
#include <map>

/** Call G and G' two grids, and u a location in G.
 * A data_relocator finds the location u' in G' which is the closest to u and
 * set the property value z(u') to z(u). If a property value was already set 
 * at u', nothing is done. However, if a datum v was already relocated at u',
 * and d(u',u) < d(u',v), then z(u') becomes z(u) (while it was formerly z(v) ).
 * The data_relocator keeps track of the locations of G' where a datum was 
 * relocated, and is able to undo the relocation.
 */


template<class CartesianGridType1, class CartesianGridType2>
class data_relocator {

 public:

  data_relocator(CartesianGridType1& from, CartesianGridType2& to, int property_name_id) : from_(from), to_(to){
    std::cerr << "data_relocator constructor not yet implemented" << std::endl;
  }

  /** Relocates the geovalues in range [first,last). 
   */
  template<class ForwardIterator>
  void relocate(ForwardIterator first, ForwardIterator last) {
    std::cerr << "data_relocator::relocate(ForwardIterator, ForwardIterator) not yet implemented" << std::endl;
  }

  void undo_relocate() {
    std::cerr << "data_relocator::undo_relocate() not yet implemented" << std::endl;
  }


 private:

  const CartesianGridType1& from_;
  CartesianGridType2& to_;

  /* This map contains the indeces of nodes of grid to_ where a datum
   * was relocated, along with the index of the datum in grid from_.
   * The map is sorted according to the index in grid to_.
   */
  std::map<int,int> nodes_with_relocated_datum_;

  bool undo_relocate_enabled_;

}; // end of class data_relocator




/** The relocation can be made more efficient if CartesianGridType2 is a 
 * "multigrid_view" of CartesianGridType1. The coordinates of the closest
 * nodes of CartesianGridType2 can be retrieved in constant time from the 
 * coordinates of a node in CartesianGridType1.
 */

template<class CartesianGridType1>
class data_relocator<CartesianGridType1, multigrid_view<CartesianGridType1> > {

 public:
  typedef multigrid_view<CartesianGridType1> MultiGridView;
  
  data_relocator(CartesianGridType1& from, MultiGridView& to,
		 int property_name_id);

  /** Relocates the geovalues in range [first, last).
   * ForwardIterator is an iterator to objects that are models of concept GeoValue
   */
  template<class ForwardIterator>
    void relocate(ForwardIterator first, ForwardIterator last);

  void undo_relocate();

  inline int nb_of_relocated_data() const;



 private:

  const CartesianGridType1& from_;
  MultiGridView to_;
  
  double scale_factor_;
  int property_name_id_;

  /* This map contains the indeces of nodes of grid to_ where a datum
   * was relocated, along with the index of the datum in grid from_.
   * The map is sorted according to the index in grid to_.
   */
  std::map<int,int> nodes_with_relocated_datum_;
  typedef std::map<int,int>::iterator map_iterator;

  typedef typename MultiGridView::grid_location_type Location;

  /** Finds the closest location to loc that belongs to the coarse grid
   * @return the closest location, with coordinates expressed in the FINE grid.
   */
  Location find_closest_location(const Location& loc) const;
  
  bool undo_relocate_enabled_;
  
};// end of specialized version of class data_relocator





template<class T>
inline int data_relocator<T, multigrid_view<T> >::nb_of_relocated_data() const{
  return nodes_with_relocated_datum_.size();
}






#if defined(__GNUC__) || defined(WIN32)
#include "data_relocator.cc"
#elif defined(__xlC__)
#pragma implementation("data_relocator.cc")
#endif

#endif
