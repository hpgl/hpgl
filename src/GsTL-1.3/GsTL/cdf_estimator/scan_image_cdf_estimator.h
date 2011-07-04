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

#ifndef __GSTL_SCAN_IMAGE_CDF_ESTIMATOR_H__
#define __GSTL_SCAN_IMAGE_CDF_ESTIMATOR_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <vector>

/** A Scan Image Cdf Estimator estimates a categorical cdf by scanning
 * a training image, searching for replicates of the data event (the 
 * data event is defined by the neighborhood of the location to be simulated).
 * If there is not an exact match of the data event in the training image,
 * one conditioning datum is dropped, and so on until the simplified data 
 * event is found into the training image. To avoid rescanning the image if
 * one conditioning datum has to be dropped, a "degenerated search tree" is 
 * built: we only build the branch defined by the data event.
 * The neighborhood input to the scan_image_cdf_estimator can either be
 * an ellipsoid neighborhood or a window neighborhood.
 */

template<class ForwardIterator, class Neighborhood>
class Scan_image_cdf_estimator_t {
 
 public:

  /** @param [begin, end) is a range of geovalues (the training image).
   */
  Scan_image_cdf_estimator_t(ForwardIterator begin, ForwardIterator end,
			     Neighborhood& neighbors,
			     int nb_of_categories);

			  
  template<class Geovalue_, class CategNonParamCdf>
  int operator()(const Geovalue_& u,
		 const Neighborhood& neighbors,
		 CategNonParamCdf& ccdf);




 private:

  typedef typename Neighborhood::value_type Geovalue_type;
  typedef typename Geovalue_type::property_type Property_type;

  ForwardIterator begin_;
  ForwardIterator end_;
  Neighborhood& window_;

  int nb_of_categories_;

  std::vector<std::vector<double> > degenerated_tree_;


  void initialize_tree(int size);

  template<class Location>
  bool build_degenerated_tree(const Location& u,
			      const Neighborhood& neighbors);

  void update_tree(const Neighborhood& data_event,
		   Property_type facies,
		   const Neighborhood& neighbors);

  template<class Cdf>
  bool get_cdf(Cdf& ccdf);

}; // end of class Scan_image_cdf_estimator_t



#include "scan_image_cdf_estimator.cc"

#endif
