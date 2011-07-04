/* GsTL: the Geostatistics Template Library
 * 
 * Author: Nicolas Remy
 * Copyright (c) 2000 The Board of Trustees of the Leland Stanford Junior University
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *   1.Redistributions of source code must retain the above copyright notice, this 
 *     list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright notice, this 
 *     list of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution. 
 *   3.The name of the author may not be used to endorse or promote products derived 
 *     from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 

#ifndef __GSTL_MATRIX_LIB_TRAITS_H__
#define __GSTL_MATRIX_LIB_TRAITS_H__
#ifdef __GNUC__
#pragma interface
#endif


/** Trait class to allow abstracting from the linear algebra library.
 * The library functionnalities are wrapped into a class (e.g. tnt_lib.h).
 * This class is then used by this traits class. 
 */

#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/matrix_library/tnt_lib.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>

//_____________________________________________
// Base (un-specialized) trait class
template<class lib_name>
struct matrix_lib_traits{

  typedef unsigned int Subscript;
  typedef typename lib_name::matrix Matrix;
  typedef typename lib_name::sym_matrix Symmetric_matrix;
  typedef typename lib_name::vector Vector;

  template<class RandomIterator>
  static inline int cholesky_solve(Matrix& A, Vector& b, RandomIterator solution){
    return 1;
  }

  // A version that does not modify b
  template<class RandomIterator>
  static inline int const_cholesky_solve(Matrix& A, const Vector& b, RandomIterator solution){
    return 1;
  }


  // A general method for solving dense systems
  template<class RandomIterator>
  static inline int solve(Matrix& A, Vector& b, RandomIterator solution){
    return 1;
  }

  // A version that does not modify b
  template<class RandomIterator>
  static inline int const_solve(Matrix& A, const Vector& b, RandomIterator solution){
    return 1;
  }

};




//__________________________________________
// Specialization for TNT

template<class T>
struct matrix_lib_traits< TNT_lib<T> >{
  typedef TNT_lib<T> tnt_;

  typedef typename tnt_::Subscript Subscript;
  typedef typename tnt_::tnt_Matrix Matrix;
  typedef typename tnt_::tnt_Matrix Symmetric_matrix;
  typedef typename tnt_::tnt_Vector Vector;

  template<class RandomIterator>
  static inline int cholesky_solve(
				   Symmetric_matrix& A, Vector& b,
				   RandomIterator solution
				   ) {
    gstl_warning_2( "Cholesky Not yet implemented..." << std::endl 
		  << "... using LU instead" );
    return tnt_::LU_solve(A,b,solution);
    //return tnt_::cholesky(A,B);
  }

  template<class RandomIterator>
  static inline int const_cholesky_solve(
					 Symmetric_matrix& A, const Vector& b,
					 RandomIterator solution
					 ) {
    gstl_warning_2( "const Cholesky Not yet implemented..." << std::endl 
	      << "... using LU instead" );
    return tnt_::const_LU_solve(A,b,solution);
    //return tnt_::cholesky(A,B);
  }

  template<class RandomIterator>
  static inline int solve(
			  Symmetric_matrix& A, Vector& b,
			  RandomIterator solution
			  ) {
    return tnt_::LU_solve(A,b,solution);
  }

  template<class RandomIterator>
  static inline int const_solve(
				Symmetric_matrix& A, const Vector& b,
				RandomIterator solution
				){
    return tnt_::const_LU_solve(A,b,solution);
  }

};


//__________________________________________
// Specialization for GsTL_TNT

template<>
struct matrix_lib_traits<GSTL_TNT_lib>{
  
  typedef GSTL_TNT_lib::Subscript              Subscript;
  typedef GSTL_TNT_lib::gstl_Matrix            Matrix;
  typedef GSTL_TNT_lib::gstl_Symmetric_matrix  Symmetric_matrix;
  typedef GSTL_TNT_lib::gstl_Vector            Vector;

  template<class RandomIterator>
  static inline int cholesky_solve(
				   Symmetric_matrix& A, Vector& b,
				   RandomIterator solution
				   ) {
    gstl_warning_2( "Cholesky Not yet implemented..." << std::endl 
	      << "... using Gauss instead" );
    return GSTL_TNT_lib::gauss_solve(A,b,solution);
  }

  template<class RandomIterator>
  static inline int const_cholesky_solve(
					 Symmetric_matrix& A, const Vector& b,
					 RandomIterator solution
					 ) {
    gstl_warning_2( "const Cholesky Not yet implemented..." << std::endl 
		  << "... using Gauss instead" );
    return GSTL_TNT_lib::const_gauss_solve(A,b,solution);
    //return tnt_::cholesky(A,B);
  }

  template<class RandomIterator>
  static inline int solve(
			  Symmetric_matrix& A, Vector& b,
			  RandomIterator solution
			  ) {
    return GSTL_TNT_lib::gauss_solve(A,b,solution);
  }

  template<class RandomIterator>
  static inline int const_solve(
				Symmetric_matrix& A, const Vector& b,
				RandomIterator solution
				){
    return GSTL_TNT_lib::const_gauss_solve(A,b,solution);
  }

};




#endif
