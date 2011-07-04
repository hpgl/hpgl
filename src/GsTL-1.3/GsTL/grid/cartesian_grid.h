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

#ifndef ___CARTESIAN_GRID__
#define ___CARTESIAN_GRID__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/grid/grid.h>
#include <GsTL/grid/cartesian_geometry.h>

template<class PropertyServer>
class cartesian_grid : public grid<PropertyServer, cartesian_geometry> {

 public:

  typedef cartesian_geometry::real_coordinate_type real_coordinate_type;

  cartesian_grid(int nx, int ny, int nz=1, 
		 double xsize=1, double ysize=1, double zsize=1,
		 real_coordinate_type x_origin = 0, real_coordinate_type y_origin = 0,
		 real_coordinate_type z_origin = 0); 
 
  inline int nx() const;
  inline int ny() const;
  inline int nz() const;


 private:
  cartesian_geometry cartesian_geom_server_;

}; // end of class cartesian_grid




template<class T>
inline int cartesian_grid<T>::nx() const {
  return cartesian_geom_server_.nx();
}


template<class T>
inline int cartesian_grid<T>::ny() const {
  return cartesian_geom_server_.ny();
}


template<class T>
inline int cartesian_grid<T>::nz() const {
  return cartesian_geom_server_.nz();
}



#if defined(__GNUC__) || defined(WIN32)
#include "cartesian_grid.cc"
#elif defined(__xlC__)
#pragma implementation("cartesian_grid.cc")
#endif


#endif
