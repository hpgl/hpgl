#include "location.h"
#include "geovalue.h"
#include "cartesian_grid.h"
#include "neighborhood.h"
#include "non_param_cdf.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <GsTL/cdf_estimator/pool_allocator.h>
#include <GsTL/cdf_estimator/standard_allocator.h>
#include <GsTL/cdf_estimator/search_tree.h>

int main(){

  typedef search_tree<int,standard_allocator<int> >  SearchTree;

  cartesian_grid grid(1000,1000);
  grid.random_generate();  
  
  int window_init[50] = {0,1, 1,0, -1,0, 0,-1, 1,1, 1,-1, -1,1, -1,-1,
			 2,0, 0,2, -2,0, 0,-2, 2,2, 2,-2, -2,2, -2,-2,
			 3,0, 0,3, -3,0, 0,-3, 3,3, 3,-3, -3,3, -3,-3,
			 4,0};

  neighborhood neighbors(grid,50,window_init); 

  SearchTree tree(grid.begin(), grid.end(),
		  neighbors, 25, 2);
  
  std::cout << "size of search tree: " << tree.size() << std::endl;

}
