#ifndef __GSTL_KRIGING_MARKOV_BAYES_COVARIANCE_H__
#define __GSTL_KRIGING_MARKOV_BAYES_COVARIANCE_H__


#include <GsTL/kriging/covariance_set.h>
#include <GsTL/math/math_functions.h>


template < class Covariance_ >
class MarkovBayes_covariance : public Covariance_set_impl<Covariance_>{

public:
  typedef typename Covariance_::result_type result_type;
  typedef typename Covariance_::first_argument_type first_argument_type;
  typedef typename Covariance_::second_argument_type second_argument_type;
  typedef first_argument_type Location;

  MarkovBayes_covariance() {}
 
  MarkovBayes_covariance( const Covariance_& C_I, double Bz ) 
    : C_I_( C_I ), Bz_( Bz ) {
    Bz_square_ = Bz_ * Bz_;
  }

  MarkovBayes_covariance( const MarkovBayes_covariance<Covariance_>& rhs ) {
    C_I_ = rhs.C_I_;
    Bz_ = rhs.Bz_;
    Bz_square_ = rhs.Bz_square_;
  }

  virtual ~MarkovBayes_covariance() {}
  virtual Covariance_set_impl<Covariance_>* clone() const {
    return new MarkovBayes_covariance( *this );
  }

  virtual result_type operator () ( unsigned int i, unsigned int j,  
                                    const Location& u1, 
                                    const Location& u2 ) const {
    
    // Markov-Bayes covariance model:
    // C_IY(h) = C_YI(h) = Bz * C_I(h)
    // C_Y(h)  = Bz^2 * C_I(h) if h > 0
    //         = |Bz| * C_I(h) if h = 0

    gstl_assert( i==1 || i==2 );
    gstl_assert( j==1 || j==2 );

    if( i==1 && j==1 )
      return C_I_( u1, u2 );

    if( i==1 || j==1 ) {
      return Bz_ * C_I_( u1, u2 );
    }

    if(  square_euclidean_distance(u1, u2) == 0.0 )
      return GsTL::abs( Bz_ ) * C_I_( u1, u2 );

    return Bz_square_ * C_I_( u1, u2 );
  }


private:
  Covariance_ C_I_;
  double Bz_;
  double Bz_square_;

};


#endif
