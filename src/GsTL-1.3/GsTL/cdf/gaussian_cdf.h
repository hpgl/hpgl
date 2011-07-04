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

 

#ifndef __GSTL_CDF_GAUSSIAN_CDF_H__
#define __GSTL_CDF_GAUSSIAN_CDF_H__


#include <GsTL/cdf/cdf_basics.h>
#include <GsTL/math/math_functions.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <cmath>


class Gaussian_cdf : public Cdf<float> {

 public:

  typedef GsTL::continuous_variable_tag   variable_category;


  Gaussian_cdf(value_type mean = 0.0, value_type var = 1.0) : mean_(mean) ,
                                                              var_(var){}
  virtual ~Gaussian_cdf() {}

  float& mean() {return mean_;}
  const float& mean() const {return mean_;}

  float& variance() {return var_;}
  const float& variance() const {return var_;}

  int set_moments(float mean, float var ) {
    if( var < 0 ) return 1;
    mean_ = mean;
    var_ = var;
    return 0;
  }

  // Added Oct 2005.  Set up the mean and the variance
 // void mean( value_type mean ) { mean_ = mean;}
 // void variance( value_type var ) { var_ = var;}


  /** Computes zk such that P(Z<z) = p.
   * This function uses a numerical approximation from : Statistical Computing,
   * by W.J. Kennedy, Jr. and James E. Gentle, 1980, p. 95.
   *
   * @return z such that P(Z<z)=p
   * @param p is a real number between 0 and 1 (probability)
   */
  virtual value_type inverse(double p) const;


  /** Computes the probability P(Z<z).
   * This function uses the approximation proposed by Abramowitz and Stegun
   * in "Handbook of Mathematical Functions", Dover Publications, 1965.
   * The error in this approximation is less than 0.00001 for all z.
   *
   * @return P(Z<z)
   */
  virtual double prob(value_type z) const;


 private:

  value_type mean_;
  value_type var_;
  
}; // end of class Gaussian_cdf




//___________________________
// Inline functions

inline Gaussian_cdf::value_type 
       Gaussian_cdf::inverse(double p) const {

  // Make sure the variance is not negative...
  gstl_assert(var_ >= 0);

  static const double lim=1.0e-12;
  static const double INFINITY = GsTL::INFINITY;

  //  static const double P0 = -0.322232431088;
  static const double Pconst[5]= {-0.322232431088,-1.0,-0.342242088547,
				  -0.0204231210245,-0.0000453642210148};
  static const double Qconst[5]= {0.0993484626060,0.588581570495,0.531103462366,
				  0.103537752850,0.0038560700634};
  
  gstl_assert(p<=1.0 && p>=0.0);

  // specific cases  
  if (p<=lim) return -INFINITY;
  if (p>=1-lim) return INFINITY; 
  if(p==0.5) return mean_;
  
  double tmp_p=p;
  
  if(p>0.5)
    tmp_p=1-p; 

  double y = sqrt(log(1.0/(tmp_p*tmp_p))); 
  double num=(((y*Pconst[4]+Pconst[3])*y+Pconst[2])*y+Pconst[1])*y+Pconst[0];
  double denom=(((y*Qconst[4]+Qconst[3])*y+Qconst[2])*y+Qconst[1])*y+Qconst[0];

  double result = y+num/denom;

  if (p==tmp_p)
    result = -result;

  return result*sqrt(var_) + mean_;
};




inline double Gaussian_cdf::prob(value_type z) const {

  static const double Aconst[3]= {0.4361836, -0.1201676, 0.9372980};

  z = (z-mean_)/sqrt(var_);

  double tmp_z=z;
  if(z<0) tmp_z=-z;
  double t=1/(1+.33267*tmp_z);
  double p_z = 1/sqrt( 2 * GsTL::PI ) * exp( -0.5*tmp_z*tmp_z );
  double P = 1-p_z*(Aconst[0]*t + Aconst[1]*t*t + Aconst[2]*pow(t,3));

  if(z<0) P=0.5-(P-0.5);

  return P;
};




#endif
