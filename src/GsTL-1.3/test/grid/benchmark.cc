#include <GsTL/grid/property_server.h>
#include <GsTL/grid/cartesian_geometry.h>
#include <GsTL/grid/grid.h>
#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/multigrid_view.h>

#include <GsTL/geometry/geometry.h>

#include <string>
#include <iostream>


/** This program is a benchmark of the access functions to cartesian_grid and 
 * multiple views of grids.
 */


int main() {
  
  typedef cartesian_grid<property_server<float> > CartesianGrid;
  typedef multigrid_view<CartesianGrid> MultiGridView;

  CartesianGrid grid(1000,1000,8);
  //  std::cout << "grid size: " << grid.size() << std::endl;
  grid.new_property("poro");

  MultiGridView coarse_grid(&grid, 1);
  
  MultiGridView::iterator coarse_it = coarse_grid.begin("poro");

  MultiGridView::iterator end = coarse_grid.end("poro");

  for(; coarse_it != end; coarse_it ++) 
    coarse_it->set_property_value(99.9);
}

