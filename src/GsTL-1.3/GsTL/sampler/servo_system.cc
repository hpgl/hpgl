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


#include <GsTLAppli/grid/grid_model/rgrid.h>

#include <GsTL/utils/gstl_error_messages.h>

template < class RandNumberGenerator >
template < class ForwardIterator, class CategNonParamCdf >
Servo_system_sampler< RandNumberGenerator >::
Servo_system_sampler(
		     const CategNonParamCdf& target,
		     double constraint,
		     ForwardIterator first, ForwardIterator last,
		     const RandNumberGenerator& rand
		     ) 
  : target_pdf_( target.p_begin(), target.p_end() ),
    current_histogram_( target.size(), 0 ),
    gen_(rand) {

  // constraint can vary from 0 to 1
  mu_ = constraint / ( 1.00001 - constraint);

  nb_of_categories_ = target.size();
  nb_of_data_ = 0;

  // Compute the pdf of range [first, last)
  for( ; first != last ; ++first) {
    if( ! first->is_informed() ) continue;

    current_histogram_[ int( first->property_value() ) ] ++;
    nb_of_data_ ++;

  }
}



template < class RandNumberGenerator >
template < class CategNonParamCdf >
Servo_system_sampler< RandNumberGenerator >::
Servo_system_sampler(
		     const CategNonParamCdf& target,
		     double constraint,
		     const std::vector<double>& initial_histogram,
		     const RandNumberGenerator& rand
		     ) 
  : target_pdf_( target.p_begin(), target.p_end() ),
    current_histogram_( initial_histogram ),
    gen_(rand) {

  // constraint can vary from 0 to 1
  mu_ = constraint / ( 1.00001 - constraint);

  nb_of_categories_ = target.size();
  nb_of_data_ = std::accumulate( initial_histogram.begin(),
				 initial_histogram.end(), 0 );
}



template < class RandNumberGenerator >
Servo_system_sampler< RandNumberGenerator >::
Servo_system_sampler( const Servo_system_sampler<RandNumberGenerator>& rhs ) 
  : nb_of_categories_( rhs.nb_of_categories_ ),
    target_pdf_( rhs.target_pdf_ ),
    current_histogram_( rhs.current_histogram_ ),
    nb_of_data_( rhs.nb_of_data_ ),
    mu_( rhs.mu_ ),
    gen_( rhs.gen_ ) {

}



template < class RandNumberGenerator >
template < class GeoValue, class CategNonParamCdf >
int 
Servo_system_sampler< RandNumberGenerator >::
operator () ( GeoValue& gval, const CategNonParamCdf& ccdf ) {

  typedef typename CategNonParamCdf::value_type value_type;
  typedef typename CategNonParamCdf::p_iterator p_iterator;

  const double tolerance = 0.01;
  CategNonParamCdf corrected_ccdf( ccdf );

  // Don't try to correct the ccdf if nb_of_data_ = 0
  if( nb_of_data_ != 0 ) {

    // Correct each probability value of the cpdf
    int i=0;
    for( p_iterator p_it=corrected_ccdf.p_begin() ; 
	 p_it != corrected_ccdf.p_end(); ++p_it, ++i) {
    
      // If the probability is extreme (ie close to 0 or 1), don't touch it
      if(*p_it < tolerance || *p_it > 1-tolerance) continue; 

      // Correct each probability 
      *p_it += mu_ * (target_pdf_[i] - current_histogram_[i]/nb_of_data_);
    
      // reset the probability between 0 and 1 if needed.
      *p_it = std::max(*p_it, 0.0);
      *p_it = std::min(*p_it, 1.0);
      
    }

    // corrected_ccdf now contains a pdf which may not be valid, i.e. it is not 
    // garanteed that the probabilities add-up to 1.
    corrected_ccdf.make_valid();
  }


  // Draw a realization from ccdf and update the servo system
  // A comment about types: "realization" should be of integral type (eg int)
  // but GeoValue::property_type could be different (eg float).
  typedef typename CategNonParamCdf::value_type value_type;
  typedef typename GeoValue::property_type property_type;
  value_type realization = corrected_ccdf.inverse( gen_() );
				
  gval.set_property_value( static_cast<property_type>(realization) );
  nb_of_data_ ++;
  gstl_assert( realization>=0  &&  
	  realization < static_cast<int>(current_histogram_.size()) );
  current_histogram_[ realization ] ++;
  
  // for testing only
  //cout << "simulated node location " << gval.node_id() << endl;
  //print();

  return 0;
}




