#include <GsTL/grid/property_server.h>
#include <GsTL/grid/cartesian_geometry.h>
#include <GsTL/grid/grid.h>
#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/multigrid_view.h>
#include <GsTL/grid/window_neighborhood.h>

#include <GsTL/geometry/geometry.h>
#include <GsTL/math/random_number_generators.h>

#include <iostream>




int main() {
  
  std::cout << "_______________________________________________" << std::endl
	    << "This program performs various tests on " << std::endl
	    << "property_server and cartesian_geometry" 
	    << std::endl << std::endl;

  std::cout << "_______________________________________________" << std::endl
	    << "   Testing property server: " << std::endl << std::endl;

  property_server<double> pserver(5);
  pserver.new_property("poro", 0.2);
  pserver.new_property("perm", 10);
  pserver.new_property("ntg", 0.5);
  pserver.new_property("tmp", 11);

  std::cout << "properties: "<< std::endl;
  for(int i=0; i< pserver.nb_of_properties(); i++) {
    std::string name = pserver.property_name(i);
    std::cout << name << std::endl;
  }

  std::cout << std::endl << "deleting tmp" << std::endl;
  bool ok = pserver.delete_property("tmp");
  assert(ok);

  std::cout << std::endl << "new properties: "<< std::endl;
  for(int i=0; i< pserver.nb_of_properties(); i++) {
    std::string name = pserver.property_name(i);
    std::cout << name << std::endl;
  }

  std::cout << std::endl;
  std::cout << "poro at 2: " << pserver.property_value(2,"poro") << std::endl;
  std::cout << "perm at 2: " << pserver.property_value(2,"perm") << std::endl;
  std::cout << "ntg at 2: " << pserver.property_value(2,"ntg") << std::endl;
  std::cout << "tmp exists? " << pserver.property_exists("tmp") << std::endl;

  std::cout << std::endl 
       << "changing poro at 2: " << std::endl;
  pserver.property_value(2,"poro") = 55;
  std::cout << "new poro at 2: " << pserver.property_value(2,"poro") << std::endl;
  std::cout << std::endl 
       << "changing again: " << std::endl;
  int poro_id = pserver.property_name_id("poro");
  pserver.property_value(2,poro_id) = 99;
  std::cout << "new poro at 2: " << pserver.property_value(2,"poro") << std::endl;


  property_array<double> parray( pserver.size() );
  for(property_array<double>::iterator it=parray.begin(); it!=parray.end(); ++it)
    *it = 21;

  pserver.new_property("seismic", &parray);
  std::cout << std::endl;
  std::cout << "seismic at 2: " << pserver.property_value(2,"seismic") << std::endl;





  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing cartesian geometry" << std::endl << std::endl;

  typedef location_3d<int> grid_location;
  typedef location_3d<float> real_location;
  
  typedef cartesian_geometry Cartesian_Geometry;

  Cartesian_Geometry geometry_server(5,5,1,
				     3.4, 5.2, 3,
				     1.3, 3.1);
  
  grid_location gloc(1,2,0);
  real_location rloc = geometry_server.convert(gloc);
  std::cout << "Converting (1,2,0) in real space coordinates: " 
       << "(" << rloc[0] << " , " << rloc[1] << " , " << rloc[2] << ")" << std::endl;
 
  geometry_server.resize(6,4,1);

  std::cout << std::endl
       << "index corresponding to (1,2,0) : " << geometry_server.get_index(gloc) 
       << std::endl;

  std::cout << std::endl
       << "index corresponding to "
       << "(" << rloc[0] << " , " << rloc[1] << " , " << rloc[2] << ") : "
       << geometry_server.get_index(rloc) 
       << std::endl;


  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing grid" << std::endl << std::endl;

  typedef grid<property_server<double>, Cartesian_Geometry> Cartesian_Grid;
  
  Cartesian_Grid my_grid(&geometry_server);
  my_grid.new_property("poro", 0.54);

  grid_location gloc1(1,2,0);
  if(my_grid.contains(gloc1))
    std::cout << "value at " << gloc1 << " : " 
	 << my_grid.get_property_value(gloc1, "poro") << std::endl ;
  
  Cartesian_Grid::geovalue_type gval = my_grid(gloc1, "poro");
  std::cout << "value at geovalue gval: " << gval.get_property_value() << std::endl;

  std::cout << std::endl << "Changing value at " << gval.get_location() << " through gval" << std::endl;
  gval.set_property_value(0.98);
  std::cout << "value at geovalue gval: " << gval.get_property_value() << std::endl;
  std::cout << "value at " << gloc1 << " is now : " 
	 << my_grid.get_property_value(gloc1, "poro") << std::endl ;


  std::cout << std::endl << std::endl << "Adding new property seismic: " << std::endl;
  my_grid.new_property("seismic");
  Cartesian_Grid::geovalue_type seismic_gval = my_grid(gloc1, "seismic");
  seismic_gval.set_property_value(1.33);
  std::cout << "Seismic property at " << seismic_gval.get_location() 
       << " : " << seismic_gval.get_property_value() << std::endl;


  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing sequential grid iterator" << std::endl << std::endl;
  
  std::cout << "grid size: " << my_grid.size() << std::endl;
  typedef Cartesian_Grid::iterator iterator;
  iterator it = my_grid.begin("poro");
  for(;it != my_grid.end("poro"); ++it)
      std::cout << "value at " << it->get_location()
	   << " : " << it->get_property_value() << std::endl;



  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing random grid iterator" << std::endl << std::endl;

  typedef Cartesian_Grid::random_path_iterator random_path_iterator;
  
  random_path_iterator r_it = my_grid.random_path_begin("poro");
  for(; r_it != my_grid.random_path_end("poro"); ++r_it)
      std::cout << "value at " << r_it->get_location()
	   << " : " << r_it->get_property_value() << std::endl;
  
  std::cout << std::endl << "Another random path:" << std::endl << std::endl;
  
  Rand48_generator rand_gen(543894);
  my_grid.initialize_random_path(rand_gen); 
  r_it = my_grid.random_path_begin("poro");
  for(; r_it != my_grid.random_path_end("poro"); ++r_it)
      std::cout << "value at " << r_it->get_location()
	   << " : " << r_it->get_property_value() << std::endl;





  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing cartesian grid" << std::endl << std::endl;
  
  cartesian_grid<property_server<double> > cgrid(5,5,1);
  std::cout << "grid size: " << cgrid.size() << std::endl;
  std::cout << "nx= " << cgrid.nx() << "  ny= " << cgrid.ny() << "  nz= " << cgrid.nz() << std::endl;
  cgrid.new_property("poro", 0.54);

  
  if(cgrid.contains(gloc1))
    std::cout << "value at " << gloc1 << " : " 
	 << cgrid.get_property_value(gloc1, "poro") << std::endl ;
  

  
  std::cout << std::endl << std::endl
       << "___________________________________________________________" << std::endl
       << "  Testing multiple grid" << std::endl << std::endl;
  
  typedef cartesian_grid<property_server<double> > CartesianGrid;
  multigrid_view<CartesianGrid> coarse_grid(&cgrid, 2);

  std::cout << "coarse grid size: " << coarse_grid.size() << std::endl;


  multigrid_view<CartesianGrid>::iterator coarse_it = coarse_grid.begin("poro");

  for(; coarse_it != coarse_grid.end("poro"); coarse_it ++) 
    coarse_it->set_property_value(99.9);
  
  
  for(int i=0; i<5; i++) {
    for(int j=0; j<5; j++) {
      grid_location loc(i,j,0);
      std::cout << cgrid.get_property_value(loc, "poro") << "  " ;
    }
    std::cout << std::endl;
  } 



  
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
  window_neighborhood<CartesianGrid> neighbors(&cgrid, "poro", window, window+4);
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;
  std::cout << "current neighborhood is empty? " << neighbors.empty() << std::endl;
  std::cout << "Retrieving neighbors of (1,2,0)" << std::endl;
  neighbors.find_neighbors(grid_location(1,2,0));
  std::cout << "current neighborhood size= " << neighbors.size() << std::endl;

  
  for(wTemplate::iterator n_it=neighbors.begin(); n_it!=neighbors.end(); n_it++)
    std::cout << n_it->get_property_value() << std::endl;

  
}

