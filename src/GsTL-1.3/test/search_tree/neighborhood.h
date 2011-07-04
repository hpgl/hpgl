#ifndef __GTL_NEIGHBORHOOD_WINDOW_EXAMPLE__
#define __GTL_NEIGHBORHOOD_WINDOW_EXAMPLE__
 

#include "cartesian_grid.h"
#include "geovalue.h"
#include "location.h"

#include <vector>



class neighborhood{

 private: 
  cartesian_grid& grid_;
  std::vector<geovalue> neigh_;
  std::vector<int> window_;   //relative coordinates of window


 public:

  typedef std::vector<geovalue>::iterator iterator;
  typedef std::vector<geovalue>::const_iterator const_iterator;
  typedef geovalue value_type;

  neighborhood(cartesian_grid& grid, int window_size, int* window_coord) : grid_(grid), 
    neigh_(int(window_size/2)),
    window_(window_coord, window_coord + window_size) {};

  neighborhood(cartesian_grid& grid, iterator first, iterator last):grid_(grid) {
    std::copy(first,last,neigh_.begin());
  }

  inline iterator begin() {return neigh_.begin();}
  inline iterator end() {return neigh_.end();}
  inline const_iterator begin() const {return neigh_.begin();}
  inline const_iterator end() const {return neigh_.end();}
  inline unsigned int size() {return neigh_.size();}
  inline bool empty() {return neigh_.empty();}

  void find_neighbors( const geovalue& g ) {
    location_2d u = g.location();
    find_neighbors( u );
  }

  void find_neighbors( const location_2d& u ) {
    neigh_.resize(int(window_.size()/2));               // very important !
    for(unsigned int i=0; i<neigh_.size(); i++)
      neigh_[i]=grid_(u[0]-window_[2*i+1], u[1]+window_[2*i]);
    
    // remove the trailing unestimated neighbors
    std::vector<geovalue>::iterator r_it = neigh_.end();
    r_it--;
    while( !(r_it->is_informed())
	   && r_it != neigh_.begin() ) {--r_it ;}
    // erase resizes neigh_. Hence the resize at the beginning
    // to re-initialize neigh_.
    neigh_.erase(++r_it, neigh_.end());     
  }

  void push_back(geovalue& g){neigh_.push_back(g);}
  void clear(){neigh_.clear();}
};

#endif
