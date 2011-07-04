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

template<class Grid>
template<class Iterator>
window_neighborhood<Grid>::window_neighborhood(Grid* grid,
					       std::string property_name,
					       Iterator window_begin,
					       Iterator window_end) {
  grid_= grid;
  property_name_id_ = grid_->property_name_id(property_name);

  for(Iterator it= window_begin; it!= window_end; ++it)
    window_.push_back(*it);

  neighbors_.reserve(window_.size());
}



template<class T>
void window_neighborhood<T>::find_neighbors(const location_type& u) {
  neighbors_.clear();

  typedef typename std::vector<EuclideanVector>::const_iterator window_iterator;
  window_iterator bound = window_.end () - 1;

  while(bound >= window_.begin () && (! grid_->is_informed(u+(*bound), property_name_id_ ) ) 
	 )
  {
	  if (bound == window_.begin())
		  break;
		--bound;
  }

	int gsize = grid_->size();

  for(window_iterator it=window_.begin(); it != bound + 1; ++it) 
  {
	value_type v = (*grid_)( u+(*it), property_name_id_);

	if (gsize > v.index () && v.is_informed() ) // added by nobu ~ undefined value checking
		neighbors_.push_back(v);
  }
}

template<class T>
void window_neighborhood<T>::find_neighbors(const value_type& g) {
	find_neighbors( g.location() );
}

template<class T>
template<class ForwardIterator>
void  window_neighborhood<T>::set_geometry(ForwardIterator begin,
					   ForwardIterator end) {
  window_.assign(begin, end);
  neighbors_.clear();
}

#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/grid/window_neighborhood.h>



