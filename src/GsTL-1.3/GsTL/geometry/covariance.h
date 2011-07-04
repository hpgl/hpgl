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

#ifndef __GSTL_COVARIANCE_H__
#define __GSTL_COVARIANCE_H__

#include <GsTL/math/math_functions.h>
#include <GsTL/geometry/geometry_algorithms.h>
#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/geometry/geometry.h>
#include <GsTL/utils/gstl_error_messages.h>

#include <vector>
#include <map>
#include <string>

/** The covariance models all derive from class "Covariance_base".
 * They simply override the pure virtual operator() of "covariance"
 * Operator() performs very few computations and is likely to be called 
 * many times in a program (in sequential gaussian simulation, it might be
 * called N*(20*20/2) times, if N is the number of nodes to simulate and we
 * use 20 conditioning data for simulating each node), hence it is questionable 
 * whether it is a good choice to make operator() virtual.
 */

template<class Location>
class Covariance_base{

 public:
  // STL Binary Function requirements
  typedef Location first_argument_type;
  typedef Location second_argument_type;
  typedef double result_type;


 public:
  typedef Location argument_type;
  typedef typename Location::difference_type EuclideanVector;
  
  virtual ~Covariance_base() {}
  
  virtual Covariance_base<Location>* clone() const = 0 ;

  virtual result_type c0() const = 0;

  inline result_type operator()(const Location& u1, const Location& u2) const {
    return compute(u2-u1);
  } 

  virtual result_type compute(const EuclideanVector& vec) const = 0; 

 protected:
  Covariance_base() {}
  Covariance_base(const Covariance_base<Location>& cov) {};

};



/** A Basic Model is one of the canonical covariance models:
 *   spherical, exponential, gaussian.
 */
template<class Location>
class Basic_model : public Covariance_base<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;

 public:
	 Basic_model() {}
  Basic_model(double range, double sill) 
    : a_(range), c_(sill) {}

  virtual Basic_model<Location>* clone() const = 0 ;

  inline double& range()             {return a_; }
  inline double& sill()              {return c_; }
  inline const double& range() const {return a_; }
  inline const double& sill() const  {return c_; }
  virtual result_type c0() const { return c_; }

  virtual result_type isotrop_compute(double h) const = 0;

 protected:
  double a_;
  double c_;
};



/** Spherical covariance
 */

template<class Location>
class Spherical_covariance : public Basic_model<Location> {
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
	 Spherical_covariance( ) {}
  Spherical_covariance( double range, double sill ) 
    : Basic_model<Location>(range, sill) {}

  Spherical_covariance( const Spherical_covariance<Location>& rhs ) 
    : Basic_model<Location>(rhs.a_, rhs.c_) {
  }

  virtual Basic_model<Location>* clone() const {
    return new Spherical_covariance<Location>(this->a_, this->c_);
  }

  virtual result_type compute( const EuclideanVector& vec ) const;

  virtual result_type isotrop_compute( double h ) const;
};




/** Exponential covariance
 */

template<class Location>
class Exponential_covariance : public Basic_model<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
  Exponential_covariance(double range, double sill) 
    : Basic_model<Location>(range, sill) {}

  Exponential_covariance( const Exponential_covariance<Location>& rhs) 
    : Basic_model<Location>(rhs.a_, rhs.c_) {
  }

  virtual Basic_model<Location>* clone() const {
    return new Exponential_covariance<Location>(this->a_, this->c_);
  }

  virtual result_type compute(const EuclideanVector& vec) const;

 protected:
  virtual result_type isotrop_compute(double h) const;
};




/** Gaussian covariance
 */

template<class Location>
class Gaussian_covariance : public Basic_model<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
  Gaussian_covariance(double range, double sill) 
    : Basic_model<Location>(range, sill) {}

  Gaussian_covariance( const Gaussian_covariance<Location>& rhs) 
    : Basic_model<Location>(rhs.a_, rhs.c_) {
  }

  virtual Basic_model<Location>* clone() const {
    return new Gaussian_covariance<Location>(this->a_, this->c_);
  }

  virtual result_type compute(const EuclideanVector& vec) const;

  virtual result_type isotrop_compute(double h) const;
};



