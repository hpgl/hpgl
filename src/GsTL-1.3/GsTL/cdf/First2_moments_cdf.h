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

 

#ifndef __GSTL_CDF_SOARES_CDF_H__
#define __GSTL_CDF_SOARES_CDF_H__


#include <GsTL/cdf/cdf_basics.h>
#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/math/math_functions.h>
#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/cdf/non_param_cdf.h>


#include <cmath>

const float MIN_PROB = 0.000001;
static float MAX_PROB = 0.999999;

class First2_moments_cdf : public Cdf<float> {

public :
	First2_moments_cdf(){}
	~First2_moments_cdf(){}
	virtual int set_moments(float mean, float var ) {return 0;}

  float mean() {return mean_;}
  const float mean() const {return mean_;}

  float variance() {return var_;}
  const float variance() const {return var_;}

  virtual value_type inverse(double p) const = 0;

  virtual double prob(value_type z) const = 0;

protected :
  value_type mean_;
  value_type var_;
};




/* ----------------------------------------
  Log Normal cdf, it simply a wrapper over Gaussian_cdf with
  the appropraite transformation from the normal space to the
  lognormal space
*/
class LogNormal_cdf : public First2_moments_cdf {

 public:

  typedef GsTL::continuous_variable_tag   variable_category;

  LogNormal_cdf(value_type mean = 1.0, value_type var = 1.0): mean_(mean) ,
                                                              var_(var)   {
	value_type gaussian_var = std::log( var_/mean_/mean_ + 1 );
	value_type gaussian_mean = std::log(mean_) -  gaussian_var/2;
	gaussian_cdf_ = new Gaussian_cdf(gaussian_mean,gaussian_var);
  }
  
  virtual ~LogNormal_cdf() {}

  float mean() {return mean_;}
  const float mean() const {return mean_;}

  int set_moments(float mean, float var ) {
	  if( mean <= 0 ) return 1;
	  mean_ = mean;
	  var_ = var;
	  gaussian_cdf_->variance() = std::log( var_/mean_/mean_ + 1 );
	  gaussian_cdf_->mean() = std::log(mean_) -  gaussian_cdf_->variance()/2;
	  return 0;
  }


  float variance() {return var_;}
  const float variance() const {return var_;}

  virtual value_type inverse(double p) const;

  virtual double prob(value_type z) const;

  private:

  value_type mean_;
  value_type var_;

  Gaussian_cdf* gaussian_cdf_;
  
};


inline LogNormal_cdf::value_type 
       LogNormal_cdf::inverse(double p) const {
	return std::exp( gaussian_cdf_->inverse(p) );
}

inline double LogNormal_cdf::prob(value_type z) const {
	return (z>0) ? gaussian_cdf_->prob( std::log(z) ) : 0.0;
}


//Uniform cdf. It is intialized with mean and variance instead of the
// traditional min and max.  The conversion is done for computation.
//
class Uniform_cdf : public First2_moments_cdf {

 public:

  typedef GsTL::continuous_variable_tag   variable_category;

  Uniform_cdf(value_type min = 0, value_type max = 1): a_(min), b_(max) {
    mean_ = (a_+b_)/2;
	var_ = (b_-a_)*(b_-a_)/12;
  }
  
  virtual ~Uniform_cdf() {}

  float mean() {return mean_;}
  const float mean() const {return mean_;}

  int set_moments(float mean, float var ) {
	mean_ = mean;
	var_ = var;
	a_ = mean_ - std::sqrt(3.0*var_);
	b_ = std::sqrt(3.0*var_) + mean_;
	return 0;
  }

  void set_ranges(float a, float b ); 

  float variance() {return var_;}
  const float variance() const {return var_;}

  virtual value_type inverse(double p) const{
	return a_ + p*(b_-a_);
}

  virtual double prob(value_type z) const;

  private:

  value_type mean_;
  value_type var_;
  float a_;
  float b_;
  
};

inline double Uniform_cdf::prob(value_type z) const 
{
	if( z <= a_ ) return 0.;
	if( z >= b_ ) return 1;
	return (z-a_)/(b_-a_);
}


 inline void Uniform_cdf::set_ranges(float a, float b ) 
 {
	assert( a < b);
	a_ = a;
	b_ = b;
	mean_ = (a_ + b_)/2;
	var_ = std::pow(b_-a_,2)/12;
  }






class Search_closest_ccdf {
public :
  Search_closest_ccdf(float mean ,float var): mean_(mean),var_(var){}
  ~Search_closest_ccdf( ){}

  template<typename MomentsCdf>
  bool operator()(MomentsCdf moments1,MomentsCdf moments2) {
    float val1 = std::pow((mean_ - moments1.first.first),2) + std::pow((var_ - moments1.first.second),2);
    float val2 = std::pow((mean_ - moments2.first.first),2) + std::pow((var_ - moments2.first.second),2);
    return val1 < val2;
  }
protected :
  float mean_;
  float var_;
};



