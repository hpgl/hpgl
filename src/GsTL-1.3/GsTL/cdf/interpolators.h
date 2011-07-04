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

 

#ifndef __GSTL_CDF_INTERPOLATORS_H__
#define __GSTL_CDF_INTERPOLATORS_H__

#include <GsTL/math/math_functions.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <cmath>
#include <memory>


/** Middle interpolator (MI)
 * interpolates linearly between two points (p1,z1) and (p2,z2)
 */
struct Linear_interpol {

  double z(const double z1, const double p1, 
	   const double z2, const double p2,
	   const double p) const {
    return ( z1*(p1-p2) - (z1-z2)*(p1-p) ) / (p1-p2);
  }
  
  double p(const double z1, const double p1,
	   const double z2, const double p2,
	   const double z) const {
    return ( p1*(z1-z2) - (z1-z)*(p1-p2) ) / (z1-z2);
  }
};




/** Lower Tail Interpolator (LTI)
 * exponential behavior: the probability p is given by:
 * p(z) = p1 * exp(-(z-z1)^2)
 *
 * (p1,z1) is a given point.
 */
/*struct Exponential_LTI{

  double z(const double z1, const double p1,
	   const double p) const {
    return z1-sqrt(log(p1/p));
  }

  double p(double z1, double p1,
	   const double z) const {
    return p1*exp(-pow((z-z1),2));
  }
};
*/



/** Upper Tail Interpolator (UTI)
 * exponential behavior: the probability p is given by:
 * p(z) = 1 - (1-p1) * exp(-(z-z1)^2)
 *
 * (p1,z1) is a given point.
 */
/*struct Exponential_UTI{

  double z( const double z1, const double p1,
	          const double p) const {
    return z1+sqrt( log( (1-p1)/(1-p) ) );
  }

  double p( const double z1, const double p1,
	          const double z) const {
    return 1-(1-p1)*exp(-pow((z-z1),2));
  }

};
*/

class Tail_interpolator_impl {
public:
  virtual ~Tail_interpolator_impl() {}
  virtual Tail_interpolator_impl* clone() const = 0;
  virtual double z( double z1, double p1, double p ) const = 0;
  virtual double p( double z1, double p1, double z ) const = 0;
};


/** Tail_interpolator makes a copy of implementation Tail_interpolator_impl.
* It does not take ownership of the pointer it is passed.
*/
class Tail_interpolator {
public:
  Tail_interpolator( Tail_interpolator_impl* f = 0 ) {
    f_ = 0;
    if( f )
      f_ = f->clone();
  }
  Tail_interpolator( const Tail_interpolator& rhs ) {
    if( rhs.f_ )
      f_ = rhs.f_->clone();
    else
      f_ = 0;
  }

  ~Tail_interpolator() { delete f_; };

  const Tail_interpolator& operator = ( const Tail_interpolator& rhs ) {
    if( f_ == rhs.f_ ) return *this;
    delete f_;
    if( rhs.f_ )
      f_ = rhs.f_->clone();
    else
      f_ = 0;
    return *this;
  }

  double z( double z1, double p1, double p ) const { return f_->z(z1,p1,p); }
  double p( double z1, double p1, double z ) const { return f_->p(z1,p1,z); }

private:
  Tail_interpolator_impl* f_;
};


/** No tail "interpolation" (extrapolation actually)
 *  Simply return the last and first z and p values.
*/
class No_TI  : public Tail_interpolator_impl {
public :
  No_TI( ) {}
  virtual Tail_interpolator_impl* clone() const { return new No_TI( );}
  virtual double z( double z1, double p1, double p ) const {
    return z1;
  }
  virtual double p( double z1, double p1, double z ) const {
    return p1;
  }
};


/** Hyperbolic upper-tail "interpolation" (extrapolation actually)
*/
class Hyperbolic_UTI : public Tail_interpolator_impl {
public:
  Hyperbolic_UTI( double omega = 1. ) : omega_( omega ) {
    if( omega_ < 1 ) {
      gstl_warning( "omega must be >= 1: resetting omega to 1" );
      omega_ = 1;
    }
  }
  virtual Tail_interpolator_impl* clone() const { return new Hyperbolic_UTI(omega_);}

