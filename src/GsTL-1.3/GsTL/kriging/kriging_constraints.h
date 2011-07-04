#ifndef __GSTL_KRIGING_KRIGING_CONSTRAINTS_H__
#define __GSTL_KRIGING_KRIGING_CONSTRAINTS_H__


#include <GsTL/kriging/SK_constraints.h>
#include <GsTL/kriging/OK_constraints.h>
#include <GsTL/kriging/KT_constraints.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>

#include <vector>


template <
          class Neighborhood_,
          class Location_,
          class WeightsVector,
          class MatrixLibrary
         >
class Kriging_constraints_impl;





/** This class models the KrigingConstraints concept. 
 */
template <
          class Neighborhood_,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class Kriging_constraints {
 private:
  typedef Neighborhood_ N;
  typedef Location_ L;
  typedef MatrixLibrary M;
  typedef WeightsVector W;
  typedef Kriging_constraints_impl<N,L,W,M> BaseClass;
  typedef Kriging_constraints<N,L,W,M> Self;
  typedef typename WeightsVector::iterator Iterator_;
  
 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
  
 public:
 
  Kriging_constraints( BaseClass* impl ) {
    if( impl )
      impl_ = impl->clone();
    else
      impl_ = 0;
  }

  Kriging_constraints( const Self& rhs ) {
    if( rhs.impl_ )
      impl_ = rhs.impl_->clone();
    else
      impl_ = 0;  
  }


  ~Kriging_constraints() { delete impl_; }

  Self& operator = ( const Self& rhs ) {
    if( impl_ == rhs.impl_ ) return *this;
    delete impl_;
    if( rhs.impl_ == 0 ) 
      impl_ = 0;
    else
      impl_ = rhs.impl_->clone();
    return *this;
  }
  
  inline unsigned int operator () (
				   SymMatrix& A,
				   Vector& b,
				   const Location_& center,
				   const Neighborhood_& neighbors
				   ) const {
    return (*impl_)( A,b,center, neighbors );
  }


  inline double kriging_variance_contrib( const Location_& center,
                                          Iterator_ weights_begin,
                                          Iterator_ weights_end ) const {
    return impl_->kriging_variance_contrib( center, weights_begin, weights_end );
  }
                                           

 private:
  BaseClass* impl_;

};





//====================================
/** This class defines a dynamic hierarchy of kriging constraints.
 * The aim of this class and its derived classes is simply to wrap
 * classes SK_constraints, OK_constraints and KT_constraints into
 * a hierarchy of classes, so that a kriging constraint can be selected
 * dynamically. 
 *
 * What is convenient with SK_constraints, OK_constraints and KT_constraints
 * is that "operator()" is templated, hence the template arguments don't
 * have to be explicitly supplied (they are automatically deduced by the
 * compiler). This is no the case with  Kriging_constraints_impl and its
 * derived classes (a virtual function can not be templated).
 */
template <
          class Neighborhood_,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class Kriging_constraints_impl {
 protected:
 typedef Neighborhood_ N;
 typedef Location_ L;
 typedef WeightsVector W;
 typedef MatrixLibrary M;
 typedef Kriging_constraints_impl<N,L,W,M> Self;
 typedef typename WeightsVector::iterator Iterator_;

 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
 
 public:
  virtual ~Kriging_constraints_impl() {}
  
  virtual Self* clone() const = 0;

  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    const Neighborhood_& neighbors
				    ) const = 0;

  virtual double kriging_variance_contrib( const Location_& center,
                                           Iterator_ weights_begin,
                                           Iterator_ weights_end ) const =0;
};




//====================================
/** SK kriging constraints.
 */

template <
          class Neighborhood_,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class SKConstraints_impl : public Kriging_constraints_impl<Neighborhood_,
							   Location_, WeightsVector,
							   MatrixLibrary> {
 private:
  typedef Neighborhood_ N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary M;
  typedef Kriging_constraints_impl<N,L,W,M> BaseClass;
  
 public:
  typedef typename BaseClass::SymMatrix SymMatrix;
  typedef typename BaseClass::Vector Vector;
 
  virtual BaseClass* clone() const {
    return new SKConstraints_impl<N,L,W,M>;
  }


  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    const Neighborhood_& neighbors
				    ) const {

    SK_constraints constraints;				      
    return constraints( A,b, center, neighbors );
  }

  virtual double kriging_variance_contrib( const Location_& center,
                                           typename BaseClass::Iterator_ weights_begin,
                                           typename BaseClass::Iterator_ weights_end ) const {
    return 0.0;
  }
};



//====================================
/** OK kriging constraints.
 */

template <
          class Neighborhood_,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class OKConstraints_impl : public Kriging_constraints_impl<Neighborhood_,
							   Location_, WeightsVector,
							   MatrixLibrary> {

 private:
  typedef Neighborhood_ N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary M;
  typedef Kriging_constraints_impl<N,L,W,M> BaseClass;
  
 public:
  typedef typename BaseClass::SymMatrix SymMatrix;
  typedef typename BaseClass::Vector Vector;
 
  virtual BaseClass* clone() const {
    return new OKConstraints_impl<N,L,W,M>;
  }


  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    const Neighborhood_& neighbors
				    ) const {
    OK_constraints constraints;
    return constraints( A,b, center, neighbors );
  }

  virtual double kriging_variance_contrib( const Location_& center,
                                           typename BaseClass::Iterator_ weights_begin,
                                           typename BaseClass::Iterator_ weights_end ) const {
    return *weights_begin;
  }
};





//====================================
/** KT kriging constraints.
 */

template <
          class MeanFunctor,
          class Neighborhood_,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class KTConstraints_impl : public Kriging_constraints_impl<Neighborhood_,
							   Location_, WeightsVector,
							   MatrixLibrary> {

 private:
  typedef MeanFunctor MF;
  typedef Neighborhood_ N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary ML;
  typedef Kriging_constraints_impl<N,L,W,ML> BaseClass;


 public:
  typedef typename BaseClass::SymMatrix SymMatrix;
  typedef typename BaseClass::Vector Vector;


  KTConstraints_impl( const std::vector<MeanFunctor>& mean_functions ) 
    : BaseClass(), 
      mean_functions_( mean_functions ),
      constraints_( mean_functions_.begin(), mean_functions_.end() ) { 
  }

  virtual BaseClass* clone() const {
    return new KTConstraints_impl<MF,N,L,W,ML>( mean_functions_ );
  }


  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    const Neighborhood_& neighbors
				    ) const {
    return constraints_( A,b, center, neighbors ); 
  }

  virtual double kriging_variance_contrib( const Location_& center,
                                           typename BaseClass::Iterator_ weights_begin,
                                           typename BaseClass::Iterator_ weights_end ) const {
    return constraints_.kriging_variance_contrib( center, weights_begin, weights_end );
  }
  
 protected:
  std::vector<MeanFunctor> mean_functions_;
  typedef typename std::vector<MeanFunctor>::const_iterator mean_iterator;
  KT_constraints<mean_iterator> constraints_; 
};



#endif