/* ----------------------------------------
  Soares cdf, sampling of the global distribution centered on mean
  with variance spread.
*/

template< class Global_cdf >
class Soares_cdf : public First2_moments_cdf {

 public:

  typedef GsTL::continuous_variable_tag   variable_category;
  

  Soares_cdf(Global_cdf* global_cdf, bool bias_correction = true) :
	global_cdf_(global_cdf),local_bias_correction_(bias_correction) {
	gaussian_global_cdf_ = new Gaussian_cdf(0.0,1.0);
	gaussian_local_cdf_ = new Gaussian_cdf(0.0,1.0);
	 mean_ = global_cdf_->mean();
	 var_ = global_cdf_->variance();
	 mean_global_ = mean_;
	 var_global_ = var_;
   }
	  
  virtual ~Soares_cdf(){}

  float mean() {return mean_;}
  const float mean() const {return mean_;}

  int set_moments(float mean, float var );

  float variance() {return var_;}
  const float variance() const {return var_;}

  virtual value_type inverse(double p) const;

  virtual double prob(value_type z) const;

  protected:

	bool local_bias_correction_;

	float var_global_;
	float mean_global_;
	Global_cdf* global_cdf_;
	Gaussian_cdf* gaussian_local_cdf_;
	Gaussian_cdf* gaussian_global_cdf_;
  
};

template< class Global_cdf >
  inline int Soares_cdf<Global_cdf>::set_moments(float mean, float var )
  {
	  if(mean < global_cdf_->inverse(0.0) ) 
		  mean_ = global_cdf_->inverse(MIN_PROB) ;
	  else if (mean > global_cdf_->inverse(1.0) ) 
		  mean_ = global_cdf_->inverse(MAX_PROB) ;
	  else 
		  mean_ = mean;
	  var_ = var;
	  value_type gaussian_mean = gaussian_global_cdf_->inverse(global_cdf_->prob(mean_));
	  gaussian_local_cdf_->mean() = gaussian_mean;
	  gaussian_local_cdf_->variance() = var/var_global_;
	  return 0;
  }
 
template< class Global_cdf >
inline float Soares_cdf<Global_cdf>::inverse(double p) const 
{
	const float inc = 0.001;
	value_type y = gaussian_local_cdf_->inverse( p );
	value_type z = global_cdf_->inverse( gaussian_global_cdf_->prob( y ) );
	if(local_bias_correction_) {
		value_type biased_mean = 0.0;
		value_type biased_var = 0.0;
		for(float i=0.001; i<0.999; i+=inc ) {
			value_type z_s = global_cdf_->inverse( gaussian_global_cdf_->prob( gaussian_local_cdf_->inverse( i ) ) );
			biased_mean += z_s * inc;
			biased_var += z_s * z_s * inc;
		}
		biased_var -= biased_mean*biased_mean;
//		value_type z2=z;
//		z = ( z - biased_mean )*std::sqrt(var_/biased_var) + mean_;
		z = z - biased_mean + mean_;
		if(z < global_cdf_->inverse(0.0) ) return global_cdf_->inverse(MIN_PROB);
		if(z > global_cdf_->inverse(1.0) ) return global_cdf_->inverse(MAX_PROB);
		return z;
	}
	else return z;
//   value_type y = gaussian_local_cdf_->inverse( p );
 //  double p = gaussian_global_cdf_.prob( y );
//   return global_cdf_->inverse( gaussian_global_cdf_->prob( y ) );
}

template< class Global_cdf >
inline double Soares_cdf<Global_cdf>::prob(value_type z) const {
	return 0.0;
}

/* ----------------------------------------
  Deutsch cdf, Build a look-up table of ccdf from a sampling of the global 
  distribution centered on mean with variance spread.
*/

template< 
  typename Global_cdf,
  typename Sampling_cdf = Gaussian_cdf >
class Deutsch_cdf : public First2_moments_cdf {

 public:

  typedef GsTL::continuous_variable_tag   variable_category;
  

