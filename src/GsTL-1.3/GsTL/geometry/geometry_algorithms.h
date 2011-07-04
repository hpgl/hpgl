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

#ifndef GSTL_GEOMETRY_ALGORITHMS_H
#define GSTL_GEOMETRY_ALGORITHMS_H
#ifdef __GNUC__
#pragma interface
#endif



#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/utils/debug_tools.h>


#define DEG2RAD .01745329251994329576   //  = pi/180



/** Converts degrees to radians
 */
template<class RealType>
RealType degree_to_radian(RealType degree_angle){
  return DEG2RAD*degree_angle;
}



/** Computes the euclidean distance between two Locations
 */
template<class Location>
inline double 
euclidean_distance(const Location& loc1, const Location& loc2){
  double dist=0;
  for(int i=0; i<Location::dimension ; i++)
    dist+=(loc1[i]-loc2[i])*(loc1[i]-loc2[i]);

  return sqrt(dist);
}


/** Computes the squared euclidean distance between two Locations
 */
template<class Location>
inline double 
square_euclidean_distance(const Location& loc1, const Location& loc2){
  double dist=0;
  for(int i=0; i<Location::dimension ; i++)
    dist+=(loc1[i]-loc2[i])*(loc1[i]-loc2[i]);

  return dist;
}


template<class EuclideanVector>
inline double 
euclidean_norm(const EuclideanVector& v){
  double sq_norm=0;
  for(int i=0; i<EuclideanVector::dimension ; i++)
    sq_norm+= v[i]*v[i];

  return sqrt(sq_norm);
}


template<class EuclideanVector>
inline double 
square_euclidean_norm(const EuclideanVector& v){
  double sq_norm=0;
  for(int i=0; i<EuclideanVector::dimension ; i++)
    sq_norm+= v[i]*v[i];

  return sq_norm;
}

template<class EuclideanVector>
inline double
inner_product( const EuclideanVector& v1, const EuclideanVector& v2 ){
  double res=0.0;
  for(int i=0; i<EuclideanVector::dimension ; i++)
    res += v1[i]*v2[i];

  return res;
}


/** This functor performs an anisotropic transformation, ie a rotation
 * and a stretch of the coordinates. The stretch is not the same on all coordinates.
 * The anisotropy is defined by an 
 * ellipsis, characterized by 3 parameters: a max and min radius, and the
 * angle between the max radius and the X axis.
 * The transformation matrix is:
 * S*R with:
 * S= |A1  0|     R= |cos(T1)   sin(T1)|
 *    |0  A2|        |-sin(T1)  cos(T1)|
 * with A1=1/max_radius, A2=1/min_radius. T1 is the angle between the direction 
 * of maximum continuity and the X axis (it is NOT the azimuth).
 */
template<class EuclideanVector>
class Anisotropic_norm_2d{
 public:
  typedef EuclideanVector argument_type;
  typedef double result_type;
  
  /** Sets up the transformation.
   * @param range_max is the stretching factor
   * @param angle is expressed in rad. and is measured from the
   * X axis, counter-clockwise.
   */
  Anisotropic_norm_2d(double max_radius, double min_radius, 
				double angle);
  
  Anisotropic_norm_2d(const Anisotropic_norm_2d& rhs);

  inline result_type operator()(const argument_type& vec) const;


 private:
  TNT::Matrix<double> transformation_matrix_;
};


template<class EuclideanVector>
Anisotropic_norm_2d<EuclideanVector>::
Anisotropic_norm_2d(double max_radius, double min_radius, 
		    double angle) {

  TNT::Matrix<double> S(2,2,0.0);
  TNT::Matrix<double> R(2,2);

  gstl_assert( min_radius != 0 );
  S(1,1)=1.0;
  S(2,2)=max_radius/min_radius;

  R(1,1) =  cos(angle);
  R(2,2) =  cos(angle);
  R(1,2) =  sin(angle);
  R(2,1) = -sin(angle);
  
  transformation_matrix_ = S*R;
}


template<class EuclideanVector>
Anisotropic_norm_2d<EuclideanVector>::
Anisotropic_norm_2d(const Anisotropic_norm_2d& rhs) {
  transformation_matrix_ = rhs.transformation_matrix_;
}


template<class EuclideanVector>
inline double
Anisotropic_norm_2d<EuclideanVector>::
operator()(const argument_type& vec) const {

  gstl_assert(argument_type::dimension >= 2);
  double norm2=0;

  // Compute the coordinates as double and cast into the actual coordinate_type
  for(int i=0; i<2; i++){
    double new_coord =  transformation_matrix_(i+1,1) * static_cast<double>(vec[0])
                      + transformation_matrix_(i+1,2) * static_cast<double>(vec[1]);
    norm2 += new_coord*new_coord;
  }

  return sqrt(norm2);;
}





