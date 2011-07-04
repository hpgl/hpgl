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

 

#ifndef __GSTL_COKRIGING_WEIGHTS_H__
#define __GSTL_COKRIGING_WEIGHTS_H__


#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/utils/debug_tools.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/kriging/helper_functions.h>
#include <GsTL/kriging/kriging_constraints_traits.h>



/** This function computes cokriging weights.
 * @return 0 if the computation was successful, 1 if solving the kriging
 * system failed, and 2 if no conditioning data were provided (i.e. all 
 * the neighborhoods were empty).
 *
 * @param weights is the vector were the cokriging weights will be stored
 * @param kriging_variance is where the kriging variance will be output
 * @param center is the Location being estimated
 * @param [first_neigh,last_neigh) is a range of Neighborhoods. If it is not
 * desirable that a neighborhood be stored directly into a container (e.g. 
 * because it is a too large object), it suffices to create a "handle" object
 * that meets the requirements of Neighborhood and store the handles into the
 * container.  
 * @param covar is a covariance set
 * @param Kconstraints are the kriging constraints. 
 */

template <
          class MatrixLibrary,
          class Location,
          class InputIterator,
          class CovarianceSet,
          class CokrigingConstraints,
          class Vector
         >
int
cokriging_weights(
		  Vector& weights, double& kriging_variance, 
		  const Location& center,
		  InputIterator first_neigh, InputIterator last_neigh,
		  CovarianceSet& covar,
		  CokrigingConstraints& Kconstraints
		  ) {

  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;

  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;
  
  // let the kriging constraint resize the kriging system (A and b)
  int nb_conditioning_data = Kconstraints(A, b,
					  center,
					  first_neigh, last_neigh);

  if(nb_conditioning_data == 0) {
    gstl_warning( "No conditioning data. No kriging to be done" );
    return 2;
  }

  // Build the cokriging matrix (this function is defined at the end of
  // this file
  build_cokriging_matrix(A,b,
			 center,
			 first_neigh, last_neigh,
			 covar );
 
  // Resize the output vector if necessary  
  if( (int)weights.size() != (int)A.num_cols() )
    weights.resize(A.num_cols());
  
  
  // solve the cokriging system
  int status = 
         kriging_constraints_traits<
                                    CokrigingConstraints,
                                    MatrixLibrary
                                   >::const_kriging_solver( A, b, weights.begin() );

  // Compute the kriging variance, unless we were unable to solve
  // the cokriging system. 
  if(status == 0) {
    double C0=covar(1,1, center,center);
    kriging_variance = 
      compute_kriging_variance(weights.begin(),
                               weights.begin()+nb_conditioning_data, weights.end(),
                               b, Kconstraints, center, C0);
  }
  else
    kriging_variance = -99;
  
  return status;

}




/** This function computes cokriging weights. It is essentially the 
 * same function as above except that it does not compute the kriging
 * variance.
 */

template <
          class MatrixLibrary,
          class Location,
          class InputIterator,
          class CovarianceSet,
          class CokrigingConstraints,
          class Vector
         >
int
cokriging_weights(
		  Vector& weights,
		  const Location& center,
		  InputIterator first_neigh, InputIterator last_neigh,
		  CovarianceSet& covar,
		  CokrigingConstraints& Kconstraints
		  ) {

  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;

  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;
  
  // let the kriging constraint resize the kriging system (A and b)
  int nb_conditioning_data = Kconstraints(A, b,
					  center,
					  first_neigh, last_neigh);

  if(nb_conditioning_data == 0) {
    gstl_warning( "No conditioning data. No kriging to be done" );
    return 2;
  }

  // Build the cokriging matrix (this function is defined at the end of
  // this file
  build_cokriging_matrix(A,b,
			 center,
			 first_neigh, last_neigh,
			 covar );
 
  // Resize the output vector if necessary  
  if( int( weights.size() ) != int( A.num_cols() ) )
    weights.resize(A.num_cols());
  
  
  // solve the cokriging system
  int status = 
         kriging_constraints_traits<
                                    CokrigingConstraints,
                                    MatrixLibrary
                                   >::kriging_solver( A, b, weights.begin() );

  return status;

}





//_________________________________________________
// Use GSTL_TNT_lib by default

template <
          class Location,
          class InputIterator,
          class CovarianceSet,
          class CokrigingConstraints,
          class Vector
         >
int
cokriging_weights(
		  Vector& weights, double& kriging_variance, 
		  const Location& center,
		  InputIterator first_neigh, InputIterator last_neigh,
		  CovarianceSet& covar,
		  CokrigingConstraints& Kconstraints
		  ) {

  return cokriging_weights< GSTL_TNT_lib >( weights, kriging_variance,
					    center,
					    first_neigh, last_neigh,
					    covar, Kconstraints );
}



//_________________________________________________
// Use GSTL_TNT_lib by default

template <
          class Location,
          class InputIterator,
          class CovarianceSet,
          class CokrigingConstraints,
          class Vector
         >
int
cokriging_weights(
		  Vector& weights,
		  const Location& center,
		  InputIterator first_neigh, InputIterator last_neigh,
		  CovarianceSet& covar,
		  CokrigingConstraints& Kconstraints
		  ) {

  return cokriging_weights< GSTL_TNT_lib >( weights, 
					    center, 
					    first_neigh, last_neigh,
					    covar, Kconstraints );
}




//=================================================
// The function that computes the kriging system

template <
          class SymmetricMatrix,
          class MatVector,
          class Location,
          class InputIterator,
          class CovarianceSet
         >
void
build_cokriging_matrix(
		       SymmetricMatrix& A, MatVector& b, 
		       const Location& center,
		       InputIterator first_neigh, InputIterator last_neigh,
		       CovarianceSet& covar
		       ) {

  int row_index=1;
  int nb_of_neighborhoods = 
        static_cast<int>( std::distance(first_neigh, last_neigh) );

  InputIterator neighborhood_i = first_neigh;
  for(int row=0; row<nb_of_neighborhoods; row++)
    {
      int col_index = 1;
      InputIterator neighborhood_j = first_neigh;
      for(int col=0; col<nb_of_neighborhoods; col++)
      	{
	        if(row==col)
	          build_Cii( A,
		                   neighborhood_j->begin(), neighborhood_j->end(),
		                   covar,row+1,
		                   row_index );
	        else
	          build_Cij( A, row_index, col_index,
		                   neighborhood_i->begin(), neighborhood_i->end(),
		                   neighborhood_j->begin(), neighborhood_j->end(),
		                   covar,
		                   row+1, col+1 );

      	  col_index += neighborhood_j->size();
	        neighborhood_j++;
	  
	      }

      build_bj( b,
	              center,
	              neighborhood_i->begin(), neighborhood_i->end(),
	              covar,row+1,1,
	              row_index );

      row_index += neighborhood_i->size();
      neighborhood_i++;
    }

  DEBUG_PRINT_KRIGING_SYSTEM( A,b);
  
}



#endif
