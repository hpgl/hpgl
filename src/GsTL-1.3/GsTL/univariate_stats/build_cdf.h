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

 

#ifndef __GTL_BUILD_CDF__
#define __GTL_BUILD_CDF__


#include <GsTL/cdf/cdf_basics.h>
#include <GsTL/math/math_functions.h>
#include <GsTL/utils/helper_functions.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iterator>

#include <cmath>


/** Build a cdf from range [first,last) of values, ie gives P( Z <= z0 ) 
 * for a given z0.
 * Notice the "lesser or equal to". It implies that P(Z<=z0) where z0 is the
 * smallest element of the range is NOT 0. P(Z<=z_n) where z_n is the largest
 * element of the range IS 1.
 * Range [first,last) is modified by this function call (it is sorted).
 * Version 1 of the algorithm will use a cdf discretization of size equal to
 * the size of range [first,last).
 * In version 2 of the algorithm, the discretization of the cdf has already been
 * established and the algorithm computes the probabilities corresponding to 
 * the provided discretization. 
 * @param new_cdf is the cdf into which the resulting cdf is stored.
 * It will be overwritten by the algorithm. 
 */


//______________________
// Version 1

template<class NonParamCdf, class RandomIterator>
void
build_cdf(RandomIterator first, RandomIterator last,
	  NonParamCdf& new_cdf, bool includes_max = false )
{
  int nb_of_values = last - first;
  int nb_of_thresholds = nb_of_values;
  std::sort(first, last);

  new_cdf.resize(nb_of_thresholds);

  typename NonParamCdf::z_iterator z_it = new_cdf.z_begin();
  typename NonParamCdf::p_iterator p_it = new_cdf.p_begin();

  int updated_nb_of_thresholds = nb_of_thresholds;

  if( !includes_max ) nb_of_values++;

  // initialize the first element of the cdf
  *z_it = *first;
  *p_it = 1.0 / double(nb_of_values) ;
  z_it++; p_it++;  
  typename std::iterator_traits<RandomIterator>::value_type previous = *first;
  int position = 1;

  for( int i = 2; i <= nb_of_thresholds; i++ ) {
    *z_it = *(first + position) ;
    if( *z_it == previous ) {
      z_it--;
      p_it--;
      updated_nb_of_thresholds--;
    }
    *p_it = double(position+1) / double(nb_of_values) ;
    
    position++ ;
    previous = *z_it;
    z_it++;
    p_it++;
  }

  new_cdf.resize( updated_nb_of_thresholds );
  
}



/*
template<class NonParamCdf, class RandomIterator>
void
build_cdf(RandomIterator first, RandomIterator last,
	  NonParamCdf& new_cdf, int nb_of_thresholds)
{
  int range_size = last - first;
  nb_of_thresholds = std::min( range_size, nb_of_thresholds);

  std::sort(first, last);
  
  
  if(nb_of_thresholds <= 0) {
    nb_of_thresholds = range_size;
  }
  new_cdf.resize(nb_of_thresholds+1);
  
  double bin_width = ( *(last-1) - *first ) / double(nb_of_thresholds);

  typename NonParamCdf::z_iterator z_it = new_cdf.z_begin();
  typename NonParamCdf::p_iterator p_it = new_cdf.p_begin();
  RandomIterator previous = last;
  int updated_nb_of_thresholds = nb_of_thresholds+1;
  

  for(int i = 0; i < nb_of_thresholds+1; i++) {
    double z = (*first) + i*bin_width;

    // find where z is in the range. If z is not in the range, 
    // change z to the closest range element
    RandomIterator position = std::upper_bound( first, last, z );
    if( *(position-1) == z )
      position--;

    if( position == previous ) {
      updated_nb_of_thresholds--;
      continue;
    }

    *z_it = *position;
    *p_it = double( position-first + 1 ) / double(range_size);

    ++z_it;
    ++p_it;
    previous = position;
  }
  
  new_cdf.resize( updated_nb_of_thresholds );
}
*/

//_____________________
// Version 2

/** @param [z_begin, z_end) is the cdf discretization
 * @param p_begin is an iterator to the range of the cdf probabilities
 */
template <
  class RandomIterator,
  class CdfZIterator,
  class CdfPIterator>
void
build_cdf(RandomIterator first, RandomIterator last,
	  CdfZIterator z_begin, CdfZIterator z_end,
	  CdfPIterator p_begin, bool include_max = false )
{
  int nb_of_values = last - first;
  std::sort(first, last);

  if( !include_max ) nb_of_values++;

  for( ; z_begin != z_end; z_begin++)
    {
      RandomIterator position = std::upper_bound( first, last, *z_begin );
      if( *(position-1) == *z_begin )
	position--;

      int rank = position - first + 1;
      *p_begin = double(rank) / double(nb_of_values);
      p_begin ++ ;
    }
}




//===================================================================


