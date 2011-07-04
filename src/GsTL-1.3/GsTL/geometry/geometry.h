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

#ifndef GSTL_GEOMETRY_CLASSES_H
#define GSTL_GEOMETRY_CLASSES_H
#ifdef __GNUC__
#pragma interface
#endif


#include <iostream>

/** This class implements a 2D vector of an Euclidean space
 */
template<class T>
class euclidean_vector_2d{
 public:
  typedef T coordinate_type;

  static const int dimension = 2;
  
  euclidean_vector_2d(coordinate_type x=0, coordinate_type y=0){
    coord_[0]=x; coord_[1]=y;
  }

  euclidean_vector_2d(const euclidean_vector_2d& rhs) {
    for(int i=0; i<dimension ; i++)
      coord_[i] = rhs.coord_[i];
  }

  euclidean_vector_2d& operator=(const euclidean_vector_2d& rhs) {
    for(int i=0; i<dimension ; i++)
      coord_[i] = rhs.coord_[i];
    return *this;
  }

  inline bool operator==(euclidean_vector_2d loc) const{
    if( loc[0]==coord_[0] && loc[1]==coord_[1] )
      return true;
    else
      return false;
  }

  inline coordinate_type& operator[](unsigned int i) {return coord_[i];}
  inline const coordinate_type& operator[](unsigned int i) const {return coord_[i];}
  
 private:
  coordinate_type coord_[2];
};


/** This class implements a 3D vector of an Euclidean space
 */
template<class T>
class euclidean_vector_3d{
 public:
  typedef T coordinate_type;

  static const int dimension = 3;
  
  euclidean_vector_3d(coordinate_type x=0, coordinate_type y=0, 
		      coordinate_type z=0) {
    coord_[0]=x; coord_[1]=y; coord_[2]=z;
  }

  euclidean_vector_3d(const euclidean_vector_3d& rhs) {
    for(int i=0; i<dimension ; i++)
      coord_[i] = rhs.coord_[i];
  }

  euclidean_vector_3d& operator=(const euclidean_vector_3d& rhs) {
    for(int i=0; i<dimension ; i++)
      coord_[i] = rhs.coord_[i];
    return *this;
  }

  inline bool operator==(euclidean_vector_3d loc) const {
    if( loc[0]==coord_[0] && loc[1]==coord_[1] && loc[2]==coord_[2] )
      return true;
    else
      return false;
  }

  inline coordinate_type& operator[](unsigned int i) {return coord_[i];}
  inline const coordinate_type& operator[](unsigned int i) const {return coord_[i];}
  
private:
  coordinate_type coord_[3];
};




/** This class implements a 2D point (location) of an Euclidean space
 */
template<class T>
class location_2d{
 public:
  typedef T coordinate_type;
  typedef euclidean_vector_2d<T> difference_type;

  static const int dimension = 2;
  
  explicit location_2d(coordinate_type x=0, coordinate_type y=0){
    coord_[0]=x; coord_[1]=y;
  }

  inline bool operator==(location_2d loc) const{
    if( loc[0]==coord_[0] && loc[1]==coord_[1] )
      return true;
    else
      return false;
  }

  inline coordinate_type& operator[](unsigned int i) {return coord_[i];}
  inline const coordinate_type& operator[](unsigned int i) const {return coord_[i];}
  
  inline euclidean_vector_2d<T> operator-(const location_2d& P) const;
  inline location_2d<T> operator+(const difference_type& V) const;

 private:
  coordinate_type coord_[2];
};

template<class T>
inline euclidean_vector_2d<T> location_2d<T>::operator-(const location_2d<T>& P) const {
  euclidean_vector_2d<T> res(coord_[0]-P[0], coord_[1]-P[1]);
  return res;
};

template<class T>
inline location_2d<T> location_2d<T>::operator+(const difference_type& V) const {
  return location_2d<T>(coord_[0]+V[0], coord_[1]+V[1]);
}



/** This class implements a 3D point (location) of an Euclidean space
 */
template<class T>
class location_3d{
 public:
  typedef T coordinate_type;
  typedef euclidean_vector_3d<T> difference_type;

  static const int dimension = 3;
  
  explicit location_3d(coordinate_type x=0, coordinate_type y=0, coordinate_type z=0){
    coord_[0]=x; coord_[1]=y; coord_[2]=z;
  }

  inline bool operator==(location_3d loc) const{
    if( loc[0]==coord_[0] && loc[1]==coord_[1] && loc[2]==coord_[2] )
      return true;
    else
      return false;
  }

  inline coordinate_type& operator[](unsigned int i) {return coord_[i];}
  inline const coordinate_type& operator[](unsigned int i) const {return coord_[i];}
  
  inline euclidean_vector_3d<T> operator-(const location_3d& P) const;
  inline location_3d<T> operator+(const difference_type& V) const;

private:
  coordinate_type coord_[3];
};

template<class T>
inline euclidean_vector_3d<T> location_3d<T>::operator-(const location_3d<T>& P) const {
  euclidean_vector_3d<T> res(coord_[0]-P[0], coord_[1]-P[1], coord_[2]-P[2]);
  return res;
};


template<class T>
inline location_3d<T> location_3d<T>::operator+(const difference_type& V) const {
  return location_3d<T>(coord_[0]+V[0], coord_[1]+V[1], coord_[2]+V[2]);
}





template <class T>
std::ostream& operator<<(std::ostream& os, const location_2d<T>& loc) {
  os << "(" << loc[0] << " , " << loc[1] << ")" << std::endl;
  return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const location_3d<T>& loc) {
  os << "(" 
     << loc[0] << " , " 
     << loc[1] << " , "
     << loc[2] << ")" ;
  return os;
}


#endif
