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



// Definitions of operator() for the different covariance models


//--------------------
// Spherical covariance

template<class Location>
typename Spherical_covariance<Location>::result_type 
Spherical_covariance<Location>::
compute(const typename Spherical_covariance<Location>::EuclideanVector& vec) const{
  double h=euclidean_norm(vec);

  if( h >= this->a_ ) 
    return 0;
  else
    return this->c_ * (1 -  1.5*h/this->a_ + 0.5*pow(h/this->a_ , 3));
}

template<class Location>
typename Spherical_covariance<Location>::result_type 
Spherical_covariance<Location>::isotrop_compute(double h) const{
  if( h >= this->a_ ) 
    return 0.0;
  else
    return this->c_ * (1 -  1.5*h/this->a_ + 0.5*pow(h/this->a_ , 3));
}


//--------------------
// Exponential covariance

template<class Location>
typename Exponential_covariance<Location>::result_type 
Exponential_covariance<Location>::compute(const typename Exponential_covariance<Location>::
					        EuclideanVector& vec) const{
  double h=euclidean_norm(vec);
  return this->c_ * exp(-3*h/this->a_) ;
}
 
template<class Location>
typename Exponential_covariance<Location>::result_type 
Exponential_covariance<Location>::isotrop_compute(double h) const{
  return this->c_ * exp(-3*h/this->a_) ;
}

//--------------------
// Gaussian covariance

template<class Location>
typename Gaussian_covariance<Location>::result_type 
Gaussian_covariance<Location>::compute(const typename Gaussian_covariance<Location>::
				             EuclideanVector& vec) const{
  double h=euclidean_norm(vec);
  return this->c_ * exp( -3*pow(h/this->a_ ,2) );
}

template<class Location>
typename Gaussian_covariance<Location>::result_type 
Gaussian_covariance<Location>::isotrop_compute(double h) const{
  return this->c_ * exp( -3*pow(h/this->a_ ,2) );
}



//---------------------
// Anisotropy 2D

//Constructor

template<class Location>
Anisotropic_covariance_2d<Location>::
Anisotropic_covariance_2d(const Basic_model<Location>& covar,
			  double range1, double range2,
			  double angle, double sill) 
  : Covariance_base<Location>(), 
    transform_(range1, range2, angle) {

  covar_ = covar.clone();
  covar_->range() = range1;
  covar_->sill() = sill;
  
  range2_ = range2;
  angle_ = angle;
}



template<class Location>
Anisotropic_covariance_2d<Location>::
Anisotropic_covariance_2d(const Anisotropic_covariance_2d& rhs) 
  : Covariance_base<Location>(),
    transform_(rhs.transform_) {

  covar_ = (rhs.covar_)->clone();
  range2_ = rhs.range2_;
  angle_ = rhs.angle_;
}



// Destructor
template<class Location>
Anisotropic_covariance_2d<Location>::
~Anisotropic_covariance_2d() {
  gstl_assert(covar_ != 0);
  delete covar_;
}


template<class Location>
void
Anisotropic_covariance_2d<Location>::
get_ranges(double& R1, double& R2) const {
  R1 = covar_->range();
  R2 = range2_;
}

template<class Location>
void 
Anisotropic_covariance_2d<Location>::
set_ranges(double R1, double R2) {
  covar_->range() = R1;
  range2_ = R2;

  transform_  = Anisotropic_norm_2d<EuclideanVector>(R1,R2,angle_);
}



template<class Location>
void
Anisotropic_covariance_2d<Location>::
get_angle(double& alpha) const {
  alpha = angle_;
}

template<class Location>
void 
Anisotropic_covariance_2d<Location>::
set_angle(double alpha) {
  angle_ = alpha;
  transform_  = Anisotropic_norm_2d<EuclideanVector>(covar_->range(),
						     range2_,angle_ );
}

template<class Location>
void 
Anisotropic_covariance_2d<Location>::
set_geometry(double R1, double R2, double alpha) {
  covar_->range() = R1;
  range2_ = R2;
  angle_ = alpha;

  transform_  = Anisotropic_norm_2d<EuclideanVector>(R1,R2,angle_);
}


// operator()

template<class Location>
typename Anisotropic_covariance_2d<Location>::result_type 
Anisotropic_covariance_2d<Location>::
compute(const typename Anisotropic_covariance_2d<Location>::EuclideanVector& vec) const{
  
  return covar_->isotrop_compute( transform_(vec) );
}
  





//--------------------------
// anisotropy 3d


//constructor