  virtual double z( double z1, double p1, double p ) const {
    if( GsTL::equals( p, 1. ) ) 
      p = 1 - GsTL::EPSILON;
    return z1 * std::pow( ( 1-p1)/(1-p), 1./omega_ );
  }
  virtual double p( double z1, double p1, double z ) const {
    return 1 - ( 1-p1 )*std::pow( z1/z, omega_ );
  }

private:
  double omega_;
};




class Power_UTI : public Tail_interpolator_impl {
public:
  Power_UTI( double z_max, double omega = 1./3., double p_max = 1. ) 
    : z_max_( z_max ), p_max_( p_max ), omega_( omega ) {
    if( omega_ > 1 ) {
      gstl_warning( "omega must be <= 1: resetting omega to 1" );
      omega_ = 1;
    }
  }
  virtual Tail_interpolator_impl* clone() const { 
    return new Power_UTI( z_max_, omega_, p_max_ );
  }

  virtual double z( double z1, double p1, double p ) const {
    gstl_assert( z1 <= z_max_ );
    gstl_assert( p1 <= p_max_ );
    if( GsTL::equals( p1, p_max_ ) ) return z_max_;
    if( p > p_max_ ) return z_max_;

    return z1 + ( z_max_ - z1 ) * std::pow( (p-p1)/(p_max_-p1), 1./omega_ );
  }

  virtual double p( double z1, double p1, double z ) const {
    gstl_assert( z1 <= z_max_ );
    gstl_assert( p1 <= p_max_ );
    if( GsTL::equals( z_max_, z1 ) ) return p_max_;
    if( z >= z_max_ ) return p_max_;

    return p1 + (p_max_ - p1) * std::pow( (z-z1)/(z_max_-z1), omega_ );
  }

private:
  double z_max_, p_max_;
  double omega_;
};



class Power_LTI : public Tail_interpolator_impl {
public:
  Power_LTI( double z_min, double omega = 3., double p_min = 0. ) 
    : z_min_( z_min ), p_min_( p_min ), omega_( omega ) {
    if( omega_ < 1 ) {
      gstl_warning( "omega must be >= 1: resetting omega to 1" );
      omega_ = 1;
    }
  }
  virtual Tail_interpolator_impl* clone() const { 
    return new Power_LTI( z_min_, omega_, p_min_ );
  }

  virtual double z( double z1, double p1, double p ) const {
    gstl_assert( z1 >= z_min_ );
    gstl_assert( p1 >= p_min_ );
    if( GsTL::equals( p1, p_min_ ) ) return z_min_;
    if( p <= p_min_ ) return z_min_;

    return z_min_ + ( z1 - z_min_ ) * std::pow( (p1-p)/(p1-p_min_), 1./omega_ );
  }

  virtual double p( double z1, double p1, double z ) const {
    gstl_assert( z1 >= z_min_ );
    gstl_assert( p1 >= p_min_ );
    if( GsTL::equals( z_min_, z1 ) ) return p_min_;
    if( z <= z_min_ ) return p_min_;

    return p1 - (p1 - p_min_) * std::pow( (z1-z)/(z1-z_min_), omega_ );
  }

private:
  double z_min_, p_min_;
  double omega_;
};




class Exponential_LTI : public Tail_interpolator_impl {
  virtual Tail_interpolator_impl* clone() const { return new Exponential_LTI; }

  virtual double z( double z1, double p1, double p) const {
    gstl_assert( p <= p1 );
    if( p == 0. ) 
      p = GsTL::EPSILON;
    return z1 - std::sqrt( std::log(p1/p) );
  }

  virtual double p( double z1, double p1, double z) const {
    return p1 * std::exp( - std::pow((z-z1), 2) );
  }
};

#endif
