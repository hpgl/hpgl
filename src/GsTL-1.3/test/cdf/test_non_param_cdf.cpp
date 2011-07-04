#include <GsTL/cdf/non_param_cdf.h>
#include <GsTL/cdf/categ_non_param_cdf.h>

#include <GsTL/univariate_stats.h>


#include <iostream>
#include <vector>
#include <cmath>

/*
class middle_interp{
public:
  double z(double z1, double p1, double z2, double p2, double p) const{
    cout << "interpolator" << std::endl
	 << z1 << " " << p1 << " " << z2 << " " << p2 << " " << p << std::endl;
    return ( z1*(p1-p2) - (z1-z2)*(p1-p) ) / (p1-p2);
  }
  
  double p(double z1, double p1, double z2, double p2, double z) const{
    return ( p1*(z1-z2) - (z1-z)*(p1-p2) ) / (z1-z2);
  }
};

class lower_tail_interp{
public:
  double z(double z1, double p1, double p) const{
    return (p*z1) / p1;
  }

  double p(double z1, double p1, double z) const{
    return (z*p1) / z1;
  }
};


// exponential behavior
class upper_tail_interp{
public:
  double z(double z1, double p1, double p) const{
    return z1+sqrt( log( (1-p1)/(1-p) ) );
  }

  double p(double z1, double p1, double z) const{
    return 1-(1-p1)*exp(-pow((z-z1),2));
  }
};
*/



int main(){

  std::cout << std::endl 
       << "___________________________________________________" << std::endl
       << "categorical non param cdf" << std::endl ;

  double prob[5] = {0.2, 0.1, 0.4, 0.3};

  Categ_non_param_cdf<> cdf1(4);
  Categ_non_param_cdf<> cdf2(4, prob);
  
  cdf1.p_set(prob, prob+4);

  std::cout << "prob of Z=2: " << cdf1.prob(2) <<std::endl;

  std::cout << "cdf.inverse(1):  Zk=" << cdf1.inverse(0.7) << std::endl;



  std::cout << std::endl 
       << "___________________________________________________" << std::endl
       << "continuous non param cdf" << std::endl ;

  double Z[5]= {2.3, 4.4, 5.5, 6.6, 7.7};
  double P[5]= {0.2, 0.34, 0.4, 0.7, 0.87};

  typedef Non_param_cdf<> Cont_cdf;
  typedef Non_param_cdf<> Default_cont_cdf;


  Default_cont_cdf  cdf3(Z, Z+5, P);

  std::cout << "cdf3:" << std::endl
       << "Z: ";
  for(Cont_cdf::z_iterator zit = cdf3.z_begin(); zit != cdf3.z_end(); zit++)
    std::cout << *zit << " ";
  std::cout << std::endl 
       << "P: ";
  for(Cont_cdf::p_iterator pit=cdf3.p_begin(); pit!=cdf3.p_end(); pit++)
    std::cout << *pit << " ";
  std::cout << std::endl << std::endl;

  std::cout << "P(Z<=9.9) = " << cdf3.prob(9.9) << std::endl
       << "z st P(Z<=z)=0.7 : " << cdf3.inverse(0.7) << std::endl;

  std::cout << "P(Z<=4.4) = " << cdf3.prob(4.4) << std::endl;

}