  Deutsch_cdf( Global_cdf* global_cdf, int inc_mean = 10, int inc_var = 10,int inc_q = 1000,  
        Sampling_cdf* sampling_cdf = NULL ) : global_cdf_(global_cdf) 
  {
      if(!sampling_cdf ) sampling_cdf = new Gaussian_cdf(0.0,1.0);

    mean_global_ = global_cdf->mean();
    var_global_ = global_cdf->variance();

    Sampling_cdf sampling_cdf1(*sampling_cdf);

    global_min_ = global_cdf->inverse(0.);
    global_max_ = global_cdf->inverse(1.);

    Tail_interpolator lti = Tail_interpolator( new Power_LTI(global_cdf->inverse(0)) );
    Linear_interpol mi = Linear_interpol();
    Tail_interpolator uti = Tail_interpolator( new Power_UTI(global_cdf->inverse(1)) );
    
    std::vector<float> p_values;
    for(float q=0.001; q<=0.999; q += 0.998/inc_q ) p_values.push_back( q );
    std::vector<float> q_values(p_values.size());

    float min_mean = sampling_cdf->inverse(0.01);
    float max_mean = sampling_cdf->inverse(0.99);
    float min_var = 0.;
    float max_var = 1.5*sampling_cdf->variance();

    for( float m=min_mean; m<= max_mean; m +=( max_mean-min_mean )/ inc_mean ) {
      for( float v=min_var; v<= max_var; v +=( max_var-min_var )/ inc_var ) {
         sampling_cdf1.set_moments(m,v);
         std::vector<float>::iterator it_p = p_values.begin();
         std::vector<float>::iterator it_q = q_values.begin();
         for( ; it_p != p_values.end(); ++it_p, ++it_q ) {
          float inv_sampler = sampling_cdf1.inverse( *it_p );
          float p_global = sampling_cdf->prob(sampling_cdf1.inverse(*it_p));
          float val = global_cdf_->inverse(sampling_cdf->prob(sampling_cdf1.inverse(*it_p)));
          *it_q =  val ;
        }
//        Tail_interpolator lti = Tail_interpolator( new Power_LTI(q_values.front()) );
//        Linear_interpol mi = Linear_interpol();
//        Tail_interpolator uti = Tail_interpolator( new Power_UTI(q_values.back()) );


        Non_param_cdf<>* sampled_cdf = 
          new Non_param_cdf<>(q_values.begin(),q_values.end(),p_values.begin(),lti,mi,uti );
        sampled_cdf->make_valid();
        Moments moments(sampled_cdf->mean(),sampled_cdf->variance());
        ccdfs_.push_back( std::pair<Moments,Non_param_cdf<>*>(moments, sampled_cdf  ) );
      }
    }
  }
	  
  virtual ~Deutsch_cdf(){
//    ccdfs_vectorT::iterator it = ccdfs_.begin();
//    for( ; it =! ccdfs_.end(); ++it) delete it->second;
  }

  float mean() {return mean_;}
  const float mean() const {return mean_;}

  int set_moments(float mean, float var );

  float variance() {return var_;}
  const float variance() const {return var_;}

  virtual value_type inverse(double p) const;

  virtual double prob(value_type z) const;

  protected:
    typedef std::pair<float,float> Moments;
    typedef std::vector< std::pair<Moments,Non_param_cdf<>*> > ccdfs_vectorT;
    ccdfs_vectorT ccdfs_;
	  float var_global_;
	  float mean_global_;
	  Global_cdf* global_cdf_;
    Non_param_cdf<>* ccdf_;
    value_type global_min_;
    value_type global_max_;
    std::pair<Moments,Non_param_cdf<>*> moments_ccdf_;
  
};


template< typename Global_cdf, typename Sampling_cdf >
  inline int Deutsch_cdf<Global_cdf,Sampling_cdf>::set_moments(float mean, float var )
  {
	  if(mean < global_cdf_->inverse(0.0) ) 
		  mean_ = global_cdf_->inverse(MIN_PROB) ;
	  else if (mean > global_cdf_->inverse(1.0) ) 
		  mean_ = global_cdf_->inverse(MAX_PROB) ;
	  else 
		  mean_ = mean;
	  var_ = var;

    Search_closest_ccdf search_fctr(mean_, var_);
    ccdfs_vectorT::iterator it = std::min_element(ccdfs_.begin(), ccdfs_.end(), search_fctr );
    moments_ccdf_ = *it;
    ccdf_ = it->second;

    //ccdf_ = (std::min_element(ccdfs_.begin(), ccdfs_.end(), search_fctr ) )->second;

	  return 0;
  }

template< typename Global_cdf, typename Sampling_cdf >
First2_moments_cdf::value_type Deutsch_cdf<Global_cdf,Sampling_cdf>::inverse(double p) const {
  //value_type val = ccdf_->inverse( p );
  value_type val = moments_ccdf_.second->inverse( p );
  value_type val_cor = (val - moments_ccdf_.first.first )*std::sqrt(var_/moments_ccdf_.first.second) + mean_;
  //value_type val_cor = (val - ccdf_->mean())*std::sqrt(var_/ccdf_->variance()) + mean_;
  if( val_cor  < global_min_ ) return global_min_;
  if( val_cor  > global_max_ ) return global_max_;
  return val_cor;
}

template< typename Global_cdf, typename Sampling_cdf >
double Deutsch_cdf<Global_cdf,Sampling_cdf>::prob(value_type z) const {
  return ccdf_->prob( z );
}




#endif
