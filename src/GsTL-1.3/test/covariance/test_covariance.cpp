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
  
  
  typedef Covariance_base<Location_2d> Covariance_base_2d;
  typedef Covariance_base<Location_3d> Covariance_base_3d;

  std::cout << std::endl
	    << "______________________________________________"  << std::endl
	    << "o  Covariance in 2D" << std::endl << std::endl;

  Location_2d u1(2,0);
  Location_2d u2(0,0);

  /* Build two isotropic 2D-covariances:
   *  - a spherical model of range 10 and sill 1
   *  - an exponential model of range 10 and sill 1
   */
  Spherical_covariance<Location_2d> sph2d(10,1);
  Exponential_covariance<Location_2d> expo2d(10,1);
  
  std::cout << "spherical: " <<  sph2d(u1,u2) << std::endl
	    << "exponential: " << expo2d(u1,u2) << std::endl; 
  




  std::cout << std::endl << std::endl
	    << "______________________________________________"  << std::endl
	    << "o  Covariance in 3D" << std::endl << std::endl;

  Location_3d U1(5,0,0);
  Location_3d U2(0,0,0);

  /* Build two isotropic 3D-covariances:
   *  - a spherical model of range 10 and sill 1
   *  - an exponential model of range 10 and sill 1
   */

  Spherical_covariance<Location_3d> sph3d(10,1);
  Exponential_covariance<Location_3d> expo3d(10,1);

  std::cout << "spherical: " <<  sph3d(U1,U2) << std::endl
	    << "exponential: " << expo3d(U1,U2) << std::endl << std::endl; 



  
  Covariance_base_3d* covariance_ptr = new Spherical_covariance<Location_3d>(10,1);
  std::cout << "with virtual call: " << covariance_ptr->operator()(U1,U2) << std::endl;



  
  std::cout << std::endl
	    << "_______________________________________________________" <<std::endl
	    << "Anisotropy"
	    << std::endl;
  
  typedef Anisotropic_covariance_2d<Location_2d> Anisotrop_covariance_2d;

  Anisotrop_covariance_2d anisotrop_spherical_2d(sph2d,
						 20.0, 10.0, 3.14/3,  1.0);


  std::cout << "spherical with anisotropy (2D): "<< std::endl
	    << anisotrop_spherical_2d(u1,u2) << std::endl
	    << std::endl;


  
  typedef Anisotropic_covariance_3d<Location_3d> Anisotrop_covariance_3d;

  Anisotrop_covariance_3d  anisotrop_spherical_3d(sph3d,
						  20, 10, 10, 
						  3.14/3, 0, 0,
						  1.0);

  std::cout << "spherical with anisotropy (3D): "<< std::endl
	    << anisotrop_spherical_3d(U1,U2) << std::endl
	    << std::endl;

  


  std::cout << std::endl
	    << "_______________________________________________________" <<std::endl
	    << "Nested Structures"
	    << std::endl;

  typedef Nested_structures_covariance<Location_3d> Nested_covariance_3d;
  std::vector<Covariance_base_3d*> covariances;
  
  Gaussian_covariance<Location_3d> gauss_covar(15,0.2);

  covariances.push_back( new Spherical_covariance<Location_3d>(10, 0.3) );
  covariances.push_back( new Exponential_covariance<Location_3d>(10, 0.3) );
  covariances.push_back( new Anisotropic_covariance_3d<Location_3d>(gauss_covar, 
								    30, 10, 10, 
								    M_PI/4, 0, 0, 
								    0.2) );
  
  Nested_covariance_3d nested_covar(0.2, covariances.begin(), covariances.end());
  
  std::cout << "number of nested structures: " << nested_covar.number_of_structures()
	    << std::endl
	    << "covariance value: " << nested_covar(U1,U2) << std::endl
	    << std::endl;

  std::cout << std::endl << std::endl << "Values for different lags" << std::endl;
  Location_3d O(0,0,0);
  for(int h=0; h<30; h++) {
    Location_3d u(h,0,0);
    std::cout << h << "  " <<  nested_covar(O, u) << ";" << std::endl;
  }

  std::cout << std::endl << "Covariance table: " << std::endl;
  Covariance_table table(30, 1, 1, nested_covar);
  for(int h=0; h<30; h++) {
    Location_3d u(h,0,0);
    std::cout << h << "  " <<  table(O, u) << ";" << std::endl;
  }


  typedef Covariance_functor<Location_3d> Covariance_handle;
  Covariance_handle handle1(&gauss_covar );
  Covariance_handle handle2(handle1);
  Covariance_handle handle3 = handle1;
  Covariance_handle handle4(&nested_covar);

  std::vector<Covariance_handle> cov_vec;
  cov_vec.push_back(handle1);
  cov_vec.push_back(handle2);
  cov_vec.push_back(handle3);
  cov_vec.push_back(handle4);

  for(std::vector<Covariance_handle>::iterator it = cov_vec.begin();
      it != cov_vec.end() ; ++it)
    std::cout << (*it)(U1,U2) << std::endl;
  
  Covariance_handle tab[4];
  std::copy(cov_vec.begin(), cov_vec.end(), tab);
  
  for(int i=0; i<4; i++)
    std::cout << (tab[i])(U1,U2) << std::endl;

  TNT::Matrix<Covariance_handle> A(2,2);
  A(1,1) = handle1;
  A(1,2) = handle2;
  A(2,1) = handle3;
  A(2,2) = handle4;
  
  std::cout << A(1,2)(U1,U2) << std::endl;

 
  for(std::vector<Covariance_base_3d*>::iterator it = covariances.begin() ; 
      it != covariances.end() ; ++it)
    delete *it;


  return 0;
}
