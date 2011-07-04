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


template<class Grid>
template <class Covariance>
Ellipsoid_neighborhood<Grid>::
Ellipsoid_neighborhood(int xradius, int yradius, int zradius,
			 int max_neighbors,
			 const Covariance& measure,
			 Grid& grid, std::string property_name) 
  : grid_(grid) {

  max_neighbors_ = max_neighbors;
  property_name_id_ = grid.property_name_id(property_name);

  set_geometry(xradius, yradius, zradius, measure);
}


template<class Grid>
Ellipsoid_neighborhood<Grid>::
Ellipsoid_neighborhood(Grid& grid, 
		       const Ellipsoid_neighborhood& rhs) 
  : grid_(grid), geometry_(rhs.geometry_), neighbors_(rhs.neighbors_) {
  
  max_neighbors_ = rhs.max_neighbors_;
  property_name_id_ = grid.property_name_id(property_name);
}



template<class Grid>
template<class Location>
void Ellipsoid_neighborhood<Grid>::find_neighbors(const Location& loc) {
  
  neighbors_.clear();
 
  int count = 0;
  typename std::vector<EuclideanVector>::iterator it = geometry_.begin();
  while (it != geometry_.end() && count < max_neighbors_ ) {
    if( grid_.is_informed(loc + (*it) , property_name_id_) ) {
      neighbors_.push_back( grid_(loc + (*it) , property_name_id_ ) );
      count++;
    }
    ++it;
  }

}
 





template<class Grid>
template<class Covariance>
void 
Ellipsoid_neighborhood<Grid>::set_geometry(int xradius, int yradius, int zradius,
					   const Covariance& measure) {

  /** Look for the neighbors in a rectangular window of HALF dimensions
   * (xradius, yradius, zradius).
   */ 
  for(int x = -xradius ; x<= xradius ;x++)
    for(int y = -yradius ; y<= yradius ;y++)
      for(int z = -zradius ; z<= zradius ;z++) {
        if(x!=0 || y!=0 || z!=0) {
          EuclideanVector vec(x,y,z);
	  geometry_.push_back(vec);
	}
      }
  
  typedef typename Grid::grid_location_type Location;
  Location center(0,0,0);

  // Sort the window vectors according to the distance function "dist"
  std::sort(geometry_.begin(), geometry_.end(), Evector_greater_than(measure, center) );

  // Get rid of the unnecessary nodes, ie the nodes u such that cov(u,center) < thres

  double thres = measure(center,center) / 100.0;

  geom_iterator new_end = geometry_.begin();
  while ( measure(center+ (*new_end), center) > thres  &&
	  new_end != geometry_.end() ) {
    new_end++;
  }

  geometry_.erase( new_end, geometry_.end() );
}






#ifdef __GNUC__
#pragma implementation
#endif
#include "ellipsoid_neighborhood.h"



