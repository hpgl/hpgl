#ifndef TNT_LIB_H
#define TNT_LIB_H

/** This class encapsulates the functionnalities or the TNT library.
 * Currently, only the linear system solving functionalities are 
 * encapsulated.
 */

#include <GsTL/matrix_library/tnt/tnt.h>
#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/matrix_library/tnt/cholesky.h>
#include <GsTL/matrix_library/tnt/lu.h>
#include <GsTL/matrix_library/tnt/triang.h>
#include <GsTL/matrix_library/tnt/trisolve.h>
#include <GsTL/matrix_library/tnt/tnt_utilities.h>


template<class T>
struct TNT_lib{

  typedef TNT::Subscript Subscript;
  typedef TNT::Matrix<T> tnt_Matrix;
  typedef TNT::Vector<T> tnt_Vector;

  /** Cholesky factorization.
   * @return 0 if successful.
   * @param A is a reference to the original value, and B a reference
   * to the output matrix
   */
  static inline int cholesky(TNT::Matrix<T>& A, TNT::Matrix<T>& B){
    return Cholesky_upper_factorization(A,B);
  }


  /** Solve a system of linear equations using LU decomposition.
   * The algorithm does not use the fact that A is a symmetric matrix.
   * @return 0 if successful
   * @param A is the matrix to be factorised. A will be overwritten by the
   * algorithm. index is the pivot vector (a vector of integers).  It describes 
   * how the rows of A were reordered.
   * @param b is the right hand side of the system. b will be modified by the 
   * algorithm
   */ 
  template<class RandomIterator>
  static inline int LU_solve(
			     TNT::Matrix<T>& A, TNT::Vector<T>& b,
			     RandomIterator solution
			     ) {
    return TNT::GsTL_LU_solve(A,b,solution);
  }

  /** LU factorisation.
   * Same as LU_solve, except that b is not modified
   */ 
  template<class RandomIterator>
  static inline int const_LU_solve(
				   TNT::Matrix<T>& A, const TNT::Vector<T>& b,
				   RandomIterator solution
				   ) {
    TNT::Vector<T> b_copy(b);
    return TNT::GsTL_LU_solve(A, b_copy, solution);
  }


};  // end of class TNT_lib

#endif