template<class Location>
Anisotropic_covariance_3d<Location>::
Anisotropic_covariance_3d(const Basic_model<Location>& covar,
			  double range1, double range2, double range3,
			  double alpha, double beta, double theta,
			  double sill)
      : Covariance_base<Location>(),
	transform_(range1, range2, range3,
                   alpha, beta, theta) {

  covar_ = covar.clone();
  covar_->range() = range1;
  covar_->sill() = sill;

  range2_ = range2;
  range3_ = range3;
  alpha_  = alpha;
  beta_   = beta;
  theta_  = theta;
}


template<class Location>
Anisotropic_covariance_3d<Location>::
Anisotropic_covariance_3d(const Anisotropic_covariance_3d& rhs) 
  : Covariance_base<Location>(),
    transform_(rhs.transform_) {

  covar_ = rhs.covar_->clone();

  range2_ = rhs.range2_;
  range3_ = rhs.range3_;
  alpha_  = rhs.alpha_;
  beta_   = rhs.beta_;
  theta_  = rhs.theta_;
}


// Destructor
template<class Location>
Anisotropic_covariance_3d<Location>::
~Anisotropic_covariance_3d() {
  gstl_assert(covar_ !=0);
  delete covar_;
}


template<class Location>
void 
Anisotropic_covariance_3d<Location>::
get_ranges(double& R1, double& R2, double& R3) const {
  R1 = covar_->range();
  R2 = range2_;
  R3 = range3_;
}

template<class Location>
void 
Anisotropic_covariance_3d<Location>::
set_ranges(double R1, double R2, double R3) {
  covar_->range() = R1;
  range2_ = R2;
  range3_ = R3;
  
  transform_ = Anisotropic_norm_3d<EuclideanVector>(R1,R2,R3,
						    alpha_,beta_,theta_);
}


template<class Location>
void 
Anisotropic_covariance_3d<Location>::
get_angles(double& alpha, double& beta, double& theta) const {
  alpha = alpha_;
  beta = beta_;
  theta = theta_;
}


template<class Location>
void 
Anisotropic_covariance_3d<Location>::
set_angles(double alpha, double beta, double theta) {
  alpha_ = alpha;
  beta_ = beta;
  theta_ = theta;

  transform_ = Anisotropic_norm_3d<EuclideanVector>(covar_->range(),
						    range2_,range3_,
						    alpha_,beta_,theta_);

}

template<class Location>
void 
Anisotropic_covariance_3d<Location>::
set_geometry(double R1, double R2, double R3, 
	     double alpha, double beta, double theta) {

  covar_->range() = R1;
  range2_ = R2;
  range3_ = R3;
  
  alpha_ = alpha;
  beta_ = beta;
  theta_ = theta;

  transform_ = Anisotropic_norm_3d<EuclideanVector>(covar_->range(),
						    range2_,range3_,
						    alpha_,beta_,theta_);
}



template<class Location>
typename Anisotropic_covariance_3d<Location>::result_type 
Anisotropic_covariance_3d<Location>::
compute(const typename Anisotropic_covariance_3d<Location>::EuclideanVector& vec) const{
  return covar_->isotrop_compute( transform_(vec) );
}









//--------------------------------
// nested structures

template<class Location>
Nested_structures_covariance<Location>::
Nested_structures_covariance(double nugget_effect) 
  : Covariance_base<Location>(),
    nugget_effect_(nugget_effect) {}


template<class Location>
template<class ConstForwardIterator>
Nested_structures_covariance<Location>::
Nested_structures_covariance(result_type nugget_effect,
			     ConstForwardIterator begin, ConstForwardIterator end) 
  : Covariance_base<Location>(),
    nugget_effect_(nugget_effect) {
  
  for(ConstForwardIterator it = begin; it!=end; ++it)
    nested_structures_.push_back( (*it)->clone() );
}



template<class Location>
Nested_structures_covariance<Location>::
Nested_structures_covariance(const Nested_structures_covariance<Location>& rhs)
  : Covariance_base<Location>() {
  
  typedef typename std::vector<Covariance_base<Location>* >::const_iterator const_iterator;

  for(const_iterator it = rhs.nested_structures_.begin();
      it != rhs.nested_structures_.end() ; ++it)
    nested_structures_.push_back( (*it)->clone() );

  nugget_effect_ = rhs.nugget_effect_;
}