/** This functor performs an anisotropic transformation, ie a rotation
 * and an anisotropic stretch of the coordinates. The anisotropy is defined  
 * by an ellipsis, characterized by 6 parameters: a max, medium and min radii,
 * and 3 rotation angles. 
 * Call alpha, beta and theta the three angles, (XYZ) the original coordinate 
 * system. All angles are measured counter-clockwise.
 * The first rotation is about the Z axis, by angle alpha. call X'Y'Z the new 
 * coordinate system. 
 * The second rotation is about axis Y', by angle beta. Call X''Y'Z' the new
 * coordinate system.
 * The last rotation is about axis X'', by angle theta.
 * The new axis X",Y",Z" identify the directions of maximum continuity,
 * "medium" of continuity, and least continuity, respectively.
 */
template<class EuclideanVector>
class Anisotropic_norm_3d{
 public:
  typedef EuclideanVector argument_type;
  typedef double result_type;
  
  /** Sets up the transformation.
   */
  Anisotropic_norm_3d(double max_radius, double mid_radius,
		      double min_radius, 
		      double alpha, double beta, double theta);
  
  Anisotropic_norm_3d(const Anisotropic_norm_3d& rhs);

  inline result_type operator()(const argument_type& vec) const;


 private:
  TNT::Matrix<double> transformation_matrix_;

};


template<class EuclideanVector>
Anisotropic_norm_3d<EuclideanVector>::
Anisotropic_norm_3d(double max_radius, double mid_radius, 
		    double min_radius, 
		    double alpha, double beta, double theta) {

// The thrid argument of TNT matrix constructor is the initialization value
  TNT::Matrix<double> S(3,3,0.0);
  TNT::Matrix<double> R1(3,3,0.0);
  TNT::Matrix<double> R2(3,3,0.0);
  TNT::Matrix<double> R3(3,3,0.0);

  bool equal_zero=false;
  bool too_big = false;

  if( max_radius==0 ) {equal_zero=true; max_radius=0.0001; }
  if( mid_radius==0 ) {equal_zero=true; mid_radius=0.0001; }
  if( min_radius==0 ) {equal_zero=true; min_radius=0.0001; }
  if( mid_radius > max_radius ) {too_big=true; mid_radius = max_radius; }
  if( min_radius > mid_radius ) {too_big=true; min_radius = mid_radius; }
  
  if( equal_zero ) {
    gstl_warning( "one (or more) of the radii was reset to 0.0001 "
		  << "because it was equal to zero" );
  }
  if( too_big ) {
     gstl_warning( "The ranges values were modified so that"
		   << " max_radius >= mid_radius >= min_radius" );
  }
 

  gstl_assert( min_radius != 0 && mid_radius != 0 );
  S(1,1)=1.0;
  S(2,2)=max_radius/mid_radius;
  S(3,3)=max_radius/min_radius;
  
  // Rotation about Z-axis
  R1(1,1) =  cos(alpha);
  R1(2,2) =  cos(alpha);
  R1(1,2) =  sin(alpha);
  R1(2,1) = -sin(alpha);
  R1(3,3) =  1;

  // Rotation about Y'-axis
  R2(1,1) =  cos(beta);
  R2(3,3) =  cos(beta);
  R2(1,3) = -sin(beta);
  R2(3,1) =  sin(beta);
  R2(2,2) =  1;

  // Rotation around X"-axis
  R3(1,1) =  1;
  R3(2,2) =  cos(theta);
  R3(3,3) =  cos(theta);
  R3(2,3) =  sin(theta);
  R3(3,2) = -sin(theta);

  //transformation_matrix_ = S*((R1*R2)*R3);   //order seems wrong
  // rotation order: R1 first, then R2 and R3
  transformation_matrix_ = S*((R3*R2)*R1);

  WRITE_TO_DEBUG_STREAM( "S: " << S << "\n"
	    << "R1: " << R1 <<  "\n"
	    << "R2: " << R2 << "\n"
	    << "R3: " << R3 << "\n"
	    << "transf: " << transformation_matrix_<< std::endl );

}


template<class EuclideanVector>
Anisotropic_norm_3d<EuclideanVector>::
Anisotropic_norm_3d(const Anisotropic_norm_3d& rhs) 
  : transformation_matrix_(rhs.transformation_matrix_) {
}


