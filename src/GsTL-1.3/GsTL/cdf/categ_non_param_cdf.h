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

 

#ifndef __GSTL_CATEG_NON_PARAM_CDF__
#define __GSTL_CATEG_NON_PARAM_CDF__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/cdf/cdf_basics.h>
#include <GsTL/univariate_stats/build_cdf.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <vector>
#include <algorithm>

/** It doesn't make much sense to talk about a cdf for a categorical variable.
 * If we transpose the definition of the cdf of a continuous variable to the
 * discrete case, we get F( z, z_k ) = Prob( z = z_k ), which is a PDF.
 * Categ_non_param_cdf implements that function F. It is called cdf nevertheless
 * because it is a model of concept Cdf: it has a prob function that returns
 * F(z=z_k ) and an  inverse function which, given a probability p, returns
 * a class at "random".
 * The vector of probabilities contained in Categ_non_param_cdf is Prob(z=z_k),
 * for k=0 to K (K is the total number of classes)
 */
template<class T = unsigned int>
class Categ_non_param_cdf : public Non_parametric_cdf<T> {

  public:
  typedef typename Non_parametric_cdf<T>::value_type value_type;
  typedef GsTL::discrete_variable_tag  variable_category;
  
  //-----------------
  // constructors 

  Categ_non_param_cdf() {}

  Categ_non_param_cdf( unsigned int categories_count ) 
  {
    this->z_values_.resize( categories_count );
    for( unsigned int i = 0; i < categories_count; i++ )
      this->z_values_[i] = i;
    
    this->p_values_.resize( categories_count );
  };


  template< class P_iterator >
  Categ_non_param_cdf( unsigned int categories_count,
		       P_iterator p_begin) 
  {
    this->z_values_.resize( categories_count );
    this->p_values_.resize( categories_count );

    for( unsigned int i = 0; i < categories_count; i++ )
      {
	this->z_values_[i] = i;
	this->p_values_[i] = *p_begin;
	p_begin++;
      }
  };


  ~Categ_non_param_cdf() {}

  //-----------------
  inline void z_set( unsigned int categories_count );
 
  template<class forward_iterator>
  inline void p_set(forward_iterator p_begin, forward_iterator p_end);
 
  virtual bool make_valid() { return make_cdf_valid( *this); } 
  virtual double prob(value_type z) const;
  virtual value_type inverse(double p) const;

}; // end of class Categ_non_param_cdf



//___________________________
// Inline functions

template<class T>
void Categ_non_param_cdf<T>::z_set( unsigned int categories_count )
{
  this->p_values_.clear();
  this->z_values_.resize( categories_count );
  this->p_values_.resize( categories_count );

  for( unsigned int i = 0; i < categories_count; i++ )
    this->z_values_[i] = i;
}



template<class T>
template<class forward_iterator>
void Categ_non_param_cdf<T>::p_set(forward_iterator p_begin, 
				   forward_iterator p_end)
{
  unsigned int categories_count = std::distance( p_begin, p_end );
  gstl_assert( this->p_values_.size() == categories_count );
  
  int i=0;
  for(forward_iterator it = p_begin; it!=p_end; it++) {
    gstl_assert( i < static_cast<int>( this->p_values_.size()) );
    this->p_values_[i++] = *it;
  }
}


template<class T>
double Categ_non_param_cdf<T>::prob(value_type z) const
{
  gstl_assert( is_valid_cdf( *this ) );

  unsigned int categ = static_cast<unsigned int>( z );
  gstl_assert( categ < this->p_values_.size() );
  return this->p_values_[ categ ];
  
}



template<class T>
typename Categ_non_param_cdf<T>::value_type
Categ_non_param_cdf<T>::inverse(double p) const
{
  gstl_assert( is_valid_cdf( *this ) );

  int clas = -1;
  while( p > 0 && clas+1 != int(this->p_values_.size()) ) {
    ++clas;
    p -= this->p_values_[clas];
  }

  return clas;
}


#endif
