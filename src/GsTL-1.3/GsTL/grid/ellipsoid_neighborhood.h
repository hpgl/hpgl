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

#ifndef ___ELLIPSOID_NEIGHBORHOOD__
#define ___ELLIPSOID_NEIGHBORHOOD__
#ifdef __GNUC__
#pragma interface
#endif


#include <vector>
#include <string>



template <class Grid>
class Ellipsoid_neighborhood{

 public:
  typedef typename Grid::geovalue_type value_type;

  typedef typename std::vector<value_type>::iterator iterator;
  typedef typename std::vector<value_type>::const_iterator const_iterator;
  
  template <class Covariance>
  Ellipsoid_neighborhood(int xradius, int yradius, int zradius,
			 int max_neighbors,
			 const Covariance& measure,
			 Grid& grid, std::string property_name);

  // Pseudo copy contructor. 
  // Creates a copy of the neighborhood, but on a different grid.
  Ellipsoid_neighborhood(Grid& grid, 
			 const Ellipsoid_neighborhood& rhs);


  template<class Location>
  void find_neighbors(const Location& loc);
 

  inline int size() const;
  inline bool empty() const;
 

  inline iterator begin();
  inline iterator end();

  inline const_iterator begin() const;
  inline const_iterator end() const;


 private:
  
  typedef typename value_type::location_type location;
  typedef typename location::difference_type EuclideanVector;
  typedef typename std::vector<EuclideanVector>::iterator geom_iterator;

  std::vector<value_type> neighbors_;
  std::vector<EuclideanVector> geometry_;

  int max_neighbors_;

  Grid& grid_;
  int property_name_id_;
  
  template<class Covariance>
  void set_geometry(int xradius, int yradius, int zradius,
		    const Covariance& measure);

};  // end of class ellipsoid_neighborhood.






//=============================================
// Inline functions

template <class Grid>
inline int Ellipsoid_neighborhood<Grid>::size() const {
  return static_cast<int>(neighbors_.size());
}


template <class Grid>
inline bool Ellipsoid_neighborhood<Grid>::empty() const {
  return neighbors_.empty();
}
 

template <class Grid>
inline typename Ellipsoid_neighborhood<Grid>::iterator
Ellipsoid_neighborhood<Grid>::begin() {
  return neighbors_.begin();
}


template <class Grid>
inline typename Ellipsoid_neighborhood<Grid>::iterator 
Ellipsoid_neighborhood<Grid>::end(){
  return neighbors_.end();
}

template <class Grid>
inline typename Ellipsoid_neighborhood<Grid>::const_iterator 
Ellipsoid_neighborhood<Grid>::begin() const{
  return neighbors_.begin();
}


template <class Grid>
inline typename Ellipsoid_neighborhood<Grid>::const_iterator 
Ellipsoid_neighborhood<Grid>::end() const{
  return neighbors_.end();
}




//===========================================
// Comparison of two euclidean vectors based on covariance value
// v1 > v2  iif  cov(v1) > cov(v2) 

template<class Covariance, class Location>
class Evector_greater_than_t {
 public:
  Evector_greater_than_t(const Covariance& proximity, const Location& center)
    : proximity_(&proximity), center_(center) { };

  template<class EuclideanVector>
  bool operator()(const EuclideanVector& v1, const EuclideanVector& v2) {
    return (*proximity_)(center_, center_ + v1) > (*proximity_)(center_, center_ + v2);
  }

 private:
  const Covariance* proximity_; 
  Location center_;
};


template<class Covariance, class Location>
Evector_greater_than_t<Covariance, Location> 
Evector_greater_than(const Covariance& proxim,  const Location& center) {
  return Evector_greater_than_t<Covariance, Location>(proxim, center);
}






#if defined(__GNUC__) || defined(WIN32)
#include "ellipsoid_neighborhood.cc"
#elif defined(__xlC__)
#pragma implementation("ellipsoid_neighborhood.cc")
#endif

#endif
