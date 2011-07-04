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

#include <GsTL/geometry/geometry_algorithms.h>

#include <cmath>
#include <vector>


template<class CartesianGridType1>
data_relocator<CartesianGridType1,
	       multigrid_view<CartesianGridType1> >::data_relocator(CartesianGridType1& from,
								    MultiGridView& to,
								    int property_name_id) 
		 : from_(from), to_(to) {

  scale_factor_ = std::pow(2,to_.coarseness_level()-1);
  property_name_id_ = property_name_id;

  undo_relocate_enabled_ = false;
}




/* Algorithm "relocate" flowchart:
 * Call z(u) the hard datum to be relocated in the coarse grid
 *  - find the coarse grid location U closest to u
 *  - if U is already informed, two possibilities: a hard datum was already relocated there
 *    or the node is really informed
 *   - if a datum z(u') was previouly relocated in U:
 *    - if d(u,U) < d(u',U), z(U)=z(u), else leave z(U) as is 
 *  - if U is not informed, make z(U)=z(u).
 */
template<class CartesianGridType1>
template<class ForwardIterator>
void data_relocator<CartesianGridType1,
		    multigrid_view<CartesianGridType1> >::relocate(ForwardIterator first,
								   ForwardIterator last) {
  
  ForwardIterator it;
  for(it=first; it!=last; ++it) {
    
    Location closest_loc = find_closest_location(it->location() );

    Location coord_in_coarse_grid;
    int i=0;
    for( ; i<Location::dimension; i++)
      coord_in_coarse_grid[i] = static_cast<int>(closest_loc[i] / scale_factor_);
    
    int index_in_coarse_grid = to_.index(coord_in_coarse_grid);

    // Check if the node where the datum is to be relocated is already informed
    // If yes, is it because a datum has already been relocated at that location ?

    if ( from_.is_informed(closest_loc, property_name_id_ ) ) {
      map_iterator already_it = nodes_with_relocated_datum_.find(index_in_coarse_grid);

      if(already_it != nodes_with_relocated_datum_.end() ) {
	// A datum was already relocated here
	Location datum2_loc = from_.grid_location(already_it->second);

	if(euclidean_distance(closest_loc, it->location() ) < 
	   euclidean_distance(closest_loc, datum2_loc) ) {
	  // The new datum is closer to "closest_loc" than the datum that was previously
	  // relocated there

	  already_it->second = from_.index(it->location() );
	  to_.set_property_value(it->property_value(), index_in_coarse_grid,
				 property_name_id_);
	}
      }
    }
    else {
      // The node is not informed yet
      to_.set_property_value(it->property_value(), index_in_coarse_grid,
				 property_name_id_);
      nodes_with_relocated_datum_[index_in_coarse_grid] = from_.index(it->location());
    }
  }

  undo_relocate_enabled_ = true;
}





template<class T>
void data_relocator<T,multigrid_view<T> >::undo_relocate(){

  if( ! undo_relocate_enabled_ ) return;

  map_iterator it=nodes_with_relocated_datum_.begin();
  for( ; it!=nodes_with_relocated_datum_.end(); ++it) {
    to_.unset_property_value(it->first, property_name_id_);
  }
  undo_relocate_enabled_ = false;
}





template<class T>
data_relocator<T,multigrid_view<T> >::Location
data_relocator<T,multigrid_view<T> >::find_closest_location(const Location& loc) const {
  
  int i1 = loc[0] ;
  int j1 = loc[1] ;
  int k1 = loc[2] ;
  
  int s = static_cast<int>(scale_factor_);

  int i2 = i1 % s;
  int j2 = j1 % s;
  int k2 = k1 % s;
  
  // The eight closest neighbors (8 neighbors, because in 3D) are then,
  // in grid "from_" coordinates:

  
  std::vector<Location> closest_neighbors;

  closest_neighbors.push_back( Location( (i1-i2), (j1-j2), (k1-k2)) );
  
  closest_neighbors.push_back( Location( (i1-i2),   (j1-j2),   (k1+s-k2)) );
  closest_neighbors.push_back( Location( (i1-i2),   (j1+s-j2), (k1-k2)) );
  closest_neighbors.push_back( Location( (i1+s-i2), (j1-j2),   (k1-k2)) );

  closest_neighbors.push_back( Location( (i1+s-i2), (j1+s-j2), (k1-k2)) );
  closest_neighbors.push_back( Location( (i1+s-i2), (j1-j2),   (k1+s-k2)) );
  closest_neighbors.push_back( Location( (i1-i2),   (j1+s-j2), (k1+s-k2)) );
  
  closest_neighbors.push_back( Location( (i1+s-i2), (j1+s-j2), (k1+s-k2)) );

  //search for the single closest to it->location()
  std::vector<Location>::iterator closest_it=closest_neighbors.begin();
  
  std::vector<Location>::iterator candidate_it;
  for(candidate_it=closest_neighbors.begin(); candidate_it!=closest_neighbors.end();
      ++candidate_it) {
    if(euclidean_distance(loc, *candidate_it) < 
       euclidean_distance(loc, *closest_it))
      closest_it = candidate_it;
  }
  return *closest_it;
}
  




#ifdef __GNUC__
#pragma implementation
#endif
#include "data_relocator.h"