/** Pure nugget effect.
 * This class is provided solely for consistency. It is highly non-efficient
 * since more time will be spent resolving the virtual function call than 
 * doing any computations.
 * If you want to have, say a spherical model with some nugget effect, use class
 * Nested_structures_covariance which allows to specify a nugget effect as a
 * constant number (do not make a Nested_structures_covariance with a "pure_nugget"
 * and a "Spherical_covariance"...)
 */
template<class Location>
class Pure_nugget : public Covariance_base<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
  Pure_nugget(double nugget)
    : threshold_(GsTL::EPSILON), n_(nugget) {
    threshold_ *= threshold_;
  }

  Pure_nugget(const Pure_nugget& rhs) {
    threshold_ = rhs.threshold_;
    n_ = rhs.n_;
  }


  virtual Covariance_base<Location>* clone() const {
    return new Pure_nugget(threshold_,n_);
  }

  inline double& nugget()              {return n_; }
  inline const double& nugget() const  {return n_; }

  virtual result_type compute(const EuclideanVector& vec) const{
    if(square_euclidean_norm(vec) <= this->a_ )
      return n_;
    else
      return 0.0;
  };
  
  virtual result_type isotrop_compute(double h) const {
    if(h <= this->a_ )
      return n_;
    else
      return 0.0;
  }

 private:
  double threshold_;
  double n_;
};



/** The following two classes "anisotropy_2d" and "anisotropy_3d" are used
 * to account for anisotropy. The 2D and 3D case are separated because in 2D
 * the anisotropy is defined by one single angle, while in 3D 3 angles are needed
 * The anisotropy objects act as function adaptors.
 * The Anisotropic_covariances are actually wrappers around isotropic, "Basic" models.
 */

template<class Location>
class Anisotropic_covariance_2d : public Covariance_base<Location> {
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:

  /** Constructor
   * @param range1 and range2 are the dimensions of the anisotropy ellipsoid
   * @param angle is the rotation angle in RADIANS (do not forget to convert
   * your angles in radian).
   *
   * Requiremtnts on SingleStructureCovariance:
   *    - constructor:  A(range, sill);
   *    - access function:  double& range();
   *                        double& sill();
   */
  Anisotropic_covariance_2d(const Basic_model<Location>& covar,
			    double range1, double range2,
			    double angle, double sill);

  virtual ~Anisotropic_covariance_2d();

  Anisotropic_covariance_2d(const Anisotropic_covariance_2d& rhs);
    
  virtual Covariance_base<Location>* clone() const {
    return new Anisotropic_covariance_2d(*this);
  }

  void get_ranges(double& R1, double& R2) const ;
  void set_ranges(double R1, double R2);
  void get_angle(double& alpha) const;
  void set_angle(double alpha);
  void set_geometry(double R1, double R2, double alpha);

  double& sill()               { return covar_->sill(); }
  const double& sill() const   { return covar_->sill(); }
  virtual result_type c0() const { return covar_->c0(); }


  virtual result_type compute(const EuclideanVector& vec) const;

  
 private:
  Basic_model<Location>* covar_;
  double range2_;
  double angle_;

  Anisotropic_norm_2d<EuclideanVector> transform_;

};




//______________________________________________

/** This function adapter modifies the behavior of a Covariance
 * in order to account for anisotropy. The 2D case has to be distinguished
 * from the 3D one because in 2D only 1 rotation angle is needed to 
 * characterize the anisotropy ellipsoid, while in 3D, 3 angles are needed.
 *
 * The class definition uses a TNT matrix for pure convenience. The TNT matrix
 * could be replaced by an array, but the computations made by the constructor
 * might become less clear (the matrix multiplication would have to be
 * hard-coded).
 */
