#include <GsTL/kriging/LMC_covariance.h>
#include <GsTL/kriging/MM1_covariance.h>
#include <GsTL/kriging/MM2_covariance.h>

#include <GsTL/geometry/covariance.h>
#include <GsTL/geometry/geometry.h>
#include <GsTL/matrix_library/tnt/cmat.h>

#include <iostream>
#include <cmath>
#include <vector>

/** Compute the covariance between two locations, in 2D and 3D.
 * Use of single-struture covariance functions.
 */




int main(){

  // Some typedefs to make the code easier to read...
  typedef location_3d<int> Location_3d;
  typedef euclidean_vector_3d<int> Vector_3d;
  typedef location_2d<int> Location_2d;
  typedef euclidean_vector_2d<int> Vector_2d;
  typedef Covariance_base<Location_2d> Covariance_2d;
  typedef Covariance_functor<Location_2d> Covariance_handle;

  //=================================
  // Test LMC covariance

  //------------------------
  // make a matrix of POINTERS to covariances  
  const int matrix_size = 2;

  Covariance_2d* sph1 = new Spherical_covariance<Location_2d>(10,1);
  Covariance_2d* sph2 = new Spherical_covariance<Location_2d>(5,1);
  Covariance_2d* exp1 = new Exponential_covariance<Location_2d>(10,1);
  Covariance_2d* exp2 = new Exponential_covariance<Location_2d>(15,1);
  
  TNT::Matrix<Covariance_handle> covariance2d_matrix(matrix_size, matrix_size);
  covariance2d_matrix(1,1) = Covariance_handle(sph1);
  covariance2d_matrix(1,2) = Covariance_handle(sph2);
  covariance2d_matrix(2,1) = Covariance_handle(exp1);
  covariance2d_matrix(2,2) = Covariance_handle(exp2);
  //------------------------

  LMC_covariance< Covariance_handle > lmc_covar(covariance2d_matrix,
							    matrix_size);

  Location_2d l1(2,0);
  Location_2d l2(0,0);
  
  std::cout << "LMC covariance results: " << std::endl;
  std::cout << "(1,1)= " << lmc_covar(1,1,l1,l2) << std::endl
	    << "(1,2)= " << lmc_covar(1,2,l1,l2) << std::endl
	    << "(2,1)= " << lmc_covar(2,1,l1,l2) << std::endl
	    << "(2,2)= " << lmc_covar(2,2,l1,l2) << std::endl;


  std::cout << "Double check: " << std::endl;
  std::cout << "(1,1) should be = " << (*sph1)(l1,l2) << std::endl
	    << "(1,2) should be = " << (*sph2)(l1,l2) << std::endl
	    << "(2,1) should be = " << (*exp1)(l1,l2) << std::endl
	    << "(2,2) should be = " << (*exp2)(l1,l2) << std::endl;


  //=================================
  // Test MM1 covariance
  TNT::Matrix<double> cross_covar_matrix(2,2);
  cross_covar_matrix(1,1)=0.4;
  cross_covar_matrix(2,1)=1;
  cross_covar_matrix(1,2)=2;
  cross_covar_matrix(2,2)=0.2;

  Spherical_covariance<Location_2d> sph_cov(10,1);
  MM1_covariance<Spherical_covariance<Location_2d> > mm1_covar(sph_cov, 
							       cross_covar_matrix, 2);

  std::cout << std::endl
	    << "---------------------------------------------" << std::endl 
	    << "MM1 covariance results: " << std::endl;
  std::cout << "(1,1)= " << mm1_covar(1,1,l1,l2) << std::endl
	    << "(1,2)= " << mm1_covar(1,2,l1,l2) << std::endl
	    << "(2,1)= " << mm1_covar(2,1,l1,l2) << std::endl
	    << "(2,2)= " << mm1_covar(2,2,l1,l1) << std::endl;

  

  //=================================
  // Test MM2 covariance
  
  // create a vector of pointers to covariances
  // C11 is a spherical model and C22 an exponential model
  std::vector<Covariance_handle> cov_vec;
  cov_vec.push_back( Covariance_handle(sph1) );   //sph1 is a pointer to a covariance
  cov_vec.push_back( Covariance_handle(exp1) );
  typedef MM2_covariance< Covariance_handle > MM2_Covariance;

  MM2_Covariance mm2_covar(cov_vec.begin(), cov_vec.end(),
			   cross_covar_matrix, 2);

  std::cout << std::endl
	    << "---------------------------------------------" << std::endl 
	    << "MM2 covariance results: " << std::endl;
  std::cout << "(1,1)= " << mm2_covar(1,1,l1,l2) << std::endl
	    << "(1,2)= " << mm2_covar(1,2,l1,l2) << std::endl
	    << "(2,1)= " << mm2_covar(2,1,l1,l2) << std::endl
	    << "(2,2)= " << mm2_covar(2,2,l1,l1) << std::endl;

}
