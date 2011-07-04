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

 
template <
          class InputIterator,
          class Location,
          class SymmetricMatrix,
          class Vector
         >
unsigned int 
OK_constraints::operator() (
			    SymmetricMatrix& A,
			    Vector& b,
			    const Location&,
			    InputIterator first_neigh, InputIterator last_neigh
			    ) const {

  // in ordinary cokriging we must have some data on the primary
  // variable, otherwise the equation corresponding to the 
  // unbiasedness condition: \sum \lambda_i = 1 (\lambda_i are
  // the weights on the primary variable) can not be verified 
  // (there is no \lambda_i if there is no primary variable)
  if( first_neigh->size() == 0 ) return 0;

  // compute the size of the system
  
  std::vector<int> size_vector; //stores the size of each neighborhood
  int total_nb_of_neighbors=0;
  int nb_of_neighborhoods = 0;  //more precisely: nb of non-empty neighborhoods

  for ( ; first_neigh != last_neigh ; ++first_neigh)
    {
      int neighborhood_size = first_neigh->size();
      total_nb_of_neighbors += neighborhood_size;
      size_vector.push_back(neighborhood_size);
      if( !first_neigh->is_empty() )
        nb_of_neighborhoods ++;
    }

  int syst_size = nb_of_neighborhoods + total_nb_of_neighbors;

  A.resize(syst_size,syst_size);
  b.resize(syst_size);


  std::vector<int>::iterator first_size = size_vector.begin();

  // Set up the OK specific part of the matrix
  // Only the upper-half of the system is computed, the lower half
  // is left unchanged. 
  for(int col=total_nb_of_neighbors+1; col<= syst_size; col++)
    for(int row=1; row <=col; row++)
      {
	if(row > std::accumulate(first_size, first_size+(col-total_nb_of_neighbors)-1, 0) 
	   && row <= std::accumulate(first_size, first_size+(col-total_nb_of_neighbors), 0))
	  A(row,col)=1;
	else
	  A(row,col)=0;
      }


  // Set up the ordinary kriging specific part of the second member
  b(total_nb_of_neighbors+1)=1;
  for(int b_row=total_nb_of_neighbors+2; b_row<=syst_size; b_row++)
    b(b_row)=0;

  return total_nb_of_neighbors;

}




#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/kriging/OK_constraints.h>



