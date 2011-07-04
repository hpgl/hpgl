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

 

#ifndef __GSTL_NON_PARAM_PDF_H__
#define __GSTL_NON_PARAM_PDF_H__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/utils/gstl_error_messages.h>

#include <vector>
#include <algorithm>
#include <numeric>

/** The z-values must be ordered.
 */
template<class T>
class Non_param_pdf{

  public:
  
  typedef T value_type;
  typedef typename std::vector<value_type>::iterator z_iterator;
  typedef std::vector<double>::iterator     p_iterator;
  
  typedef typename std::vector<value_type>::const_iterator const_z_iterator;
  typedef std::vector<double>::const_iterator     const_p_iterator;
  

  //-----------------
  // constructors 

  Non_param_pdf() {};

  template<class Z_iterator>
  Non_param_pdf(Z_iterator z_begin, Z_iterator z_end) {
    for(Z_iterator it=z_begin; it != z_end ; it++)
      z_values_.push_back(*it);
    
    p_values_.resize(z_values_.size());
  };


  template<class Z_iterator, class P_iterator>
  Non_param_pdf(Z_iterator z_begin, Z_iterator z_end,
		P_iterator p_begin) {
    P_iterator p_it = p_begin;
    for(Z_iterator it=z_begin; it != z_end ; it++)
      {
	z_values_.push_back(*it);
	p_values_.push_back(*p_it);
	p_it++;
      }
  };

  template<class NonParamCdf>
  Non_param_pdf(const NonParamCdf& cdf) 
    : z_values_( cdf.size() ), p_values_( cdf.size() ) {
    typename NonParamCdf::variable_category a;
    create_pdf( cdf, a );
  }
 
protected: 
  template<class NonParamCdf>
  void create_pdf(const NonParamCdf& cdf, GsTL::continuous_variable_tag ) {
    std::copy(cdf.z_begin(), cdf.z_end(), z_values_.begin());
    std::adjacent_difference(cdf.p_begin(), cdf.p_end(), p_values_.begin());
  }
  
  // the "cdf" for a discrete variable is actually a pdf, so there is 
  // nothing to do 
  template<class NonParamCdf>
  void create_pdf(const NonParamCdf& cdf, GsTL::discrete_variable_tag ) {
    std::copy(cdf.z_begin(), cdf.z_end(), z_values_.begin());  
    std::copy(cdf.p_begin(), cdf.p_end(), p_values_.begin());  
  }


public:

  //-----------------

  inline void resize(unsigned int m);
  
  /** Returns the size of the discretizations, i.e. the number of z-values used
   * to characterize the distribution function.
   */
  inline int size() const;

  template<class forward_iterator>
  inline void z_set(forward_iterator z_begin, forward_iterator z_end);
 
  template<class forward_iterator>
  inline void p_set(forward_iterator p_begin, forward_iterator p_end);
 
  inline z_iterator z_begin() {return z_values_.begin();}
  inline z_iterator z_end() {return z_values_.end();}
  
  inline p_iterator p_begin() {return p_values_.begin();}
  inline p_iterator p_end() {return p_values_.end();}
  

  inline const_z_iterator z_begin() const {return z_values_.begin();}
  inline const_z_iterator z_end() const {return z_values_.end();}
  
  inline const_p_iterator p_begin() const {return p_values_.begin();}
  inline const_p_iterator p_end() const {return p_values_.end();}

  
  inline double prob(value_type z) const;



  private:

  std::vector<value_type>  z_values_;
  std::vector<double>      p_values_;

}; // end of class Non_param_pdf



//___________________________
// Inline functions

template<class T>
template<class forward_iterator>
inline void Non_param_pdf<T>::z_set(forward_iterator z_begin, 
				    forward_iterator z_end) {
  p_values_.clear();
  z_values_.resize(z_end - z_begin);
  p_values_.resize(z_values_.size());

  int i=0;
  
  for(forward_iterator it = z_begin; it!=z_end; it++)
    z_values_[i++] = *it;
  
}



template<class T>
template<class forward_iterator>
inline void Non_param_pdf<T>::p_set(forward_iterator p_begin, 
				    forward_iterator p_end) {
  gstl_assert(p_end - p_begin <= p_values_.size());
  int i=0;
  for(forward_iterator it = p_begin; it!=p_end; it++)
    p_values_[i++] = *it;
    
}



template<class T>
inline void Non_param_pdf<T>::resize(unsigned int m) {
  z_values_.resize(m);
  p_values_.resize(m);
}


template<class T>
inline int Non_param_pdf<T>::size() const {
  return static_cast<int>(z_values_.size());
}


//_________________________________________
// Definition of member function "prob"

template<class T>
inline double Non_param_pdf<T>::prob(value_type z) const {
 
  typedef typename std::vector<value_type>::const_iterator const_iterator;

  // use STL lower_bound algo to where in the z-range, z should be placed
  const_iterator pos = std::lower_bound(z_values_.begin(),z_values_.end(),z);
  
  gstl_assert( pos != z_values_.end() );
 
  return p_values_[pos - z_values_.begin()];
  
}




#endif
