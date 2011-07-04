#include <GsTL/geometry/covariance.h>
#include <GsTL/geometry/covariance_table.h>
#include <GsTL/geometry/geometry.h>

#include <GsTL/matrix_library/tnt/cmat.h>


#include <iostream>
#include <cmath>
#include <vector>

/** Compute the covariance between two locations, in 2D and 3D.
 */

int main(){
  
  // Some typedefs to make the code easier to read...
  typedef location_3d<int> Location_3d;
  typedef euclidean_vector_3d<int> Vector_3d;
  typedef location_2d<int> Location_2d;
  typedef euclidean_vector_2d<int> Vector_2d;
  
  Location_3d P1(0,0,0);
  Location_3d P2(1,1,0);
  Location_3d P3(2,2,0);
  Location_3d P4(3,3,0);
  Location_3d P5(4,4,0);
  Location_3d P6(5,5,0);
  Location_3d P7(6,6,0);
  Location_3d P8(7,7,0);
  Location_3d P9(8,8,0);
  
  std::cout << std::endl << std::endl 
	    << "________________________________________" << std::endl
	    << "   Test general-purpose Covariance class " << std::endl;
  Covariance< Location_3d > covar;
  covar.nugget( 0.3 );
  
  std::vector< std::string > types = covar.available_types();
  std::cout << "Available types: " << std::endl;
  for( int i=0; i<types.size(); i++ )
    std::cout << types[i] << " ";
  std::cout << std::endl << std::endl;

  std::cout << "cov(P1,P2)=" << covar(P1,P2) << std::endl;

  int id = covar.add_structure( "Spherical" );
  std::cout << "covariance id: " << id << std::endl;
  covar.sill( 0, 0.7 );
  covar.set_ranges( 0, 7, 5, 10 );
  std::cout << covar(P1,P1) << " " 
	    << covar(P1,P2) << " " 
	    << covar(P1,P3) << " " 
	    << covar(P1,P4) << " " 
	    << covar(P1,P5) << " " 
	    << covar(P1,P6) << " " 
	    << covar(P1,P7) << " " 
	    << covar(P1,P8) << " " 
	    << covar(P1,P9) << " " 
	    << std::endl;


  return 0;

}
