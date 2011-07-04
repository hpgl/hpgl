#ifndef __GSTL_CDF_BASICS_H__
#define __GSTL_CDF_BASICS_H__


#include <vector>

namespace GsTL {

struct continuous_variable_tag {};
struct discrete_variable_tag {};

}

template< class T > 
class Cdf {
 public:
  typedef T value_type;
  virtual value_type inverse(double p) const = 0;
  virtual double prob(value_type z) const = 0;
};


template< class T >
class Non_parametric_cdf : public Cdf<T> {
 public:
  typedef typename Cdf<T>::value_type value_type;
  typedef typename std::vector<value_type>::iterator        z_iterator;
  typedef std::vector<double>::iterator            p_iterator;
  typedef typename std::vector<value_type>::const_iterator  const_z_iterator;
  typedef std::vector<double>::const_iterator      const_p_iterator;

 public:
  static const double NaN;

  Non_parametric_cdf() {}
  virtual ~Non_parametric_cdf() {}

  virtual bool make_valid() = 0; 

  virtual void resize( unsigned int m ) {
    z_values_.resize(m);
    p_values_.resize(m);
  }
  
  virtual value_type inverse(double p) const = 0;

  virtual int size() const { return  (int) z_values_.size(); }

  inline z_iterator z_begin() {return z_values_.begin();}
  inline z_iterator z_end() {return z_values_.end();}
  
  inline p_iterator p_begin() {return p_values_.begin();}
  inline p_iterator p_end() {return p_values_.end();}
  
  inline const_z_iterator z_begin() const {return z_values_.begin();}
  inline const_z_iterator z_end() const {return z_values_.end();}
  
  inline const_p_iterator p_begin() const {return p_values_.begin();}
  inline const_p_iterator p_end() const {return p_values_.end();}

  value_type mean( void )
  {
	  double inc_p = 0.005;
	  double mean = 0.;

	  for(double pp = 0.005; pp<0.995; pp+=inc_p )
		  mean += inverse( pp )*inc_p;
	  return  mean;
  }
  value_type variance( void ) const
  {
	  double inc_p = 0.005;
	  double s1 = 0.;
	  double s2 = 0.;
	  double inv_p;

	  for(double pp = 0.005; pp<0.995; pp+=inc_p ){
		  inv_p = inverse( pp );
		  s1 += inv_p*inc_p;
		  s2 += inv_p*inv_p*inc_p;
	  }
/*
	std::vector< double > ik_pdf(p_values_.size());
	std::adjacent_difference(p_values_.begin(),p_values_.end(),ik_pdf.begin() );
	value_type s1 = std::inner_product(ik_pdf.begin(),ik_pdf.end(), z_values_.begin(), 0.0);
	value_type s2 = std::inner_product(ik_pdf.begin(),ik_pdf.end(), z_values_.begin(), 0.0, 
		std::plus<double>(),inner_prod_square);
*/
	return s2 - s1*s1;
  }

 protected:
  std::vector<value_type>  z_values_;
  std::vector<double>      p_values_;
};

template< class T >
const double Non_parametric_cdf<T>::NaN = -9e30;

//template< typename value_type >
inline double inner_prod_square(double p, double z) { 	return p*z*z; }

#endif