/*
template 
<
  class CategNonParamCdf,
  class RandNumberGenerator
>
template <class GeoValue, class CategNonParamCdf2>
int 
Servo_system_sampler
<
  CategNonParamCdf,
  RandNumberGenerator
>::
operator()(GeoValue& gval, const CategNonParamCdf2& ccdf) {

  typedef typename CategNonParamCdf2::value_type value_type;
  typedef typename Non_param_pdf<value_type>::p_iterator p_iterator;

  const double tolerance = 0.01;
  CategNonParamCdf2 corrected_ccdf( ccdf );

  // Don't try to correct the ccdf if nb_of_data_ = 0
  if( nb_of_data_ != 0 ) {

    // Build a cpdf from ccdf
    Non_param_pdf<value_type> cpdf(ccdf);

    // Correct each probability value of the cpdf
    int i=0;
    for( p_iterator p_it=cpdf.p_begin() ; p_it != cpdf.p_end(); ++p_it, ++i) {
    
      // If the probability is extreme (ie close to 0 or 1), don't touch it
      if(*p_it < tolerance || *p_it > 1-tolerance) continue; 

      // Correct each probability 
      *p_it += mu_ * (target_pdf_[i] - current_histogram_[i]/nb_of_data_);
    
      // reset the probability between 0 and 1 if needed.
      *p_it = std::max(*p_it, 0.0);
      *p_it = std::min(*p_it, 1.0);
      
    }


    // cpdf now contains a pdf which may not be valid, i.e. it is not garanteed that
    // the probabilities add-up to 1. Correct that and turn cpdf into a cdf.

    double sum = std::accumulate(cpdf.p_begin(), cpdf.p_end(), 0.0);
    std::for_each(
		  cpdf.p_begin(), cpdf.p_end(), 
		  std::bind2nd(divides<double>(), sum)
		  );
    
    pdf_to_cdf( corrected_ccdf, cpdf );
  }


  // Draw a realization from ccdf and update the servo system
  // A comment about types: "realization" should be of integral type (eg int)
  // but GeoValue::property_type could be different (eg float).
  typedef typename CategNonParamCdf2::value_type value_type;
  typedef typename GeoValue::property_type property_type;
  value_type realization = corrected_ccdf.inverse( gen_() );
				
  gval.set_property_value( static_cast<property_type>(realization) );
  nb_of_data_ ++;
  gstl_assert( realization>=0  &&  
	  realization < static_cast<int>(current_histogram_.size()) );
  current_histogram_[ realization ] ++;
  

  return 0;
}

*/


template < class RandNumberGenerator >
void Servo_system_sampler< RandNumberGenerator >::
removeSimulatedNode( GsTLGridProperty* prop, vector<int>& grid_path )
{
    int current_value; 

    for (int i=0; i<grid_path.size(); i++)
    {
        current_value = prop->get_value( grid_path[i] );
        prop->set_not_informed( grid_path[i] );

		current_histogram_[ current_value ] --;
		nb_of_data_ --;
    }
}


template < class RandNumberGenerator >
void Servo_system_sampler< RandNumberGenerator >::
removeSimulatedNode( RGrid* grid, GsTLGridProperty* prop, vector<int>& grid_path )
{
    SGrid_cursor  cursor = SGrid_cursor( *(grid->cursor()) );
    int current_value; 
    int node_id;

    for (int i=0; i<grid_path.size(); i++)
    {
        node_id = cursor.node_id( grid_path[i] );
        current_value = prop->get_value( node_id );
        prop->set_not_informed( node_id );

		current_histogram_[ current_value ] --;
		nb_of_data_ --;
    }
}

// for testing only
template < class RandNumberGenerator >
void Servo_system_sampler< RandNumberGenerator >::print()
{
    cout << " nb_data = " << nb_of_data_  << ",         prop of facies are:  ";
    for (int i=0; i<nb_of_categories_; i++)
        cout << current_histogram_[i] << " , ";

    cout << endl;
}
#include <GsTL/sampler/servo_system.h>



