#ifndef __GSTL_COVARIANCE_SET_H__
#define __GSTL_COVARIANCE_SET_H__



/** Covariance_set_impl is the base class of a set of objects that
 * model concept CovarianceSet.
 */
template < class Covariance_ >
class Covariance_set_impl {
 public:
  typedef typename Covariance_::result_type result_type;
  typedef typename Covariance_::first_argument_type first_argument_type;
  typedef typename Covariance_::second_argument_type second_argument_type;
  typedef first_argument_type Location;

 public:
  virtual Covariance_set_impl<Covariance_>* clone() const = 0;

  virtual result_type operator()(unsigned int i, unsigned int j,
				 const Location& u1, const Location& u2) const = 0;
};



/** Covariance_set is a function object that models GsTL concept CovarianceSet.
 */
template<class Covariance_>
class Covariance_set {
 public:
  typedef typename Covariance_::result_type result_type;
  typedef typename Covariance_::first_argument_type first_argument_type;
  typedef typename Covariance_::second_argument_type second_argument_type;
  typedef first_argument_type Location;

 public:
   Covariance_set( Covariance_set_impl<Covariance_>* impl = 0 ) 
     : impl_(0) {
    set_implementation( impl );
  }

  Covariance_set( const Covariance_set<Covariance_>& rhs ) {
    if( rhs.impl_ ) 
      impl_ = rhs.impl_->clone();
    else
      impl_ = 0;
  }

  ~Covariance_set() { delete impl_; }
  

  Covariance_set<Covariance_>& operator = ( const Covariance_set<Covariance_>& rhs ) {
    if( rhs.impl_ == impl_ ) return *this;
    delete impl_;
    if( rhs.impl_ == 0 )
      impl_ = 0;
    else
      impl_ = rhs.impl_->clone();
    return *this;
  }
  void set_implementation( Covariance_set_impl<Covariance_>* impl ) {
    delete impl_;
    if( !impl ) 
      impl_ = 0;
    else
      impl_ = impl->clone();
  }


  result_type operator()(unsigned int i, unsigned int j,
				 const Location& u1, const Location& u2) const {
    return (*impl_)( i,j, u1,u2 );
  }
  

 private:
  Covariance_set_impl<Covariance_>* impl_; 
};

#endif