template<class Location>
class Anisotropic_covariance_3d : public Covariance_base<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
  
  /** Constructor
   * @param stretch1,... are the stretching (shrinking) factors
   * @param angle1,... are the rotation angle in RADIANS.
   */
  Anisotropic_covariance_3d( const Basic_model<Location>& covar,
			     double range1=1., double range2=1., double range3=1.,
			     double alpha=0.0, double beta=0.0, double theta=0.0,
			     double sill=1.0 );

  virtual ~Anisotropic_covariance_3d();

  Anisotropic_covariance_3d(const Anisotropic_covariance_3d& rhs);
    
  virtual Covariance_base<Location>* clone() const {
    return new Anisotropic_covariance_3d(*this);
  }
  
  void get_ranges(double& R1, double& R2, double& R3) const ;
  void set_ranges(double R1, double R2, double R3);
  void get_angles(double& alpha, double& beta, double& theta) const;
  void set_angles(double alpha, double beta, double theta) ;
  void set_geometry(double R1, double R2, double R3, 
		    double alpha, double beta, double theta);

  double& sill()               { return covar_->sill(); }
  const double& sill() const   { return covar_->sill(); }
  virtual result_type c0() const { return covar_->c0(); }


  virtual result_type compute(const EuclideanVector& vec) const;

  
 private:
  Basic_model<Location>* covar_;              
  double range2_ , range3_;
  double alpha_, beta_, theta_;

  Anisotropic_norm_3d<EuclideanVector> transform_;
  
};





//----------------------------------------
// Finally, the definition of covariances with
// nested structures

/** \brief This class implements a covariance with nested structures.
 * The nugget effect is here coded as a mere constant instead of
 * being a Covariance. The implementation of covariances use virtual
 * functions, and it is certainly not very efficient to use a virtual
 * function to return a constant (nugget effect). Since the nugget 
 * covariance is a constant function, it seems more efficient to code
 * it as a constant number.
 */
template<class Location>
class Nested_structures_covariance : public Covariance_base<Location>{
 public:
  typedef typename Covariance_base<Location>::result_type result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:

  /** Default constructor
   */
  Nested_structures_covariance(double nugget_effect=0.0);
  
  /** Constructor. 
   * @param nugget_effect is the nugget effect of the covariance.
   * @param [begin,end) is a range of pointers to Covariances (i.e. "begin" is an 
   * iterator to a pointer to a covariance). The range is const.
   */
  template<class ConstForwardIterator>
  Nested_structures_covariance( result_type nugget_effect, 
				ConstForwardIterator begin, ConstForwardIterator end );

  Nested_structures_covariance(const Nested_structures_covariance<Location>& rhs);
  Nested_structures_covariance<Location>& operator=( const Nested_structures_covariance<Location>& rhs );

  virtual ~Nested_structures_covariance();
  
  bool operator == ( const Nested_structures_covariance<Location>& rhs ) const;
  bool operator != ( const Nested_structures_covariance<Location>& rhs ) const;


  int add_structure(const Covariance_base<Location>& cov);
  bool remove_structure(int id);
  bool replace_structure( int id, const Covariance_base<Location>& cov);
  inline unsigned int number_of_structures() const;
  virtual Covariance_base<Location>* clone() const;

  Covariance_base<Location>* get_structure(int id);
  const Covariance_base<Location>* get_structure(int id) const;

  result_type nugget() const { return nugget_effect_ ; }
  void nugget( double n ) { nugget_effect_ = n; }

  virtual result_type c0() const;

  virtual result_type compute(const EuclideanVector& vec) const;
    
  
  private:
  std::vector< Covariance_base<Location>* > nested_structures_;
  result_type nugget_effect_;

};


template<class Location>
inline unsigned int
Nested_structures_covariance<Location>::number_of_structures() const{
  return nested_structures_.size();
}




/** This is a handle class that embeds a covariance and meets the requirements
 * of concept Covariance.
 * This handle class is useful to create containers of covariances or to  
 * use function adaptors. 
 */
template<class Location>
class Covariance_functor {
 public:
  // STL Binary Function requirements
  typedef Location first_argument_type;
  typedef Location second_argument_type;
  typedef double result_type;

 public:
  typedef Location argument_type;

  Covariance_functor() : cov_(0) {}