/** Build a cdf from a range of values. 
 * Contrary to build_cdf, this function does not modify the range of values.
 */


//______________________
// Version 1

template<class NonParamCdf, class InputIterator>
void
build_cdf_copy( InputIterator first, InputIterator last,
		NonParamCdf& new_cdf ) {

  // Copy the elements in [first, last). 
  typedef typename std::iterator_traits<InputIterator>::value_type Value_type;
/*
  int size = static_cast<int>( std::distance(first, last) );
  std::vector<Value_type> range_copy(size);

  std::copy(first, last, range_copy.begin());
*/
  
  std::vector<Value_type> range_copy;
  std::copy( first, last, std::back_inserter( range_copy ) );

  build_cdf( range_copy.begin(), range_copy.end(), 
	     new_cdf );
}



//______________________
// Version 2

template <
  class InputIterator,
  class CdfZIterator,
  class CdfPIterator>
void
build_cdf_copy( InputIterator first, InputIterator last,
		CdfZIterator z_begin, CdfZIterator z_end,
		CdfPIterator p_begin ) {

  // Copy the elements in [first, last). 
  typedef typename std::iterator_traits<InputIterator>::value_type value_type;
/* 
 int size = static_cast<int>( std::distance(first, last) );
  std::vector<value_type> range_copy(size);

  std::copy(first, last, range_copy.begin());
  */

  std::vector<value_type> range_copy;
  std::copy( first, last, std::back_inserter( range_copy ) );
  
  build_cdf( range_copy.begin(), range_copy.end(), 
	     z_begin, z_end, p_begin);
}





/** Build a cdf with tie breaking from range [first,last) of values, ie gives P( Z <= z0 ) 
 * for a given z0.
 * Notice the "lesser or equal to". It implies that P(Z<=z0) where z0 is the
 * smallest element of the range is NOT 0. P(Z<=z_n) where z_n is the largest
 * element of the range IS 1.
 * Range [first,last) is modified by this function call (it is sorted).
 * Version 1 of the algorithm will use a cdf discretization of size equal to
 * the size of range [first,last).
 */

template<class NonParamCdf, class RandomIterator>
void
build_cdf_tie_break(RandomIterator first, RandomIterator last,
	  NonParamCdf& new_cdf, bool includes_max = false )
{
  int nb_of_values = last - first;
  int nb_of_thresholds = nb_of_values;
  std::sort(first, last);

  new_cdf.resize(nb_of_thresholds);

  typename NonParamCdf::z_iterator z_it = new_cdf.z_begin();
  typename NonParamCdf::p_iterator p_it = new_cdf.p_begin();

  if( !includes_max ) nb_of_values++;
  for(RandomIterator it = first ; it != last; ++it,++z_it, ++p_it ) {
    *p_it = (double)(std::distance(first,it)+1)/(double)nb_of_values;
    *z_it = *it;
  }
 
}





//=====================================================================


//______________________
// Build a cdf from a pdf
/** This function builds a pdf from a cdf.
 * @param pdf is the pdf into which the result is to be stored. It need not
 * contain the correct discretization.
 * @param cdf is the cdf to transform.
 */
template<class NonParamPdf, class NonParamCdf>
void cdf_to_pdf(NonParamPdf& pdf, const NonParamCdf& cdf) {
  if(pdf.size() != cdf.size())
    pdf.resize(cdf.size());

  std::copy(cdf.z_begin(), cdf.z_end(), pdf.z_begin());
  std::adjacent_difference(cdf.p_begin(), cdf.p_end(), pdf.p_begin());
}


//_______________________
// Build a pdf from a cdf
/** This function builds a cdf from a pdf.
 * @param cdf is the cdf into which the result is to be stored. It need not
 * contain the correct discretization.
 * @param pdf is the pdf to transform.
 */
template<class NonParamPdf, class NonParamCdf>
void pdf_to_cdf(NonParamCdf& cdf, const NonParamPdf& pdf) {
  typename NonParamCdf::variable_category dummy;
  pdf_to_cdf( cdf, pdf, dummy );
}

template<class NonParamPdf, class NonParamCdf>
void pdf_to_cdf( NonParamCdf& cdf, const NonParamPdf& pdf, 
                GsTL::discrete_variable_tag ) {
  if(pdf.size() != cdf.size())
    cdf.resize( pdf.size() );

  std::copy(pdf.z_begin(), pdf.z_end(), cdf.z_begin());
  std::copy(pdf.p_begin(), pdf.p_end(), cdf.p_begin());
}
 
template<class NonParamPdf, class NonParamCdf>
void pdf_to_cdf( NonParamCdf& cdf, const NonParamPdf& pdf, 
                GsTL::continuous_variable_tag ) {
  if(pdf.size() != cdf.size())
    cdf.resize( pdf.size() );

  std::copy(pdf.z_begin(), pdf.z_end(), cdf.z_begin());
  std::partial_sum(pdf.p_begin(), pdf.p_end(), cdf.p_begin());
}

