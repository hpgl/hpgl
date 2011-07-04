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

#ifndef ___WINDOW_NEIGHBORHOOD__
#define ___WINDOW_NEIGHBORHOOD__
#ifdef __GNUC__
#pragma interface
#endif

#include <string>
#include <vector>
#include <algorithm>

template<class CartesianGrid>
class window_neighborhood {

 public:
  typedef typename CartesianGrid::geovalue_type value_type;
  typedef typename CartesianGrid::property_type property_type;
  typedef typename value_type::location_type location_type;
  typedef typename location_type::difference_type EuclideanVector;

  typedef typename std::vector<value_type>::iterator iterator;
  typedef typename std::vector<value_type>::const_iterator const_iterator;
  
  template<class Iterator>
    window_neighborhood(CartesianGrid* grid,
			std::string property_name,
			Iterator window_begin, Iterator window_end);

  void find_neighbors(const location_type& u);
  void find_neighbors(const value_type& u);
    

  template<class ForwardIterator>
  void set_geometry(ForwardIterator begin, ForwardIterator end);

  inline int size() const;
  inline bool empty() const;
  inline bool is_empty() const { return empty(); }
  
  inline iterator begin();
  inline iterator end();
  inline const_iterator begin() const;
  inline const_iterator end() const;


 private:

  CartesianGrid* grid_;
  int property_name_id_;

  std::vector<EuclideanVector> window_;
  std::vector<value_type> neighbors_;
  
  property_type undef_; // added by nobu ~ undefined value definition
  
}; // end of class window_neighborhood



template<class CartesianGrid>
inline int window_neighborhood<CartesianGrid>::size() const {
  return neighbors_.size();
}

template<class CartesianGrid>
inline bool window_neighborhood<CartesianGrid>::empty() const {
  return neighbors_.empty();
}
  
template<class CartesianGrid>
inline typename window_neighborhood<CartesianGrid>::iterator
window_neighborhood<CartesianGrid>::begin() {
  return neighbors_.begin();
}

template<class CartesianGrid>
inline typename window_neighborhood<CartesianGrid>::iterator 
window_neighborhood<CartesianGrid>::end() {
  return neighbors_.end();
}

template<class CartesianGrid>
inline typename window_neighborhood<CartesianGrid>::const_iterator
window_neighborhood<CartesianGrid>::begin() const {
  return neighbors_.begin();
}

template<class CartesianGrid>
inline typename window_neighborhood<CartesianGrid>::const_iterator 
window_neighborhood<CartesianGrid>::end() const {
  return neighbors_.end();
}

#if defined(__GNUC__) || defined(WIN32)
#include "window_neighborhood.cc"
#elif defined(__xlC__)
#pragma implementation("window_neighborhood.cc")
#endif

#endif
