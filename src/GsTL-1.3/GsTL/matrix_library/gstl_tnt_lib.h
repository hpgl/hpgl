#ifndef __GSTL_GSTL_TNT_LIB_H__
#define __GSTL_GSTL_TNT_LIB_H__


#include <GsTL/matrix_library/gstl_tnt/gstl_symetric_matrix.h>
#include <GsTL/matrix_library/gstl_tnt/gstl_gauss_solver.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <GsTL/matrix_library/tnt/tnt.h>
#include <GsTL/matrix_library/tnt/cholesky.h>
#include <GsTL/matrix_library/tnt/tnt_utilities.h>

#include <iostream>


struct GSTL_TNT_lib{

  typedef TNT::Subscript Subscript;
  typedef TNT::Matrix<double> gstl_Matrix;
  typedef GSTL_TNT::Symmetric_matrix<double> gstl_Symmetric_matrix;
  typedef TNT::Vector<double> gstl_Vector;

  /** Cholesky factorization.
   * @return 0 if successful.
   * @param A is a reference to the original value, and B a reference
   * to the output matrix
   */
  static inline int cholesky(gstl_Symmetric_matrix& A, gstl_Symmetric_matrix& B){
    gstl_warning( "GSTL_TNT_lib::cholesky Not yet implemented" );
    return 1;
  }


  /** Solve a system of linear equations using a Gauss algorithm.
   * The algorithm capitilizes on the symmetry of matrix A.
   * @return 0 if successful
   * @param A is the matrix to be factorised. A will be overwritten by the
   * algorithm. index is the pivot vector (a vector of integers).  It describes 
   * how the rows of A were reordered.
   * @param b is the right hand side of the system. b will be modified by the 
   * algorithm
   */ 
  template<class RandomIterator>
  static inline int gauss_solve(
				gstl_Symmetric_matrix& A, 
				gstl_Vector& b,
				RandomIterator solution
				) {
    return GSTL_TNT::gauss_solver(A,b,solution);
  }

  /** LU factorisation.
   * Same as LU_solve, except that b is not modified
   */ 
  template<class RandomIterator>
  static inline int const_gauss_solve(
				      gstl_Symmetric_matrix& A, 
				      const gstl_Vector& b,
				      RandomIterator solution
				      ) {
    gstl_Vector b_copy(b);
    return GSTL_TNT::gauss_solver(A, b_copy, solution);
  }

};  // end of class GsTL_TNT_lib

#endif
