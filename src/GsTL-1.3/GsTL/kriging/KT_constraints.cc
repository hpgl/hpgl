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

#include <GsTL/utils/gstl_error_messages.h>


template<class ForwardIterator>
template <
          class InputIterator,
          class Location,
          class SymmetricMatrix,
          class Vector
         >
unsigned int 
KT_constraints<ForwardIterator>::
operator()(
	   SymmetricMatrix& A,
	   Vector& b,
	   const Location& center,
	   InputIterator first_neigh, InputIterator
	   ) const {

  // Resize the system:
  int neighborhood_size = first_neigh->size();
  int syst_size = mean_size_ + neighborhood_size;
  A.resize(syst_size,syst_size);
  b.resize(syst_size);


  // set up the lagrangian part of the matrix
  // Compute only the upper triangle of the symmetric matrix
  typedef typename std::iterator_traits<InputIterator>::value_type  Neighborhood;
  typedef typename Neighborhood::const_iterator       Neighborhood_iterator;
  Neighborhood_iterator neigh_it = first_neigh->begin();
  for(int row=1; row<=neighborhood_size; row++)
    {
      int col = neighborhood_size + 1;
      ForwardIterator it = mean_first_;
      for(; it != mean_last_; it++)
	{
	  gstl_assert(col<=syst_size);
	  A(row,col++) = (*it)(neigh_it->location());
	}
      neigh_it++;
    }
  

  ForwardIterator it = mean_first_;
  for (int row2=neighborhood_size+1; row2<=syst_size; row2++)
    {
      b(row2)=(*it)(center);
      it++;
      for (int col=row2 ; col<=syst_size; col++)
	A(row2,col)=0;
    }

  return neighborhood_size;


}

#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/kriging/KT_constraints.h>



