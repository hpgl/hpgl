#ifndef __GTL_GEOVALUE_EXAMPLE__
#define __GTL_GEOVALUE_EXAMPLE__


#include "location.h"

#include <iostream>
#include <assert.h>



class cartesian_grid;

class geovalue{

 private:

  cartesian_grid* grid_;
  int index_;

 
 public:

  typedef int property_type;
  typedef location_2d location_type;

  geovalue() : grid_(0), index_(0) {};
  geovalue(cartesian_grid* grid):grid_(grid), index_(0) {};
  geovalue(cartesian_grid* grid, int index):grid_(grid), index_(index) {};

 int& index() {return index_;}
 const int& index() const{return index_;}

 bool is_informed() const;

 property_type& property_value();
 const property_type& property_value() const;

 location_type location();
 const location_type location() const;

};


/*
std::ostream& operator<<(std::ostream& os, geovalue& g){
  if(g.is_informed())
    os << g.property_value();
  else
    os << "-99" ;
  return os;
}
*/
#endif