  Covariance_functor(Covariance_base<Location>* cov) {
    gstl_assert(cov != 0);
    cov_ = cov->clone();
  }

  Covariance_functor(const Covariance_functor<Location>& rhs) {
    if(rhs.cov_ == 0)
      cov_ = 0;
    else
      cov_ = rhs.cov_->clone();
  }

  ~Covariance_functor() {
    if(cov_ != 0)
      delete cov_;
  }

  Covariance_functor<Location>& operator=(const Covariance_functor<Location>& rhs) {
    if(rhs.cov_ == 0)
      cov_ = 0;
    else
      cov_ = rhs.cov_->clone();
    return *this;
  }

  void set_engine(Covariance_base<Location>* cov) {
    gstl_assert(cov != 0);
    cov_ = cov->clone();
  }

  inline result_type operator()(const Location& u1, const Location& u2) const {
    return (*cov_)(u1,u2);
  }


 private:
  Covariance_base<Location>* cov_;

};



//============================================================

/** This class defines a general purpose covariance function. 
 */
template< class Location >
class Covariance {

 public:
  typedef Basic_model<Location>* (*CallBackType)( void );
  static void add_covariance_type( const std::string& type_name,
				   const CallBackType& callback );

 public:
  typedef Location first_argument_type;
  typedef Location second_argument_type;
  typedef double result_type;
  typedef typename Location::difference_type EuclideanVector;
  
 public:
  Covariance();
  Covariance( const Covariance<Location>& rhs ) : cov_( rhs.cov_ ) {}
  Covariance<Location>& operator=( const Covariance<Location>& rhs ) {
    cov_ = rhs.cov_;
    return *this; 
  }

  inline result_type operator()(const Location& u1, const Location& u2) const {
    return cov_(u1,u2);
  } 

  inline result_type compute(const EuclideanVector& vec) const {
    return cov_.compute( vec );
  } 

  double nugget() const { return cov_.nugget(); }
  void nugget( double n ) { cov_.nugget( n ); }

  int structures_count() const { return cov_.number_of_structures(); } 
  int add_structure( const std::string& type );
  bool remove_structure( int id );

  std::vector<std::string> available_types() const;
  void set_type( int id, const std::string& type );

  void get_ranges( int id, double& R1, double& R2, double& R3 ) const ;
  void set_ranges( int id, double R1, double R2, double R3 );
  void get_angles( int id, double& alpha, double& beta, double& theta ) const;
  void set_angles( int id, double alpha, double beta, double theta ) ;
  void set_geometry( int id,
		     double R1, double R2, double R3, 
		     double alpha, double beta, double theta );
  double sill( int id ) const;
  void sill( int id, double s );
  
 private:
  typedef Anisotropic_covariance_3d<Location> StructureType;
  Nested_structures_covariance<Location> cov_;

 private:
  typedef std::map< std::string, CallBackType > ModelsMap;
  static ModelsMap models_map_;

};


template<class Location>
Basic_model<Location>* create_spherical_covariance();

template<class Location>
Basic_model<Location>* create_gaussian_covariance();

template<class Location>
Basic_model<Location>* create_exponential_covariance();




//================================================================

/** This class is a function adapter that turns a covariance into a
 * variogram.
 */
template< class Covariance_ >
class Variogram_function_adaptor {
 public:
  typedef typename Covariance_::first_argument_type Location;

  // STL Binary Function requirements
  typedef Location first_argument_type;
  typedef Location second_argument_type;
  typedef double result_type;

 public:
  Variogram_function_adaptor() {
    Location P1;
    null_ = P1-P1;
  }

  Variogram_function_adaptor( const Covariance_& cov )
    : cov_( cov ) {
    Location P1;
    null_ = P1-P1;
  }

  inline result_type operator()( const Location& u1, const Location& u2 ) const {
    return cov_.compute( null_ ) - cov_(u1,u2);
  }

  Covariance_* covariance() { return &cov_; }


 protected:
  Covariance_ cov_;
  typename Location::difference_type null_;
};


#include "covariance.cc"

#endif
