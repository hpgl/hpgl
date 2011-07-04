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

#ifndef __GSTL_CARTESIAN_GEOMETRY__
#define __GSTL_CARTESIAN_GEOMETRY__



/** cartesian_geometry is an implementation (model) of a Geometry Server.
 * A Geometry Server is an object that represents the geometrical organization
 * of a given number of nodes in space.
 * cartesian_geometry represents a cartesian, regular, geometry in 3D.
 * It distinguishes between two types of coordinates:
 * the real word coordinates, implemented by RealSpaceLocation and
 * the grid coordinates, implemented by GridSpaceLocation.
 * The grid coordinates are the indexes of a node in the grid (the grid is 
 * viewed as a 3d matrix). 
 * GridSpaceLocation::coordinate_type is of integral type (int, unsigned int,...)
 * RealSpaceLocation::coordinate_type is usually a real number (double, float,...)
 * All the nodes of the geometry are given a unique integer value, or index.
 * 
 * The cartesian_geometry follows the GSLIB conventions for grids.
 */

#include <GsTL/geometry/geometry.h>
#include <GsTL/math/random_number_generators.h>



class cartesian_geometry{

 public:
  typedef location_3d<float> real_location_type;
  typedef location_3d<int>   grid_location_type;
  typedef real_location_type::coordinate_type real_coordinate_type;

  /** Constructs a cartesian geometry. All the parameters required are described in
   * the GSLIB book.
   * @param nx, ny, nz are the number of cells in each direction
   * @param xsize,... are the dimensions of each cell of the grid
   * @param x_origin,... are the coodinates of the origin of the grid.
   */
  cartesian_geometry(int nx, int ny, int nz, 
		     double xsize=1, double ysize=1, double zsize=1,
		     real_coordinate_type x_origin = 0, real_coordinate_type y_origin = 0,
		     real_coordinate_type z_origin = 0);

  ~cartesian_geometry();

  inline void resize(int nx, int ny, int nz);
  inline void set_cells_dimensions(double xsize, double ysize, double zsize);

  /** @return the total size of the geometry, i.e. nx*ny*nz
   */
  inline int size() const;
  inline int nx() const;
  inline int ny() const;
  inline int nz() const;
  
  inline bool contains_node(int index) const;
  inline bool contains_node(const grid_location_type& loc) const;
  inline bool contains_node(const real_location_type& loc) const;
  

  /** Converts a grid location into a real location.
   */
  inline real_location_type convert(const grid_location_type& loc) const;
  inline grid_location_type convert(const real_location_type& loc) const;
  

  /** Returns the index, ie the unique integer identifier associated
   * with node located at loc. This function is overloaded
   * so as to take either a real-space location or a grid-space location.
   */
  inline int get_index(const real_location_type& loc) const;
  inline int get_index(const grid_location_type& loc) const;
  
  /** From an index, get the corresponding location. 
   */
  inline real_location_type get_real_location(int index) const;
  inline grid_location_type get_grid_location(int index) const;
  


 private:
  int nx_, ny_, nz_;
  double xsize_, ysize_, zsize_;
  real_location_type origin_;
  int size_;
  int nxy_;
  
}; // end of class cartesian_geometry







//====================================================
// non-inline functions. Should be a in .cc file...


inline cartesian_geometry::cartesian_geometry(int nx, int ny, int nz, 
					      double xsize, double ysize, double zsize,
					      real_coordinate_type x_origin, 
					      real_coordinate_type y_origin,
					      real_coordinate_type z_origin)
  : nx_(nx), ny_(ny), nz_(nz), xsize_(xsize), ysize_(ysize), zsize_(zsize), 
     origin_(x_origin,y_origin,z_origin)  {
  
  size_ = nx*ny*nz;
  nxy_ = nx*ny;
}



inline cartesian_geometry::~cartesian_geometry() { }





//====================================
// Inline functions


inline void cartesian_geometry::resize(int nx, int ny, int nz) {
  nx_ = nx;
  ny_ = ny;
  nz_ = nz;
  size_ = nx*ny*nz;
  nxy_ = nx*ny;
}

inline void cartesian_geometry::set_cells_dimensions(double xsize,
						     double ysize, 
						     double zsize) {
  xsize_ = xsize;
  ysize_ = ysize;
  zsize_ = zsize;
}


inline int cartesian_geometry::size() const {
  return size_;
}


inline int cartesian_geometry::nx() const { 
  return nx_; 
}


inline int cartesian_geometry::ny() const { 
  return ny_; 
}


inline int cartesian_geometry::nz() const { 
  return nz_; 
}



inline cartesian_geometry::real_location_type 
cartesian_geometry::convert(const grid_location_type& loc) const {

  typedef real_location_type::coordinate_type coord_type;
  coord_type x = origin_[0] + loc[0]*xsize_;
  coord_type y = origin_[1] + loc[1]*ysize_;
  coord_type z = origin_[2] + loc[2]*zsize_;
  
  return real_location_type(x,y,z);
}



inline cartesian_geometry::grid_location_type 
cartesian_geometry::convert(const real_location_type& loc) const {

  typedef grid_location_type::coordinate_type coord_type;
  
  coord_type x = static_cast<coord_type>((loc[0]-origin_[0])/xsize_);
  coord_type y = static_cast<coord_type>((loc[1]-origin_[1])/ysize_);
  coord_type z = static_cast<coord_type>((loc[2]-origin_[2])/zsize_);
  
  return grid_location_type(x,y,z);
}



inline bool cartesian_geometry::contains_node(int index) const {
  return index >=0 && index < static_cast<int>(size());
}

inline bool cartesian_geometry::contains_node(const grid_location_type& loc) const {
  return loc[0] >=0 && loc[0]<nx_ && loc[1]>=0 && 
    loc[1]<ny_ && loc[2]>=0 && loc[2]<nz_;
}

inline bool cartesian_geometry::contains_node(const real_location_type& loc) const {
  return contains_node(convert(loc));
}
  


inline int 
cartesian_geometry::get_index(const grid_location_type& loc) const {

  return loc[2]*nxy_ + loc[1]*nx_ + loc[0];
}
 




inline int 
cartesian_geometry::get_index(const real_location_type& loc) const {
  return get_index( convert(loc) );
}




inline cartesian_geometry::grid_location_type
cartesian_geometry::get_grid_location(int index) const {
  
  typedef grid_location_type::coordinate_type coord_type;
  
  coord_type z = static_cast<coord_type>(double(index) / double(nxy_) );
  int interm = index - z*nxy_;
  coord_type y = static_cast<coord_type>(double(interm) / double(nx_) );
  coord_type x = interm - y*nx_;

  return grid_location_type(x,y,z);
}



 

inline cartesian_geometry::real_location_type
cartesian_geometry::get_real_location(int index) const {
  
  return convert(get_grid_location(index));
}



  


#endif
