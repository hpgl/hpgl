#include <iostream>
#include <vector>
#include <algorithm>


#include "location.h"
#include "geovalue.h"
#include "cartesian_grid.h"
#include "neighborhood.h"
#include "non_param_cdf.h"

#include <GsTL/cdf_estimator/pool_allocator.h>
#include <GsTL/cdf_estimator/standard_allocator.h>
#include <GsTL/cdf_estimator/search_tree.h>

#include <cstdlib>

int main(){

  typedef search_tree<int> SearchTree;

  int init_grid[25] = {0,0,0,0,1,
		       1,1,0,0,1,
		       0,1,1,1,1,
		       0,0,0,0,0,
		       0,0,1,1,1};

  cartesian_grid grid(5,5);
  grid.initialize(init_grid);

  int window_init[8] = {0,1, 1,0, 0,-1, -1,0};  
  neighborhood neighbors(grid,8,window_init); 

  SearchTree tree(grid.begin(), grid.end(),
		  neighbors, 4, 2);

  tree.display(std::cout);
  std::cout << "size of search tree: " << tree.size() << std::endl;

  
  non_param_cdf ccdf(2);

  location_2d centre(0,0);
  geovalue centre_node(&grid,0);
  neighbors.find_neighbors( centre );

  std::cout << " ccdf at location centre:  " ;
  tree(centre_node, neighbors, ccdf);
  for(non_param_cdf::p_iterator p_it=ccdf.p_begin(); p_it!=ccdf.p_end(); p_it++)
    std::cout << *p_it << " ";
  std::cout << std::endl << std::endl;
  


  int niter = 10000;
  std::cout << "repeating the procedure " << niter << " times" << std::endl;
  for(int count =0; count <niter; count++)
    {
      int x= int(drand48()*4);
      int y= int(drand48()*4);
      
      //location_2d unknown(x,y);
      geovalue unknown = grid(x,y);
      neighbors.find_neighbors(unknown);
      tree(unknown, neighbors, ccdf);
    }
  std::cout << "Done" << std::endl;
  
  return 0;
}
