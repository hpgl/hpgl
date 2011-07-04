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


template<class Iterator, class Neighborhood>
Scan_image_cdf_estimator_t<Iterator, Neighborhood>::
Scan_image_cdf_estimator_t(Iterator begin, Iterator end,
			   Neighborhood& window,
			   int nb_of_categories) 
  : begin_(begin), end_(end), window_(window),
    nb_of_categories_(nb_of_categories) {}



template<class I, class W>
template<class Geovalue_, class NonParamCdf>
int Scan_image_cdf_estimator_t<I,W>::operator()(const Geovalue_& u,
						const W& neighbors,
						NonParamCdf& ccdf) {

  if(neighbors.is_empty())
    return 1;

  initialize_tree(neighbors.size());
  build_degenerated_tree( u.location(), neighbors );
  get_cdf(ccdf);

  return 0;
}






template<class Iterator, class Neighborhood>
void 
Scan_image_cdf_estimator_t<Iterator,Neighborhood>::
initialize_tree(int size) {
  degenerated_tree_.clear();
  for(int i=0; i<size; i++)
    degenerated_tree_.push_back(std::vector<double>(nb_of_categories_, 0.0));
}






template<class Iterator, class Neighborhood>
template<class Location>
bool 
Scan_image_cdf_estimator_t<Iterator,Neighborhood>::
build_degenerated_tree(const Location& u,
		       const Neighborhood& neighbors) {
  
  typedef typename Location::difference_type EuclideanVector;

  // Compute the geometry of the data event defined by neighborhood neighbors 
  std::vector<EuclideanVector> data_event_geometry;
  data_event_geometry.reserve(neighbors.size());
  
  for(typename Neighborhood::const_iterator neigh_it=neighbors.begin(); 
      neigh_it!=neighbors.end(); ++neigh_it) {
    if( neigh_it->is_informed() )
      data_event_geometry.push_back( neigh_it->location() - u );
  }
  
  window_.set_geometry(data_event_geometry.begin(), data_event_geometry.end());

  
  /* scan the training image.
   * For each node u of the training image, 
   *   - retrieve the neighbors of u using window neighborhood window_,
   *   - update the degenerated search tree using the data event defined by window_
   */
  for(Iterator it = begin_; it!=end_; ++it) {
    
    // retrieve the data event at it
    window_.find_neighbors( *it );
    
    update_tree(window_,
		it->property_value(),
		neighbors);
  }

  return true;
}



template<class Iterator, class Neighborhood>
void 
Scan_image_cdf_estimator_t<Iterator,Neighborhood>::
update_tree(const Neighborhood& data_event,
            Property_type current_facies,
	    const Neighborhood& neighbors) {
  typename Neighborhood::const_iterator it = data_event.begin(); 
  typename Neighborhood::const_iterator neighbor_it=neighbors.begin();
  int node_id = 0;

  while(it != window_.end() ) {
    if( !it->is_informed() || it->property_value() != neighbor_it->property_value() )
      break;

    degenerated_tree_[node_id][current_facies]++;
    ++ it;
    ++ neighbor_it;
    ++ node_id;
  }
}



template<class Iterator, class Neighborhood>
template<class Cdf>
bool 
Scan_image_cdf_estimator_t<Iterator,Neighborhood>::get_cdf(Cdf& ccdf) {

  int last = degenerated_tree_.size() - 1;
  double nb_of_replicates = 0;
  for(int i=0; i<nb_of_categories_; i++)
    nb_of_replicates += degenerated_tree_[last][i];
  
  /*
  typename Cdf::p_iterator p_it = ccdf.p_begin();
  int index = 0;
  
   *p_it = degenerated_tree_[last][index] / nb_of_replicates;
   ++p_it; 
   ++index;
   for( ; p_it != ccdf.p_end(); ++p_it)
   *p_it = degenerated_tree_[last][index]/nb_of_replicates + *(p_it-1);
   */

  int index = 0;
  for( typename Cdf::p_iterator p_it = ccdf.p_begin() ;
       p_it != ccdf.p_end(); ++p_it, ++index )
    *p_it = degenerated_tree_[last][index]/nb_of_replicates ;

  return true;
}


#ifdef __GNUC__
#pragma implementation
#endif
#include "scan_image_cdf_estimator.h"



