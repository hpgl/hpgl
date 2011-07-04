#include <GsTL/cdf_estimator/scan_image_cdf_estimator.h>

#include <GsTL/cdf/categ_non_param_cdf.h>
#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/property_server.h>
#include <GsTL/grid/window_neighborhood.h>
#include <GsTL/grid/multigrid_view.h>

#include <iostream>



template<class Grid, class Iterator>
void initialize(Grid& grid, std::string new_property_name,
		Iterator first, Iterator last) {
  grid.new_property(new_property_name);

  int index = 0;
  int name_id = grid.property_name_id(new_property_name);
  
  for(Iterator it=first; it!=last; it++)
    grid.set_property_value(*it, index++, name_id);

}




int main() {

  typedef cartesian_grid<property_server<short int> > CartesianGrid;
  typedef CartesianGrid::grid_location_type Location;
  typedef Location::difference_type EuclideanVector;

  // Initialize grid

  int init_grid[25] = {0,0,1,1,1,
                       0,0,0,0,0,
		       0,1,1,1,1,
		       1,1,0,0,1,
                       0,0,0,0,1};

  CartesianGrid grid(5,5);
  initialize(grid, "facies", init_grid, init_grid+25);


  typedef window_neighborhood<CartesianGrid> WindowNeighborhood;

  //Initialize window geometry

  EuclideanVector vec1(0,1,0);
  EuclideanVector vec2(1,0,0);
  EuclideanVector vec3(0,-1,0);
  EuclideanVector vec4(-1,0,0);
  
  EuclideanVector geometry[4];
  geometry[0] = vec1;
  geometry[1] = vec2;
  geometry[2] = vec3;
  geometry[3] = vec4;
 
  //  Neighborhood neighbors(&grid, "facies", geometry, geometry+4);
  WindowNeighborhood window(&grid, "facies", geometry, geometry+4);

  typedef Scan_image_cdf_estimator_t<CartesianGrid::iterator, WindowNeighborhood> Scanner;
  
  Scanner scan_estimator(grid.begin("facies"), grid.end("facies"),
			 window, 2);

  typedef Categ_non_param_cdf<short int> Cdf;

  for(int i=0; i<5; i++)
    for(int j=0; j<5; j++) {
      Location center( i,j);
      CartesianGrid::geovalue_type center_gval( &grid );
      center_gval.set_location( center );
      
      window.find_neighbors(center);
      for(WindowNeighborhood::iterator it=window.begin(); 
	  it!=window.end(); ++it) {
	std::cout << it->location() << "    " ;
	if( it->is_informed() )
	  std::cout << it->property_value();
	else
	  std::cout << "-99";

	std::cout << std::endl;
      }
      Cdf ccdf(2);

      scan_estimator(center_gval, window, ccdf);
      
      std::cout << "cdf at " << center << " : ";
      for(Cdf::p_iterator p_it=ccdf.p_begin(); p_it!=ccdf.p_end(); p_it++)
	std::cout << *p_it << " ";
      std::cout << std::endl << std::endl ; 
    }
}
