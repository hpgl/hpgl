#include <GsTL/grid/property_server.h>
#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/multigrid_view.h>

#include <GsTL/grid/window_neighborhood.h>
#include <GsTL/grid/ellipsoid_neighborhood.h>


#include <GsTL/geometry/geometry.h>
#include <GsTL/geometry/covariance.h>


#include <iostream>
#include <fstream>



int main() {

  std::cout << "_______________________________________________" << std::endl
       << "This program performs various tests on " << std::endl
       << "Neighborhood" << std::endl << std::endl;

  typedef location_3d<int> grid_location;
  typedef location_3d<float> real_location;


  typedef cartesian_grid<property_server<double> > CartesianGrid;

  cartesian_grid<property_server<double> > grid(5,5,1);
  std::cout << "grid size: " << grid.size() << std::endl;

  grid.new_property("poro");

  grid_location loc1(2,2,0);
  grid.set_property_value(0.54, loc1, "poro");

  grid_location loc2(1,4,0);
  grid.set_property_value(0.54, loc2, "poro");
  grid_location loc3(0,3,0);
  grid.set_property_value(0.54, loc3, "poro");

  
  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing window neighborhood" << std::endl << std::endl;
  
  typedef grid_location::difference_type EuclideanVector;
  EuclideanVector vec1(0,1,0);
  EuclideanVector vec2(1,0,0);
  EuclideanVector vec3(0,-1,0);
  EuclideanVector vec4(-1,0,0);
  
  EuclideanVector window[4];
  window[0] = vec1;
  window[1] = vec2;
  window[2] = vec3;
  window[3] = vec4;
 
  
  typedef window_neighborhood<CartesianGrid> wTemplate;
  window_neighborhood<CartesianGrid> neighbors(&grid, "poro", window, window+4);
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;
  std::cout << "current neighborhood is empty? " << neighbors.empty() << std::endl;

  std::cout << "Retrieving neighbors of (1,2,0)" << std::endl;
  neighbors.find_neighbors(grid_location(1,2,0));
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;

  for(wTemplate::iterator it=neighbors.begin(); it!=neighbors.end(); it++)
    std::cout << it->get_location() << std::endl;

  
  std::cout << "Retrieving neighbors of (3,2,0)" << std::endl;
  neighbors.find_neighbors(grid_location(3,3,0));
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;

  for(wTemplate::iterator it=neighbors.begin(); it!=neighbors.end(); it++)
    std::cout << it->get_location() << std::endl;

  
  std::cout << "Retrieving neighbors of (0,4,0)" << std::endl;
  neighbors.find_neighbors(grid_location(0,4,0));
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;

  for(wTemplate::iterator it=neighbors.begin(); it!=neighbors.end(); it++){
    if(grid.contains(it->get_location()))
      std::cout << it->get_location() << std::endl;
    else
      std::cout << "not in grid" << std::endl;
  }




  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing ellipsoid neighborhood" << std::endl << std::endl;
  
  grid.new_property("seis", 0.5);
  Spherical_covariance<grid_location> sph_covariance(10,1);

  typedef Ellipsoid_neighborhood<CartesianGrid> Ellips_neighborhood;
  
  Ellips_neighborhood ellips_neighbors(5,5,0,
				       100, sph_covariance,
				       grid, "seis");

  grid_location center(2,2,0);
  ellips_neighbors.find_neighbors(center);

  std::cout << "neighborhood size: " << ellips_neighbors.size() << std::endl << std::endl;


  // What follows is to get a visual check of the order in which the
  // neighbors are searched.
  double ind=0;
  for(Ellips_neighborhood::iterator neigh_it = ellips_neighbors.begin();
      neigh_it != ellips_neighbors.end(); ++neigh_it) {
    neigh_it->set_property_value(ind++);
  }
  
  std::ofstream OUT("order.out");

  OUT << "grid \n 1 \n order" << std::endl;
  for(CartesianGrid::iterator grid_it=grid.begin("seis"); grid_it!=grid.end("seis"); ++grid_it)
    OUT << grid_it->property_value() << std::endl;
  
}

