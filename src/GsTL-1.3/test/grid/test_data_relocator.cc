#include <GsTL/grid/property_server.h>
#include <GsTL/grid/cartesian_grid.h>
#include <GsTL/grid/multigrid_view.h>

#include <GsTL/grid/data_relocator.h>

#include <GsTL/geometry/geometry.h>


#include <iostream>
#include <vector>




int main() {

  std::cout << "_______________________________________________" << std::endl
       << "This program performs various tests on " << std::endl
       << "data_relocator" << std::endl << std::endl;

  typedef location_3d<int> grid_location;
  typedef location_3d<float> real_location;


  typedef cartesian_grid<property_server<double> > CartesianGrid;

  // Create a grid, with one property called poro
  CartesianGrid grid(5,5,1);
  grid.new_property("poro");


  // Define a vector of geovalues. They will be the data to relocate in a coarse grid
  typedef CartesianGrid::geovalue_type GeoValue;
  
  std::vector<GeoValue> harddata;

  grid_location loc1(1,0);
  grid_location loc2(1,4);
  grid_location loc3(3,4);
  grid_location loc4(0,1);
  
  
  harddata.push_back( grid(loc1, 0) );
  harddata.push_back( grid(loc2, 0) );
  harddata.push_back( grid(loc3, 0) );
  harddata.push_back( grid(loc4, 0) );
  
  double mult = 1;
  for(std::vector<GeoValue>::iterator it=harddata.begin() ; it!=harddata.end(); ++it)
    it->set_property_value(0.43*(mult++));

  std::cout << "Data to relocate:" << std::endl;
  for(std::vector<GeoValue>::iterator it=harddata.begin() ; it!=harddata.end(); ++it)
    std::cout << it->location() << "  property value: " << it->property_value() << std::endl;

  
  // Declare a coarse grid of "grid" and set the bottom left corner node value to 0
  multigrid_view<CartesianGrid> coarse_grid(&grid, 3); 
  coarse_grid.set_property_value(0,0,0); // inform node (0,0,0) of coarse grid

  typedef multigrid_view<CartesianGrid>::iterator iterator;

  
  std::cout << std::endl << std::endl << "Coarse grid before data relocation" << std::endl;
  for(iterator it=coarse_grid.begin("poro"); it!=coarse_grid.end("poro"); ++it) {
    std::cout << it->location() << "    is informed ?  " << it->is_informed() ;
    if ( it->is_informed() )
      std::cout << "   value= " << it->property_value();
    std::cout << std::endl;
  }

  
  typedef multigrid_view<CartesianGrid> MultiGrid;
  typedef data_relocator<CartesianGrid, MultiGrid> DataRelocator;
  

  // Relocate the data in vector harddata
  DataRelocator relocator(grid, coarse_grid, 0);

  relocator.relocate(harddata.begin(), harddata.end());


  std::cout << std::endl << "Coarse grid after data relocation" << std::endl;

  for(iterator it=coarse_grid.begin("poro"); it!=coarse_grid.end("poro"); ++it) {
    std::cout << it->location() << "    is informed ?  " << it->is_informed() ;
    if ( it->is_informed() )
      std::cout << "   value= " << it->property_value();
    std::cout << std::endl;
  }

  std::cout << relocator.nb_of_relocated_data() << " data were relocated" << std::endl;


  std::cout << std::endl << "Delocating the data" << std::endl;
  relocator.undo_relocate();
  for(iterator it=coarse_grid.begin("poro"); it!=coarse_grid.end("poro"); ++it) {
    std::cout << it->location() << "    is informed ?  " << it->is_informed() ;
    if ( it->is_informed() )
      std::cout << "   value= " << it->property_value();
    std::cout << std::endl;
  }
}