template<class Location>
Nested_structures_covariance<Location>& 
Nested_structures_covariance<Location>::
operator = (const Nested_structures_covariance<Location>& rhs) {

  typedef typename std::vector<Covariance_base<Location>* >::const_iterator const_iterator;
  typedef typename std::vector<Covariance_base<Location>* >::iterator iterator;

  if( *this != rhs ) {
    for( iterator it = nested_structures_.begin(); 
	 it != nested_structures_.end() ; ++it) {
      delete *it;
    }
    nested_structures_.clear();
    
    for(const_iterator it2 = rhs.nested_structures_.begin();
      it2 != rhs.nested_structures_.end() ; ++it2)
    nested_structures_.push_back( (*it2)->clone() );

    nugget_effect_ = rhs.nugget_effect_;
    
  }
  
  return *this;
}


template<class Location>
Covariance_base<Location>*
Nested_structures_covariance<Location>::clone() const {
  return new Nested_structures_covariance(*this);
}


// Destructor
template<class Location>
Nested_structures_covariance<Location>::
~Nested_structures_covariance() {
  
  typedef typename std::vector<Covariance_base<Location>* >::iterator iterator;
  
  for(iterator it=nested_structures_.begin(); it!=nested_structures_.end(); ++it) {
    gstl_assert(*it != 0);
    delete *it;
  }
  nested_structures_.clear();
}


template<class Location>
bool Nested_structures_covariance<Location>::
operator == ( const Nested_structures_covariance<Location>& rhs ) const {
  if( rhs.nested_structures_.size() != nested_structures_.size() ||
      rhs.nugget_effect_ != nugget_effect_ )
    return false;

  for( unsigned int i=0; i < nested_structures_.size(); i++ ) {
    if( nested_structures_[i] != rhs.nested_structures_[i] )
      return false;
  }

  return true;
}

template<class Location>
bool Nested_structures_covariance<Location>::
operator != ( const Nested_structures_covariance<Location>& rhs ) const {
  return !(*this == rhs );
}

template<class Location>
int
Nested_structures_covariance<Location>::
add_structure(const Covariance_base<Location>& cov){
  nested_structures_.push_back(cov.clone());
  return nested_structures_.size()-1;
}

template<class Location>
bool
Nested_structures_covariance<Location>::
remove_structure(int id){
  gstl_assert(id>=0 && id < int(nested_structures_.size()) );

  nested_structures_.erase( nested_structures_.begin()+id );
  return true;
}


template<class Location>
bool
Nested_structures_covariance<Location>::
replace_structure(int id, const Covariance_base<Location>& cov){
  gstl_assert(id>=0 && id < int(nested_structures_.size()) );

  delete nested_structures_[id];
  nested_structures_[id] = cov.clone();
  return true;
}

template<class Location>
Covariance_base<Location>* 
Nested_structures_covariance<Location>::get_structure(int id) {
  gstl_assert(id>=0 && id < int(nested_structures_.size()) );
  return nested_structures_[id];
}

template<class Location>
const Covariance_base<Location>* 
Nested_structures_covariance<Location>::get_structure(int id) const {
  gstl_assert(id>=0 && id < int(nested_structures_.size()) );
  return nested_structures_[id];
}


template<class Location>
inline typename Nested_structures_covariance<Location>::result_type
Nested_structures_covariance<Location>::c0() const {
  typedef typename std::vector< Covariance_base<Location>* >::const_iterator const_iterator;
  
  result_type c0 = nugget_effect_; 
  for(const_iterator it=nested_structures_.begin();
      it!= nested_structures_.end() ; it++) {
    c0 += (*it)->c0();
  }
  return c0;
}
  

template<class Location>
inline typename Nested_structures_covariance<Location>::result_type
Nested_structures_covariance<Location>::
compute(const typename Nested_structures_covariance<Location>::EuclideanVector& vec) const{
  
  double threshold = GsTL::EPSILON * GsTL::EPSILON;
  
  typedef typename std::vector< Covariance_base<Location>* >::const_iterator const_iterator;

  if( square_euclidean_norm(vec) <= threshold ) {
    return c0();
  }
  else {
    result_type result = 0;
  
    for(const_iterator it=nested_structures_.begin();
	it!= nested_structures_.end() ; it++) {
      result += (*it)->compute(vec);
    }
    return result;
  }
}




//--------------------------------
// Covariance
template<class Location>
Basic_model<Location>* create_spherical_covariance() {
  return new Spherical_covariance<Location>( 1, 1 );
}
template<class Location>
Basic_model<Location>* create_gaussian_covariance(){
  return new Gaussian_covariance<Location>( 1, 1 );
}
template<class Location>
Basic_model<Location>* create_exponential_covariance(){
  return new Exponential_covariance<Location>( 1, 1 );
}


