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

 

#ifndef __GSTL_KRIGING_WEIGHTS_H__
#define __GSTL_KRIGING_WEIGHTS_H__
#ifdef __GNUC__
#pragma interface
#endif


/** This class defines a generic kriging Weighting System.
 * It needs a linear algebra library for solving linear systems.
 * The library is up to the user: MatrixLibrary is a template
 * argument (see documentation for requirements on the matrix library).
 * The default matrix library is a slightly modified version of 
 * TNT ( http://math.nist.gov/tnt/ )
 */

#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/utils/debug_tools.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/kriging/kriging_constraints_traits.h>

#include <GsTL/kriging/helper_functions.h>


#include <iostream>


/** Compute the kriging weights.
 * @return 0 if no problem was encountered, 1 if the kriging system could not be
 * solved, and 2 if no neighbor were provided (empty neighborhood).
 * 
 * @param weights is the vector where the kriging weights will be output
 * @param kriging_variance is where the kriging variance will be output
 * @param center is the location being estimated
 * @param neighbors points to the neighborhood of conditioning data used to 
 * estimate center. If there are no neighbors, no kriging is performed and  
 * the return code is 2.
 * @param covar is the covariance function
 * @param Kconstraints are the kriging constraints (eg SK, OK, KT, ...).
 */

template<
         class MatrixLibrary,
         class Location,
         class Neighborhood,
         class Covariance,
         class KrigingConstraints,
         class Vector
        >
int kriging_weights(
		    Vector& weights,
		    double& kriging_variance,
		    const Location& center,
		    const Neighborhood& neighbors,
		    Covariance& covar,
		    KrigingConstraints& Kconstraints
		    ) {

  typedef matrix_lib_traits< MatrixLibrary > MatrixLib;
  
  // If the neighborhood is empty, there is no kriging to be done.
  if( neighbors.is_empty() ) {
    gstl_warning( "Empty neighborhood. No kriging to be done. " );
    return 2;
  }
  
  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;
  
  int conditioning_data = 
    build_kriging_system(A,b,
		         weights, 
		         center, neighbors,
		         covar, Kconstraints);

  // solve the system
  int status = 
         kriging_constraints_traits<
                                    KrigingConstraints,
                                    MatrixLibrary
                                   >::const_kriging_solver(A, b, weights.begin());

  // Compute the kriging variance
  if(status == 0) {
    double C0=covar(center,center);
    kriging_variance = 
      compute_kriging_variance(weights.begin(), 
                               weights.begin()+conditioning_data, weights.end(),
                               b, Kconstraints, center, C0);
  }
  else
    kriging_variance = -99;

  return status;
}



/** This overloaded function does not compute the kriging variance.
 * There are cases where the kriging variance is of no use (eg indicator
 * kriging). This function saves some time by not computing the kriging
 * variance.
 */ 
template<
         class MatrixLibrary,
         class Location,
         class Neighborhood,
         class Covariance,
         class KrigingConstraints,
         class Vector
        >
int kriging_weights(
		    Vector& weights,
		    const Location& center,
		    const Neighborhood& neighbors,
		    Covariance& covar,
		    KrigingConstraints& Kconstraints
		    ) {

  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  
  // If the neighborhood is empty, there is no kriging to be done.
  if( neighbors.is_empty() ) {
    gstl_warning( "Empty neighborhood. No kriging to be done. " );
    return 2;
  }


  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;

  build_kriging_system(A,b,
		       weights, 
		       center, neighbors,
		       covar, Kconstraints);

  // solve the system
  int status = 
       kriging_constraints_traits<
                           KrigingConstraints,
                           MatrixLibrary
                                 >::kriging_solver(A, b, weights.begin());
  return status;
}




/** This overloaded function uses the default matrix library: TNT.
 */ 
template<
         class Location,
         class Neighborhood,
         class Covariance,
         class KrigingConstraints,
         class Vector
        >
inline int
kriging_weights(
		Vector& weights,
		double& kriging_variance,
		const Location& center,
		const Neighborhood& neighbors,
		Covariance& covar,
		KrigingConstraints& Kconstraints
		) {

  return kriging_weights< GSTL_TNT_lib >( weights, kriging_variance,
					  center,neighbors,
					  covar,Kconstraints );
}



/** This overloaded function uses the default matrix library: TNT.
 */ 
template<
         class Location,
         class Neighborhood,
         class Covariance,
         class KrigingConstraints,
         class Vector
        >
inline int
kriging_weights(
		Vector& weights,
		const Location& center,
		const Neighborhood& neighbors,
		Covariance& covar,
		KrigingConstraints& Kconstraints
		) {

  return kriging_weights< GSTL_TNT_lib >( weights,
					  center,neighbors,
					  covar,Kconstraints );
}



//===========================================


/** Compute the kriging weights, using a different covariance function for
 * the kriging matrix and the right-hand side of the kriging system. This
 * is useful for block kriging for example.
 * @return 0 if no problem was encountered, 1 if the kriging system could not be
 * solved, and 2 if no neighbors were provided (empty neighborhood).
 * 
 * @param weights is the vector where the kriging weights will be output
 * @param kriging_variance is where the kriging variance will be output
 * @param center is the location being estimated
 * @param neighbors points to the neighborhood of conditioning data used to 
 * estimate \c center. If there are no neighbors, no kriging is performed and  
 * the return code is 2.
 * @param covar is the covariance function used for the kriging matrix
 * @param covar_rhs is the covariance function used to compute the right-hand
 * side of the kriging system, ie the covariance between the data and the 
 * unknown.
 * @param Kconstraints are the kriging constraints (eg SK, OK, KT, ...).
 */

template<
         class MatrixLibrary,
         class Location,
         class Neighborhood,
         class Covariance,
         class Covariance2,
         class KrigingConstraints,
         class Vector
        >
