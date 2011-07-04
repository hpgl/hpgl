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

#ifndef __GSTL_CDF_ESTIMATOR_CI_BAYES_UPDATING_H__
#define __GSTL_CDF_ESTIMATOR_CI_BAYES_UPDATING_H__


#include <GsTL/univariate_stats.h>
#include <GsTL/cdf/non_param_pdf.h>
#include <GsTL/math/math_functions.h>


#include <vector>
#include <algorithm>
#include <iterator>


/** A CI_Bayes_updating is an adaptor that models concept Cdf Estimator.
 * Its aim is to update the probability P(A|B) computed by some cdf estimator 
 * into the joint probability P(A|B,C), given the prior knowledge P(A|C).
 * For example, P(A|B) could be the probability of observing channel given some
 * well data as computed by a search tree, and P(A|C) the probability of 
 * observing channel given some seismic information, eg obtained by classification.
 * P(A|B,C) is the probability of observing a channel given both the
 * well data and the seismic signature.
 *
 * The two probabilities P(A|B) and P(A|C) are combined using the permanence of 
 * ratios formula proposed by Journel (2000), which amounts to a conditional
 * independance hypothesis (hence the name: Conditional Independance Bayes 
 * updating).
 *
 * A CI_Bayes_updating requires the knowledge of probabilities 
 * P(A|C) for every category (i.e. every possible outcome of A). Actually, if A
 * has k possible outcomes, only (k-1) probabilities are needed, the probability
 * corresponding to category k is deducted by complement: 
 *    P(A=k|B,C) = sum[i=1 to k-1] P(A=i|B,C) 
 */

template< class NonParamCdf, class PropertyMap >
class CI_Bayes_updating {

 public:
   typedef typename PropertyMap::key_type Geovalue_;
   typedef Geovalue_   first_argument;
   typedef NonParamCdf second_argument;
   typedef NonParamCdf return_type;
   
  /** Constructing a CI_Bayes_updating requires a way of estimating P(A|B),
   * a map of probabilities P(A|C) for each category, and the marginal distribution
   * P(A). A map of probabilities is a model of PropertyMap, with Geovalues
   * as keys, and P(A|C) as values.
   *
   * @param cdf_estimator a model of concept Cdf Estimator. It knows how to 
   * compute the probabilities P(A|B).
   * @param range [first, last) is a range of probability maps.
   * @param marginal is the marginal distribution P(A).
   */
  template< class PropertyMapIterator, class NonParamCdf2 >
  CI_Bayes_updating( PropertyMapIterator first, PropertyMapIterator last,
                     const NonParamCdf2& marginal) {
    std::copy( first, last, std::back_inserter( property_maps_ ) );
    
    typename NonParamCdf2::const_p_iterator it = marginal.p_begin();
    for( ; it!= marginal.p_end() ; ++it ) 
      a_.push_back( (1.0 - (*it)) / *it );
  }

  /** computes the probabilities P(A|B(u),C(u)). The resulting cdf is stored into ccdf.
   */
/*  inline return_type operator()( const first_argument& g,
		                 const second_argument& ccdf ) {
   
    NonParamCdf updated_cdf( ccdf ); 
    const double epsilon = 0.000000001;
    typedef typename NonParamCdf::value_type value_type;

    // convert updated_cdf into a pdf
    Non_param_pdf<value_type> P_AB(updated_cdf);

    // Compute x=bc/a for each category and store the values in x_
    std::vector<double> x_(updated_cdf.size());

    Non_param_pdf<value_type>::p_iterator p_ab_it = P_AB.p_begin();
    int i = 0;
    for( PmapIterator it = property_maps_.begin() ;
         it != property_maps_.end() ; 
         ++it, ++i, ++p_ab_it) {
      // x = b * c / a
      double p_ab = std::max( double(*p_ab_it), epsilon );
      double p_ac = std::max( double( get( *it, g ) ), epsilon );

      double b = (1 - p_ab)/p_ab;
      double c = (1 - p_ac)/p_ac;
      x_[i] = b * c / a_[i];
    }

    // recover P(A|B,C) from the values of x.
    Non_param_pdf<value_type> P_ABC(P_AB);
    i =0 ;
    for( Non_param_pdf<value_type>::p_iterator p_abc_it = P_ABC.p_begin();
         p_abc_it!= P_ABC.p_end() ;
         ++p_abc_it, ++i ) {
      // p(A|B,C) = 1/(1+x)
      *p_abc_it = 1.0 / std::max( double(1.0 + x_[i]), epsilon );
    }

    // turn the pdf P(A|B,C) into a cdf and put the result into updated_cdf
    make_pdf_valid( P_ABC.p_begin(), P_ABC.p_end() );
    pdf_to_cdf(updated_cdf, P_ABC);

    return updated_cdf;
  }
  */

