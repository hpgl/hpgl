#include "geovalue.h"
#include "cartesian_grid.h"

geovalue cartesian_grid::operator()(int i, int j) {    
  int index;
  if( i<0 || i>=nrow_ || j<0 || j>=ncol_ )
    index=values_.size()+99;
  else
    index=hash(i,j);

  geovalue g(this,index);
  return g;
}

geovalue cartesian_grid::operator()(int index){
  if(!(index>=0 && index < values_.size())) index =values_.size()+99;
  geovalue g(this, index);
  return g;
}

geovalue cartesian_grid::iterator::operator*(){
  return grid_(index_);
}

geovalue* cartesian_grid::iterator::operator->(){
  geovalue* g_ptr = new geovalue(&grid_, index_); 
  return g_ptr;
} 

