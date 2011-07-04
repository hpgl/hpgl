#ifndef __GSTL_MATH_INDICATOR_FUNCTIONS_H__
#define __GSTL_MATH_INDICATOR_FUNCTIONS_H__



/** This functor is the indicator function for continuous variables,
 * or more precisely, variables that are ordered
 */
template<class T>
class Indicator_function {
 public:
  virtual Indicator_function<T>* clone() const {
    return new Indicator_function<T>();
  }

  virtual bool operator () ( const T& z, const T& thresh ) const {
    return z <= thresh;
  }  
};

/** This functor is the indicator function for categorical variables,
 * ie variables that take discrete values ("classes" or "categories")
 */
template<class T>
class Class_indicator_function : public Indicator_function<T> {
 public:
  virtual Indicator_function<T>* clone() const {
    return new Class_indicator_function<T>(); 
  }

  virtual bool operator () ( const T& z, const T& category ) const {
    return z == category;
  }  
};


template <class T>
class Indicator {
 public:
  Indicator( Indicator_function<T>* indicator = 0 ) {
    if( indicator ) 
      indicator_ = indicator;
    else
      indicator_ = new Indicator_function<T>;
  }

  Indicator( const Indicator& rhs ) 
    : indicator_( rhs.indicator_->clone() ) {}

  ~Indicator() { delete indicator_; }

  void set_function( Indicator_function<T>* indicator ) {
    indicator_ = indicator ;
  }
  
  Indicator& operator = ( const Indicator& rhs ) {
    indicator_ = rhs.indicator_->clone();
    return *this;
  }
  

  bool operator () ( const T& z, const T& thres ) const {
    return (*indicator_)( z, thres ) ;
  }  

 private:
  Indicator_function<T>* indicator_;
};

#endif