int kriging_weights_2(
		    Vector& weights,
		    double& kriging_variance,
		    const Location& center,
		    const Neighborhood& neighbors,
		    Covariance& covar, Covariance2& covar_rhs,
		    KrigingConstraints& Kconstraints
		    ) {

  typedef matrix_lib_traits< MatrixLibrary > MatrixLib;
  
  // If the neighborhood is empty, there is no kriging to be done.
  if( neighbors.is_empty() ) {
    gstl_warning( "Empty neighborhood. No kriging to be done. " );
    return 2;
  }
  
  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;
  
  int conditioning_data = 
    build_kriging_system(A,b,
     		         weights, 
		         center, neighbors,
		         covar, covar_rhs, Kconstraints);

  // solve the system
  int status = 
         kriging_constraints_traits<
                                    KrigingConstraints,
                                    MatrixLibrary
                                   >::const_kriging_solver(A, b, weights.begin());

  // Compute the kriging variance
  if(status == 0) {
    double C0=covar(center,center);
    kriging_variance =
      compute_kriging_variance(weights.begin(), 
                               weights.begin()+conditioning_data, weights.end(),
                               b, Kconstraints, center, C0);
  }
  else
    kriging_variance = -99;

  return status;
}



/** This overloaded function does not compute the kriging variance.
 * There are cases where the kriging variance is of no use (eg indicator
 * kriging). This function saves some time by not computing the kriging
 * variance.
 */ 
template<
         class MatrixLibrary,
         class Location,
         class Neighborhood,
         class Covariance,
         class Covariance2,
         class KrigingConstraints,
         class Vector
        >
int kriging_weights_2(
		    Vector& weights,
		    const Location& center,
		    const Neighborhood& neighbors,
		    Covariance& covar, Covariance2& covar_rhs,
		    KrigingConstraints& Kconstraints
		    ) {

  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  
  // If the neighborhood is empty, there is no kriging to be done.
  if( neighbors.is_empty() ) {
    gstl_warning( "Empty neighborhood. No kriging to be done. " );
    return 2;
  }


  typename MatrixLib::Symmetric_matrix A;
  typename MatrixLib::Vector b;

  build_kriging_system(A,b,
		       weights, 
		       center, neighbors,
		       covar, covar_rhs, Kconstraints);

  // solve the system
  int status = 
       kriging_constraints_traits<
                           KrigingConstraints,
                           MatrixLibrary
                                 >::kriging_solver(A, b, weights.begin());
  return status;
}




/** This overloaded function uses the default matrix library: TNT.
 */ 
template<
         class Location,
         class Neighborhood,
         class Covariance,
         class Covariance2,
         class KrigingConstraints,
         class Vector
        >
inline int
kriging_weights_2(
		Vector& weights,
		double& kriging_variance,
		const Location& center,
		const Neighborhood& neighbors,
		Covariance& covar, Covariance2& covar_rhs,
		KrigingConstraints& Kconstraints
		) {

  return kriging_weights_2< GSTL_TNT_lib >( weights, kriging_variance,
					  center,neighbors,
					  covar, covar_rhs, Kconstraints );
}



/** This overloaded function uses the default matrix library: TNT.
 */ 
template<
         class Location,
         class Neighborhood,
         class Covariance,
         class Covariance2,
         class KrigingConstraints,
         class Vector
        >
inline int
kriging_weights_2(
		Vector& weights,
		const Location& center,
		const Neighborhood& neighbors,
		Covariance& covar, Covariance2& covar_rhs,
		KrigingConstraints& Kconstraints
		) {

  return kriging_weights_2< GSTL_TNT_lib >( weights,
					  center,neighbors,
					  covar, covar_rhs, Kconstraints );
}





//===========================================
// The function that does the job
template<
         class SymmetricMatrix,
         class MatVector,
         class Location,
         class Neighborhood,
         class Covariance,
         class KrigingConstraints,
         class Vector
        >
int build_kriging_system(
			  SymmetricMatrix& A, 
			  MatVector& b,
			  Vector& weights,
			  const Location& center,
			  const Neighborhood& neighbors,
			  Covariance& covar,
			  KrigingConstraints& Kconstraints
			  ) {

  int nb_conditioning_data = Kconstraints(A, b,
	                                  center, neighbors);
  
  build_invariant(A,b,
		  center,
		  neighbors.begin(), neighbors.end(),
		  covar);
  
  DEBUG_PRINT_KRIGING_SYSTEM( A,b);

  // Resize the output vector if necessary
  if( static_cast<int>(weights.size()) != static_cast<int>(A.num_cols()) )
    weights.resize(A.num_cols());

  return nb_conditioning_data;
}



template<
         class SymmetricMatrix,
         class MatVector,
         class Location,
         class Neighborhood,
         class Covariance,
         class Covariance2,
         class KrigingConstraints,
         class Vector
        >
int build_kriging_system( 
			  SymmetricMatrix& A, 
			  MatVector& b,
			  Vector& weights,
			  const Location& center,
			  const Neighborhood& neighbors,
			  Covariance& covar, Covariance2& covar_rhs,
			  KrigingConstraints& Kconstraints
			  ) {

  int nb_conditioning_data = Kconstraints(A, b,
	                                   center, neighbors);
  
  build_invariant(A, b,
		              center,
		              neighbors.begin(), neighbors.end(),
		              covar, covar_rhs);
  
  DEBUG_PRINT_KRIGING_SYSTEM( A,b);

  // Resize the output vector if necessary
  if( static_cast<int>(weights.size()) != static_cast<int>(A.num_cols()) )
    weights.resize(A.num_cols());

  return nb_conditioning_data;
}



#endif
