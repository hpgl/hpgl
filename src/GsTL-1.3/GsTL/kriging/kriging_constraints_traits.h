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

#ifndef __GSTL__KRIGING_CONSTRAINTS_TRAITS_H__
#define __GSTL__KRIGING_CONSTRAINTS_TRAITS_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/kriging/SK_constraints.h>


/** By default, use gauss-like method for solving the kriging system
 */
template <
          class T,
          class MatrixLibrary
         >
struct kriging_constraints_traits {

  template<class Matrix, class Vector, class RandomIterator>
  static inline int kriging_solver(Matrix& A, Vector& b,
				   RandomIterator solution) {
    return matrix_lib_traits<MatrixLibrary>::solve(A,b,solution);
  }


  template<class Matrix, class Vector, class RandomIterator>
  static inline int const_kriging_solver(Matrix& A, const Vector& b,
					 RandomIterator solution) {
    return matrix_lib_traits<MatrixLibrary>::const_solve(A,b,solution);
  }
};



/** Specialization for simple kriging (SK) constraints
 * With SK, the kriging matrix is positive definite. We capitalize on this
 * and use Cholesky to solve the system.
 */
template <
          class MatrixLibrary
         >
struct kriging_constraints_traits<SK_constraints, MatrixLibrary> {

  template<class Matrix, class Vector, class RandomIterator>
  static inline int kriging_solver(Matrix& A, Vector& b,
				   RandomIterator solution) {
    return matrix_lib_traits<MatrixLibrary>::cholesky_solve(A,b,solution);
  }

  template<class Matrix, class Vector, class RandomIterator>
  static inline int const_kriging_solver(Matrix& A, const Vector& b,
					 RandomIterator solution) {
    return matrix_lib_traits<MatrixLibrary>::const_cholesky_solve(A,b,solution);
  }

};



#endif
