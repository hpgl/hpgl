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

#include <GsTL/math/math_functions.h>

#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>

template<class Grid>
multigrid_view<Grid>::multigrid_view(Grid* grid, int coarseness_level)
  : coarse_geometry_(0,0,0),
    grid_(grid), coarseness_level_(coarseness_level) {

  double reduction_factor = std::pow(2.0, double(coarseness_level-1) );
  scale_factor_ = static_cast<int>(reduction_factor);

  int coarse_nx = GsTL::ceil(static_cast<double>(grid_->nx()) / reduction_factor);
  int coarse_ny = GsTL::ceil(static_cast<double>(grid_->ny()) / reduction_factor);
  int coarse_nz = GsTL::ceil(static_cast<double>(grid_->nz()) / reduction_factor);

  coarse_geometry_.resize(coarse_nx, coarse_ny, coarse_nz);
  
  random_path_ = 0;
}



template<class Grid>
multigrid_view<Grid>::~multigrid_view() {
  if(random_path_ != 0)
    delete [] random_path_;
}



template<class Grid>
template<class RandomNumberGenerator>
void multigrid_view<Grid>::initialize_random_path(RandomNumberGenerator& generator) {
 
  if(random_path_ == 0) {
    random_path_ = new int[size()];
    for(int i=0; i<size(); i++)
      random_path_[i] = i;
  }
  
  STL_generator_adaptor_t<RandomNumberGenerator> stl_generator(generator);
  std::random_shuffle(
		      random_path_, random_path_+size(),
		      stl_generator
		      );

}


template<class Grid>
template<class WindowGeometry, class RandomNumberGenerator>
void multigrid_view<Grid>::initialize_near_data_path(WindowGeometry& window,
						     RandomNumberGenerator& generator,
						     int name_id) {
  if(static_cast<int>(near_data_path_.size()) == size()) return;
 
  near_data_path_.clear();
  near_data_path_.reserve(size());

  typedef std::vector<int>::iterator iterator;
  std::vector<bool> already_visited(size(), false);
  typedef typename WindowGeometry::iterator window_iterator;

  // initialize pseudo_path  
  for(int i=0; i<size(); ++i)
    if(is_informed(i, name_id) ) {
      near_data_path_.push_back(i);
      already_visited[i] = true;
    }

  iterator current_start = near_data_path_.begin();

  while( static_cast<int>(near_data_path_.size()) != size() ) {
    iterator current_end = near_data_path_.end();
    iterator class_it=current_start;
    for( ; class_it!= current_end; ++class_it) {
      for(window_iterator it=window.begin(); it!=window.end(); ++it) {
	grid_location_type loc = grid_location(*class_it) + (*it);
	if( contains(loc) ) {
	  int ind = index(loc);
	  if( already_visited[ind] == false ) {
	    near_data_path_.push_back(ind);
	    already_visited[ind] = true;
	  }
	}
      }
    }
    STL_generator_adaptor_t<RandomNumberGenerator> stl_generator(generator);
    std::random_shuffle(random_path_, random_path_+size(),
			stl_generator );

    current_start = class_it;
  }

}


#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/grid/multigrid_view.h>