template<class Location>
typename Covariance<Location>::ModelsMap Covariance<Location>::models_map_;

template<class Location>
void 
Covariance<Location>::add_covariance_type( const std::string& type_name,
					   const CallBackType& callback ) {
  models_map_[type_name] = callback;
}



template<class Location>
Covariance<Location>::Covariance() {
  
  // Add some default covariance models to the models map
  if( models_map_.size() == 0 ) {
    add_covariance_type( "Spherical", create_spherical_covariance<Location> );
    add_covariance_type( "Gaussian", create_gaussian_covariance<Location> );
    add_covariance_type( "Exponential", create_exponential_covariance<Location> );
  }
}




template<class Location>
int
Covariance<Location>::add_structure( const std::string& type ) {
  typename ModelsMap::const_iterator it = models_map_.find( type );
  if( it == models_map_.end() ) {
    gstl_warning( "No covariance model called " << type << " available" );
    return -1;
  }
    
  Basic_model<Location>* model = it->second();
  // Use default values for the anisotropy ranges and angles
  Anisotropic_covariance_3d<Location> structure( *model );
  return cov_.add_structure( structure );
}

template<class Location>
bool
Covariance<Location>::remove_structure( int id ) {
  cov_.remove_structure( id );
  return true;
}


template<class Location>
std::vector<std::string> 
Covariance<Location>::available_types() const {
  std::vector<std::string> types;
  for( typename ModelsMap::const_iterator it = models_map_.begin();
       it != models_map_.end(); ++it )
    types.push_back( it->first );

  return types;
}



template<class Location>
void 
Covariance<Location>::set_type( int id, const std::string& type ) {
  typename ModelsMap::const_iterator it = models_map_.find( type );
  if( it == models_map_.end() ) {
    gstl_warning( "No covariance model called " << type << " available" );
    return;
  }
  
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* old_struct = dynamic_cast< StructureType* >( structure );

  // Get a new structure with the requested covariance type
  Basic_model<Location>* model = it->second();
  StructureType new_struct( *model );
  double r1,r2,r3, a1,a2,a3;
  old_struct->get_ranges( r1,r2,r3 );
  old_struct->get_angles( a1,a2,a3 );
  new_struct.set_geometry( r1,r2,r3, a1,a2,a3 );
  new_struct.sill() = old_struct->sill();

  cov_.replace_structure( id, new_struct );
}


template<class Location>
void 
Covariance<Location>::get_ranges( int id,
				  double& R1, double& R2, double& R3 ) const {
  const Covariance_base<Location>* structure = cov_.get_structure( id );
  const StructureType* varg_struct = dynamic_cast< const StructureType* >( structure );
  varg_struct->get_ranges( R1, R2, R3 );
}


template<class Location>
void 
Covariance<Location>::set_ranges( int id, double R1, double R2, double R3 ) {
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* varg_struct = dynamic_cast< StructureType* >( structure );
  varg_struct->set_ranges( R1, R2, R3 );
}


template<class Location>
void
Covariance<Location>::get_angles( int id, 
				  double& alpha, double& beta, double& theta ) const {
  const Covariance_base<Location>* structure = cov_.get_structure( id );
  const StructureType* varg_struct = dynamic_cast< const StructureType* >( structure );
  varg_struct->get_angles( alpha, beta, theta );
}


template<class Location>
void
Covariance<Location>::set_angles( int id, 
                                  double alpha, double beta, double theta ) {
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* varg_struct = dynamic_cast< StructureType* >( structure );
  varg_struct->set_angles( alpha, beta, theta );
}

template<class Location>
void
Covariance<Location>::set_geometry( int id,
				    double R1, double R2, double R3, 
				    double alpha, double beta, double theta ) {
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* varg_struct = dynamic_cast< StructureType* >( structure );
  varg_struct->set_ranges( R1, R2, R3 );
  varg_struct->set_angles( alpha, beta, theta );
}

template<class Location>
double Covariance<Location>::sill( int id ) const {
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* varg_struct = dynamic_cast< StructureType* >( structure );
  return varg_struct->sill();
}

template<class Location>
void Covariance<Location>::sill( int id, double s ) {
  Covariance_base<Location>* structure = cov_.get_structure( id );
  StructureType* varg_struct = dynamic_cast< StructureType* >( structure );
  varg_struct->sill() = s;
}
  


#ifdef __GNUC__
#pragma implementation
#endif
#include "covariance.h"



