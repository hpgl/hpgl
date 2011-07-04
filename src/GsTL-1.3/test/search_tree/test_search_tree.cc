#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/geovalue.h>
#include <GsTL/grid/property_server.h>
#include <GsTL/grid/window_neighborhood.h>
#include <GsTL/grid/multigrid_view.h>

#include <GsTL/cdf_estimator/search_tree.h>
#include <GsTL/cdf/categ_non_param_cdf.h>


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>


template<class Grid, class Iterator>
void initialize(Grid& grid, std::string new_property_name,
		Iterator first, Iterator last) {
  grid.new_property(new_property_name);

  int index = 0;
  int name_id = grid.property_name_id(new_property_name);
  
  for(Iterator it=first; it!=last; it++)
    grid.set_property_value(*it, index++, name_id);

}




int main(){
  
  typedef cartesian_grid<property_server<short int> > CartesianGrid;
  typedef CartesianGrid::geovalue_type geovalue_type;
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
  
  EuclideanVector window[4];
  window[0] = vec1;
  window[1] = vec2;
  window[2] = vec3;
  window[3] = vec4;
 
  WindowNeighborhood neighbors(&grid, "facies", window, window+4);


  // Build search tree and display it.

  typedef search_tree<> SearchTree;
  
  const int max_neighborhood_size = 4;
  const int number_of_facies = 2;
  SearchTree tree(grid.begin("facies"), grid.end("facies"),
		  neighbors, 
		  max_neighborhood_size, number_of_facies);

  std::cout << "search tree built from grid1" << std::endl 
	    << std::endl;
  tree.display(std::cout);
  std::cout << std::endl;
  
  // Read a cdf from the search tree
  
  typedef Categ_non_param_cdf<short int> Cdf;
  Cdf ccdf(2);

  Location center(2,3);
  geovalue_type center_node = grid(center,0);
  neighbors.find_neighbors(center);

  tree(center_node, neighbors, ccdf);

  std::cout << "cdf at " << center << " : ";
  for(Cdf::p_iterator p_it=ccdf.p_begin(); p_it!=ccdf.p_end(); p_it++)
    std::cout << *p_it << " ";
  std::cout << std::endl; 


  std::cout << std::endl << std::endl << std::endl
       << "______________________________________________________________" << std::endl
       << "  Testing search tree and multiple-grids." << std::endl << std::endl;

  // This grid is such that the coarse grid of coarsness 2 is the same
  // as the the previous grid. Hence the search tree obtained from 
  // "coarse_grid" should be the same as the one built from "grid"

  int init_big_grid[81] = {0,0,0,0,1,0,1,0,1,
			   0,0,0,0,0,0,0,0,0,
			   0,0,0,0,0,0,0,0,0,
			   0,0,0,0,0,0,0,0,0,
			   0,0,1,0,1,0,1,0,1,
			   0,0,0,0,0,0,0,0,0,
			   1,0,1,0,0,0,0,0,1,
			   0,0,0,0,0,0,0,0,0,
			   0,0,0,0,0,0,0,0,1};
  
  // Define a new fine grid
  CartesianGrid big_grid(9,9);
  initialize(big_grid, "facies", init_big_grid, init_big_grid+81);
  
  // Define coarse grid from "big_grid"
  multigrid_view<CartesianGrid> coarse_grid(&big_grid, 2);

  typedef window_neighborhood<multigrid_view<CartesianGrid> > NewWindowNeighborhood;
  NewWindowNeighborhood neighbors_big(&coarse_grid, "facies", window, window+4);

  SearchTree coarse_tree(coarse_grid.begin("facies"), coarse_grid.end("facies"),
			 neighbors_big, 
			 max_neighborhood_size, number_of_facies);

  std::cout << "search tree built from coarse grid of grid2" << std::endl 
	    << "since coarse grid of grid2 is equal to grid1, the search " << std::endl
	    << "tree should be the same as the previous one" << std::endl 
	    << std::endl;
  coarse_tree.display(std::cout);
}