  /** computes the probabilities P(A|B(u),C(u)). The resulting cdf is stored into ccdf.

   */

  inline return_type operator()( const first_argument& g,

		                 const second_argument& ccdf ) {



    typedef typename NonParamCdf::value_type value_type;

   

    const double infinity= -99e30;

    NonParamCdf updated_cdf( ccdf ); 



    // convert updated_cdf into a pdf

    Non_param_pdf<value_type> P_AB(updated_cdf);



    // Compute x=bc/a for each category and store the values in x_

    std::vector<double> x_(updated_cdf.size());



    typename Non_param_pdf<value_type>::p_iterator p_ab_it = P_AB.p_begin();

    int i = 0;

    for( PmapIterator it = property_maps_.begin() ;

         it != property_maps_.end() ; 

         ++it, ++i, ++p_ab_it) {

      // x = b * c / a

      double p_ab = *p_ab_it;

      double p_ac = get( *it, g );


      if( GsTL::equals( p_ab, double(0) ) || GsTL::equals( p_ac, double(0) ) ||

          GsTL::equals( a_[i], double(0) ) ) {
        x_[i] = infinity;
        continue;
      }

      double b = (1 - p_ab)/p_ab;
      double c = (1 - p_ac)/p_ac;
      x_[i] = b * c / a_[i];
    }

    // recover P(A|B,C) from the values of x.

    Non_param_pdf<value_type> P_ABC(P_AB);

    i =0 ;

    for( typename Non_param_pdf<value_type>::p_iterator p_abc_it = P_ABC.p_begin();
         p_abc_it!= P_ABC.p_end() ;

         ++p_abc_it, ++i ) {

      // p(A|B,C) = 1/(1+x)

      if( GsTL::equals( x_[i], infinity ) ) {

        *p_abc_it = 0.0;

        continue;

      }

      *p_abc_it = 1.0 / double(1.0 + x_[i]);

    }



    // turn the pdf P(A|B,C) into a cdf and put the result into updated_cdf

    bool ok = make_pdf_valid( P_ABC.p_begin(), P_ABC.p_end() );



	// It is possible that the updated cdf is not valid. For example 

	// if p(a=0|b)=1 while p(a=0|c)=0 and p(a=1|b)=0 while p(a=1|c)=1,

	// then p(a=0|b,c)=0 and p(a=1|b,c)=0. In that case, return the marginal 

	if( !ok ) {

		int j =0;
		typename Non_param_pdf<value_type>::p_iterator p_abc_it;
		for( p_abc_it = P_ABC.p_begin(); p_abc_it!= P_ABC.p_end() ;
		     ++p_abc_it, ++j ) {
			// p(A) = 1/(1+a)

			*p_abc_it = 1.0 / double(1.0 + a_[j]);

		}

	}

	pdf_to_cdf(updated_cdf, P_ABC);

    return updated_cdf;

  }

 private:
  typedef typename std::vector<PropertyMap>::iterator PmapIterator;
  typename std::vector<PropertyMap> property_maps_;
  
  std::vector<double> a_;

}; // end of class CI_Bayes_updating



#endif

