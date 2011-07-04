#ifndef __GSTL_KRIGING_COKRIGING_CONSTRAINTS_H__
#define __GSTL_KRIGING_COKRIGING_CONSTRAINTS_H__


#include <GsTL/kriging/SK_constraints.h>
#include <GsTL/kriging/OK_constraints.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>

#include <vector>

template <
          class InputIterator,
          class Location_,
          class WeightsVector,
          class MatrixLibrary
         >
class CoKriging_constraints_impl;


template <
          class InputIterator,
          class Location_,
          class WeightsVector,
          class MatrixLibrary
         >
class Co_OKConstraints_impl;


/** This class is a model of concept CoKrigingConstraints.
 */
template <
          class InputIterator,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class CoKriging_constraints {
 private:
  typedef InputIterator N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary M;
  typedef CoKriging_constraints_impl<N,L,W,M> BaseClass;
  typedef CoKriging_constraints<N,L,W,M> Self;
  typedef typename WeightsVector::iterator Iterator_;
    
 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
  
 public:
 
  CoKriging_constraints( BaseClass* impl = 0 ) {
    if( impl )
      impl_ = impl->clone();
    else
      impl_ = new Co_OKConstraints_impl<N,L,W,M> ; 
  }

  CoKriging_constraints( const Self& rhs ) {
    if( rhs.impl_ == 0 )
      impl_ = 0;
    else
      impl_ = rhs.impl_->clone();
  }


  ~CoKriging_constraints() { delete impl_; }

  Self& operator = ( const Self& rhs ) {
    if( rhs.impl_ == impl_ ) return *this;
    delete impl_;
    if( rhs.impl_ )    
      impl_ = rhs.impl_->clone();
    else
      impl_ = 0;
    return *this;
  }
  
 inline unsigned int operator () (
				  SymMatrix& A,
				  Vector& b,
				  const Location_& center,
				  InputIterator first_neigh, 
				  InputIterator last_neigh
				  ) const {
    return (*impl_)( A,b,center, first_neigh, last_neigh );
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
/** This class defines a dynamic hierarchy of cokriging constraints.
 * The aim of this class and its derived classes is simply to wrap
 * classes SK_constraints and OK_constraints into
 * a hierarchy of classes, so that a cokriging constraint can be selected
 * dynamically. 
 *
 * What is convenient with SK_constraints and OK_constraints
 * is that "operator()" is templated, hence the template arguments don't
 * have to be explicitly supplied (they are automatically deduced by the
 * compiler). This is no the case with  CoKriging_constraints_impl and its
 * derived classes (a virtual function can not be templated).
 */
template <
          class InputIterator,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class CoKriging_constraints_impl {
 protected:
 typedef InputIterator N;
 typedef Location_ L;
 typedef WeightsVector W;
 typedef MatrixLibrary M;
 typedef typename WeightsVector::iterator Iterator_;
 typedef CoKriging_constraints_impl<N,L,W,M> Self;

 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
 
 public:
  virtual ~CoKriging_constraints_impl() {}
  
  virtual Self* clone() const = 0;

  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    InputIterator first_neigh,
				    InputIterator last_neigh
				    ) const = 0;

  virtual double kriging_variance_contrib( const Location_& center,
                                           Iterator_ weights_begin,
                                           Iterator_ weights_end ) const =0;

};






//====================================
/** SK cokriging constraints.
 */

template <
          class InputIterator,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class Co_SKConstraints_impl : public CoKriging_constraints_impl<InputIterator,
							   Location_, WeightsVector,
							   MatrixLibrary> {
 private:
  typedef InputIterator N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary M;
  typedef CoKriging_constraints_impl<N,L,W,M> BaseClass;
  
 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
 
  virtual BaseClass* clone() const {
    return new Co_SKConstraints_impl<N,L,W,M>;
  }


  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    InputIterator first_neigh,
				    InputIterator last_neigh
				    ) const {

    SK_constraints constraints;				      
    return constraints( A,b, center, 
			first_neigh, last_neigh );
  }

  virtual double kriging_variance_contrib( const Location_& center,
                                           typename BaseClass::Iterator_ weights_begin,
                                           typename BaseClass::Iterator_ weights_end ) const {
    return 0.0;
  }

};



//====================================
/** OK cokriging constraints.
 */

template <
          class InputIterator,
          class Location_,
          class WeightsVector=std::vector<double>,
          class MatrixLibrary=GSTL_TNT_lib
         >
class Co_OKConstraints_impl : public CoKriging_constraints_impl<InputIterator,
							   Location_, WeightsVector,
							   MatrixLibrary> {

 private:
  typedef InputIterator N;
  typedef Location_ L;
  typedef WeightsVector W;
  typedef MatrixLibrary M;
  typedef CoKriging_constraints_impl<N,L,W,M> BaseClass;
  
 public:
  typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
  typedef typename MatrixLib::Symmetric_matrix SymMatrix;
  typedef typename MatrixLib::Vector Vector;
 
  virtual BaseClass* clone() const {
    return new Co_OKConstraints_impl<N,L,W,M>;
  }


  virtual unsigned int operator () (
				    SymMatrix& A,
				    Vector& b,
				    const Location_& center,
				    InputIterator first_neigh,
				    InputIterator last_neigh
				    ) const {
    OK_constraints constraints;
    return constraints( A,b, center, 
			first_neigh, last_neigh );
  }

  virtual double kriging_variance_contrib( const Location_& center,
                                           typename BaseClass::Iterator_ weights_begin,
                                           typename BaseClass::Iterator_ weights_end ) const {
    return *weights_begin;
  }

};



#endif
