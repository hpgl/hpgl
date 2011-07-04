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

 
#ifndef __GTL_KRIGING_HELPER_FUNCTIONS__
#define __GTL_KRIGING_HELPER_FUNCTIONS__

#ifdef __GNUC__
#pragma interface
#endif



#include <numeric>


/** The functions in this file are helper functions used by the various kriging
 * algorithms of GsTL. These functions are not supposed to be used outside this
 * framework, hence are not fully documented. However, the concepts (template 
 * arguments) used by these functions are almost identical to those used by the
 * kriging algorithms, and the requirements on these concepts should be the same
 * in both cases.
 */





/** A kriging system can be decomposed into two parts: one accounts for 
 * the correlation and redundancy between the variables, while the other
 * part accounts for some linear constraints (e.g. the weights sum-up to 1).
 * The first part is of the form:
 *  [ C11 | C12 ]
 *  [ C21 | C22 ] 
 * (with only 1 secondary variable)
 *
 *This function builds the invariant part C11 of the kriging system. This part 
 * is present in every variant of kriging. It actually only builds the upper
 * half of the matrix (the kriging matrix is symmetric).
 * @param A is a reference to the kriging matrix.
 * @param b is a reference to the second member of the system
 * @param center is the location being estimated
 * @param [first_neigh,last_neigh) is a range of neighbors of "center"
 * @param covar is the covariance function
 */
template <
          class SymmetricMatrix,
          class Vector,
          class InputIterator, 
          class Location,
          class Covariance
         >
int build_invariant(
		    SymmetricMatrix& A,
		    Vector& b,
		    const Location& center,
		    InputIterator first_neigh, InputIterator last_neigh,
		    Covariance& covar
		    ) {

  int i=1;
  int j;
  
  // Only compute the upper triangle of the matrix
  for(InputIterator row = first_neigh; row != last_neigh; row++ ) {
    j=i;
    for(InputIterator col = row ; col != last_neigh ; col++)	  
      A(i,j++)=covar( row->location(), col->location() );
    
    b(i)=covar( row->location(), center );
    
    i++;
  }

  //now i=neighborhood_size+1

  // fill in the lower triangle to make the matrix symetric
  // This part is to be removed once the default matrix library 
  // implements symetric matrices
  /*
  for(int m=1; m<i; m++)
    for(int n=1; n<m; n++)
      A(m,n)=A(n,m);
  */

  return 0; 
};



/** This is an overloaded version of the previous function. It uses a different
* covariance function for \c A and the right-hand side \c b. This is useful for
* block-kriging.
*/
template <
          class SymmetricMatrix,
          class Vector,
          class InputIterator, 
          class Location,
          class Covariance,
          class Covariance2
         >
int build_invariant(
		    SymmetricMatrix& A,
		    Vector& b,
		    const Location& center,
		    InputIterator first_neigh, InputIterator last_neigh,
		    Covariance& covar, Covariance2& covar_rhs
		    ) {

  int i=1;
  int j;
  
  // Only compute the upper triangle of the matrix
  for(InputIterator row = first_neigh; row != last_neigh; row++ ) {
    j=i;
    for(InputIterator col = row ; col != last_neigh ; col++)	  
      A(i,j++)=covar( row->location(), col->location() );
    
    b(i)=covar_rhs( row->location(), center );
    
    i++;
  }

  return 0; 
};



/** This function builds the diagonal blocks Cii of the kriging matrix.
 * @param A is a reference to the kriging matrix.
 * @param start_index is the matrix index at which the block 
 * should be inserted: if start_index=n, the block will begin at (n,n). 
 * start_index != 1 for cokriging.
 */ 
template <
          class SymmetricMatrix,
          class InputIterator,
          class CovarianceSet
         >
int build_Cii(
	      SymmetricMatrix& A,			 
	      InputIterator first_neigh, InputIterator last_neigh,
	      CovarianceSet& covar,
	      int var_i,
	      int start_index =1
	      ) {
  
  int i=start_index;
  int j;
  
  // Only compute the upper triangle of the matrix
  for(InputIterator row = first_neigh ; row != last_neigh ; row++ )
    {
      j=i;
      for(InputIterator col = row ; col != last_neigh ; col++)	  
	A(i,j++) = covar( var_i,var_i,
			  row->location(), col->location() );
      
      i++;
    }
  
  // After this loop, i=start_index + neighborhood_size + 1
  // fill in the lower triangle to make the symetric matrix
  /*
  for(int m=start_index; m<i-1; m++)
    for(int n=start_index; n<m; n++)
      A(m,n)=A(n,m);
  */

  return 0;
}


/** This is an overload function of the repvious.  It does not
 * a covarianceSet, hence not need for var_i
 * This function builds the diagonal blocks Cii of the kriging matrix.
 * @param A is a reference to the kriging matrix.
 * @param start_index is the matrix index at which the block 
 * should be inserted: if start_index=n, the block will begin at (n,n). 
 * start_index != 1 for cokriging.
 */ 
template <
          class SymmetricMatrix,
          class InputIterator,
          class Covariance
         >
int build_Cii(
	      SymmetricMatrix& A,			 
	      InputIterator first_neigh, InputIterator last_neigh,
	      Covariance& covar,
	      int start_index =1
	      ) {
  
  int i=start_index;
  int j;
  
  // Only compute the upper triangle of the matrix
  for(InputIterator row = first_neigh ; row != last_neigh ; row++ )
    {
      j=i;
      for(InputIterator col = row ; col != last_neigh ; col++)	  
	A(i,j++) = covar( row->location(), col->location() );
      
      i++;
    }
  
  // After this loop, i=start_index + neighborhood_size + 1
  // fill in the lower triangle to make the symetric matrix
  /*
  for(int m=start_index; m<i-1; m++)
    for(int n=start_index; n<m; n++)
      A(m,n)=A(n,m);
  */

  return 0;
}