template<class EuclideanVector>
inline double
Anisotropic_norm_3d<EuclideanVector>::
operator()(const argument_type& vec) const {

  gstl_assert(argument_type::dimension >= 3);
  double norm2=0;

  // Compute the coordinates as double and cast into the actual coordinate_type
  for(int i=0; i<3; i++){
    double new_coord =  transformation_matrix_(i+1,1) * static_cast<double>(vec[0])
                      + transformation_matrix_(i+1,2) * static_cast<double>(vec[1])
                      + transformation_matrix_(i+1,3) * static_cast<double>(vec[2]);
    
    norm2 += new_coord * new_coord;
  }

  return sqrt(norm2);
}





template<class EuclideanVector>
class Anisotropic_transform_3d {
  typedef Anisotropic_transform_3d<EuclideanVector> Self;

 public:
  typedef EuclideanVector argument_type;
  typedef double result_type;
  
  /** Sets up the transformation.
   */
  Anisotropic_transform_3d(double max_radius, double mid_radius,
		      double min_radius, 
		      double alpha, double beta, double theta);
  
  Anisotropic_transform_3d(const Self& rhs);

  inline EuclideanVector operator()(const argument_type& vec) const;


 private:
  TNT::Matrix<double> transformation_matrix_;

};


template<class EuclideanVector>
Anisotropic_transform_3d<EuclideanVector>::
Anisotropic_transform_3d(double max_radius, double mid_radius, 
		    double min_radius, 
		    double alpha, double beta, double theta) {

// The thrid argument of TNT matrix constructor is the initialization value
  TNT::Matrix<double> S(3,3,0.0);
  TNT::Matrix<double> R1(3,3,0.0);
  TNT::Matrix<double> R2(3,3,0.0);
  TNT::Matrix<double> R3(3,3,0.0);

  bool equal_zero=false;
  bool too_big = false;

  if( max_radius==0 ) {equal_zero=true; max_radius=0.0001; }
  if( mid_radius==0 ) {equal_zero=true; mid_radius=0.0001; }
  if( min_radius==0 ) {equal_zero=true; min_radius=0.0001; }
  if( mid_radius > max_radius ) {too_big=true; mid_radius = max_radius; }
  if( min_radius > mid_radius ) {too_big=true; min_radius = mid_radius; }
  
  if( equal_zero ) {
    gstl_warning( "one (or more) of the radii was reset to 0.0001 "
		  << "because it was equal to zero" );
  }
  if( too_big ) {
     gstl_warning( "The ranges values were modified so that"
		   << " max_radius >= mid_radius >= min_radius" );
  }
 

  gstl_assert( min_radius != 0 && mid_radius != 0 );
  S(1,1)=1.0;
  S(2,2)=max_radius/mid_radius;
  S(3,3)=max_radius/min_radius;
  
  // Rotation about Z-axis
  R1(1,1) =  cos(alpha);
  R1(2,2) =  cos(alpha);
  R1(1,2) =  sin(alpha);
  R1(2,1) = -sin(alpha);
  R1(3,3) =  1;

  // Rotation about Y'-axis
  R2(1,1) =  cos(beta);
  R2(3,3) =  cos(beta);
  R2(1,3) = -sin(beta);
  R2(3,1) =  sin(beta);
  R2(2,2) =  1;

  // Rotation around X"-axis
  R3(1,1) =  1;
  R3(2,2) =  cos(theta);
  R3(3,3) =  cos(theta);
  R3(2,3) =  sin(theta);
  R3(3,2) = -sin(theta);

  //transformation_matrix_ = S*((R1*R2)*R3);   //order seems wrong
  // rotation order: R1 first, then R2 and R3
  transformation_matrix_ = S*((R3*R2)*R1);

  WRITE_TO_DEBUG_STREAM( "S: " << S << "\n"
	    << "R1: " << R1 <<  "\n"
	    << "R2: " << R2 << "\n"
	    << "R3: " << R3 << "\n"
	    << "transf: " << transformation_matrix_<< std::endl );

}


template<class EuclideanVector>
Anisotropic_transform_3d<EuclideanVector>::
Anisotropic_transform_3d(const Self& rhs) 
  : transformation_matrix_(rhs.transformation_matrix_) {
}


template<class EuclideanVector>
inline EuclideanVector
Anisotropic_transform_3d<EuclideanVector>::
operator()(const argument_type& vec) const {

  gstl_assert(argument_type::dimension >= 3);
  EuclideanVector v;
   
  // Compute the coordinates as double and cast into the actual coordinate_type
  for(int i=0; i<3; i++){
    v[i] =  transformation_matrix_(i+1,1) * static_cast<double>(vec[0]) +
            transformation_matrix_(i+1,2) * static_cast<double>(vec[1]) +
            transformation_matrix_(i+1,3) * static_cast<double>(vec[2]);
  }

  return v;
}


#endif