//=====================================================================




/** Check whether range [begin,end) defines a valid cdf. This function
 * does not apply to cdf's of categorical variables because it allows the
 * last probability to be lesser than 1. For a continuous variable, the 
 * last probability can be lesser than 1 because the last threshold is not 
 * necessarily the maximum value of the variable.
 */
template <class ForwardIterator>
bool is_valid_cdf( ForwardIterator begin, ForwardIterator end,
		   GsTL::continuous_variable_tag ) {

  if( begin == end ) return true;

  if( *begin < 0 || *begin > 1 ) return false;
  ForwardIterator last = begin;
  std::advance( last, std::distance( begin, end ) - 1 );
  if( *last > 1 ) return false;

  return GsTL::is_sorted( begin, end );
}



/** Check whether range [begin,end) defines a valid cdf. This function
 * does not apply to cdf's of categorical variables because it allows the
 * last probability to be lesser than 1. For a continuous variable, the 
 * last probability can be lesser than 1 because the last threshold is not 
 * necessarily the maximum value of the variable.
 */
template <class ForwardIterator>
bool is_valid_cdf( ForwardIterator begin, ForwardIterator end,
		   GsTL::discrete_variable_tag ) {

  return is_valid_pdf( begin, end );
}




/** Check whether a cdf is valid or not.
 */
template <class NonParametricCdf>
bool is_valid_cdf( const NonParametricCdf& cdf ) {
  return is_valid_cdf( cdf.p_begin(), cdf.p_end(),
  		       typename NonParametricCdf::variable_category() );
}




/** Modify range [begin,end) so that it defines the probability values
 * of a valid cdf (ie the values are in [0,1] and are sorted in ascending order).
 */ 
template <class RandomIterator>
bool make_cdf_valid( RandomIterator begin, RandomIterator end,
		     GsTL::continuous_variable_tag tag ) {

  if( is_valid_cdf( begin, end, tag ) ) {
    // no corrections needed
    return true;
  }

  int size = std::distance( begin, end );

  // Allocate temporary buffers
  std::vector<double> upward_correction( size );
  std::vector<double> downward_correction( size );

  // Upward correction 
  upward_correction[0] = std::max (std::min   (begin[0], 1.0), 0.0);
  for( int i=1; i< size ; i++ ) 
    upward_correction[i] = std::max (std::min   (begin[i],1.0), upward_correction[i-1]);
  
  // Downward correction
  downward_correction[size-1] = std::max (std::min   (begin[size-1], 1.0), 0.0);
  for(int j=size-2; j>=0 ; j--) 
    downward_correction[j] = std::min (std::max   (begin[j], 0.0), downward_correction[j+1]);
  
  // final correction
  for(int k=0; k<size ; k++)
    begin[k] = (downward_correction[k] + upward_correction[k]) / 2.0 ; 

  return true;
}



/** This version of make_cdf_valid works on categorical variables.
 * It modifies range [begin,end) so that it defines the probability values
 * of a valid cdf: range [begin,end) is interpreted as defining the probabilities
 * of belonging to classes c_0 to c_k (k is the size of range [begin,end) ), 
 * in other words, range [begin,end) is a (not yet valid) PDF.
 */ 
template <class RandomIterator>
bool make_cdf_valid( RandomIterator begin, RandomIterator end,
		     GsTL::discrete_variable_tag tag ) {

 return make_pdf_valid( begin, end );        
}



/** Turn a cdf into a valid cdf.
 */ 

template <class NonParametricCdf>
bool make_cdf_valid( NonParametricCdf& cdf ) {
  return make_cdf_valid( cdf.p_begin(), cdf.p_end(), 
			 typename NonParametricCdf::variable_category() );
}




/** make_pdf_valid modifies range [begin,end) so that it defines the 
 * probability values of a valid pdf
 */ 
template <class RandomIterator>
bool make_pdf_valid( RandomIterator begin, RandomIterator end ) {
  
  if( is_valid_pdf( begin, end ) ) {
    // no corrections needed
    return true;
  }

  // Make all values > 0 and standardize so that they sum-up to 1.
  for( RandomIterator it = begin; it != end; ++it ) 
    *it = std::max ( *it, 0.0 );

  double sum = std::accumulate( begin, end, 0.0 );
  if( sum == 0 ) return false;

  for( RandomIterator it2 = begin; it2 != end; ++it2 ) 
    *it2 = *it2 / sum;

  return true;
}



/** Check whether range [begin,end) defines a valid pdf. 
 */
template <class ForwardIterator>
bool is_valid_pdf( ForwardIterator begin, ForwardIterator end ) {

  double sum = std::accumulate( begin, end, 0.0 );
  if( !GsTL::equals( sum, 1.0, 0.00001 ) )
    return false;

  for( ; begin != end; ++begin ) 
    if( *begin < 0 || *begin > 1 ) return false;

  return true;
}

#endif