/** A cokriging matrix has the following structure:
 *  [ C11 | C12 ]
 *  [ C21 | C22 ] 
 * (with only 1 secondary variable)
 * 
 * build_Cij builds block Cij, i != j (non-diagonal blocks of covariance
 * between variable i and variable j).
 * @param A is the "total" matrix into which block Cij will
 * be inserted.
 * @param start_row and start_col are the indices at which the block
 * will be inserted.
 * @param first_i is an iterator to the first element of a neighborhood of
 * nodes informed by variable i.
 * last_i is an iterator to 1 past the last element of that neighborhood.
 * @param first_j is an iterator to the first element of a neighborhood of
 * nodes informed by variable j.
 */
template<
         class SymmetricMatrix,
         class InputIterator,
         class Covariance
        >
void build_Cij(
	       SymmetricMatrix& A,
	       int start_row, int start_col,
	       InputIterator first_i, InputIterator last_i,
	       InputIterator first_j, InputIterator last_j,
	       Covariance& covar,
	       int var_i, int var_j
	       ) {

  int i=start_row;
  for(InputIterator row = first_i ; row != last_i ; row++)
    {
      int j=start_col;
      
      for(InputIterator col = first_j ; col != last_j ; col++)
	A(i,j++) = covar(var_i,var_j,
			 row->location(), col->location());
      
      i++;
    }
}



/** Build_C builds a covariance matrix based on the same caovariance funtion
 * @param A is the "total" matrix into which block Cij will
 * be inserted.
 * @param start_row and start_col are the indices at which the block
 * will be inserted.
 * @param first_i is an iterator to the first element of a neighborhood of
 * nodes informed by variable i.
 * last_i is an iterator to 1 past the last element of that neighborhood.
 * @param first_j is an iterator to the first element of a neighborhood of
 * nodes informed by variable j.
 */
template<
         class SymmetricMatrix,
         class InputIterator,
         class Covariance
        >
void build_C(
	       SymmetricMatrix& A,
	       int start_row, int start_col,
	       InputIterator first_i, InputIterator last_i,
	       InputIterator first_j, InputIterator last_j,
	       Covariance& covar
	       ) {

  int i=start_row;
  for(InputIterator row = first_i ; row != last_i ; row++)
    {
      int j=start_col;
      
      for(InputIterator col = first_j ; col != last_j ; col++)
	A(i,j++) = covar( row->location(), col->location());
      
      i++;
    }
}




/** build_bj builds a part of the second member of a cokriging system.
 * @param b is the vector containing the second member.
 * @param start_index is the index at which part bj is inserted into b.
 * @param center is the point to be estimated (center of the neighborhood)
 */
template<
         class Vector,
         class InputIterator,
         class Location,
         class Covariance
        >
void build_bj(
	      Vector& b,
	      const Location& center,
	      InputIterator first_neigh, InputIterator last_neigh,
	      Covariance& covar,
	      int var_i, int var_j,
	      int start_index =1
	      ) {
  
  int j=start_index;
  for( ; first_neigh != last_neigh ; ++first_neigh)
    b(j++)=covar(var_i,var_j,
		 first_neigh->location(), center);
}





/** solves a system of linear equations A.x=b using LU decomposition
 * A is overwritten by its LU decomposition, and b contains the solution
 * of the system.
 */
/*
template<class matrix_lib, class iterator>
int solve_kriging_system(typename matrix_lib_traits<matrix_lib>::Matrix& A,
			 typename matrix_lib_traits<matrix_lib>::Vector& b,
			 iterator solution_begin)
{
  matrix_lib_traits<matrix_lib>::LU_solve(A,b,solution_begin);
  
  return 0;
}
*/

 


/** Computes the kriging variance
 * @param [weights_begin, weights_end) is a range of kriging weights, which
 * includes the Lagrange parameters.
 * @param lagrange_begin is an iterator in [weights_begin, weights_end) that
 * indicates the first Lagrange parameter. 
 * @param b is the second member of the kriging system
 * The requirements on template argument Vector are: 
 *   - vec.begin() is a  valid expressions that 
 *     returns an iterators to the begining of the Vector
 * @param kconstraints are the kriging constraints
 * @param center is the location being krigged.
 * @param C0 is the auto-covariance C(0) 
 */
template<
         class Vector,
         class InputIterator,
         class KrigingConstraints,
         class Location_
        >
double compute_kriging_variance(
				InputIterator weights_begin,
                                InputIterator lagrange_begin, 
                                InputIterator weights_end,
				Vector& b,
                                KrigingConstraints& kconstraints,
                                const Location_& center,
				double C0
				) {
  double contrib = 
    kconstraints.kriging_variance_contrib( center, lagrange_begin, weights_end );
  double inner_product = std::inner_product(weights_begin, lagrange_begin,
                                            b.begin(), contrib );
  return C0 - inner_product;
};

/*
template<
         class Vector,
         class InputIterator
        >
double compute_kriging_variance(
				InputIterator weights_begin,
				Vector& b,
				double C0
				) {

  double inner_product = std::inner_product(b.begin(), b.end(), 
					    weights_begin, 
					    0.0 );
  return C0 - inner_product;
};
*/


#endif
