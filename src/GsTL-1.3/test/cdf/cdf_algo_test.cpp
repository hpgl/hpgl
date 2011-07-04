#include <GsTL/cdf/categ_non_param_cdf.h>
#include <GsTL/cdf/gaussian_cdf.h>


#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>


#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/cdf/non_param_cdf.h>
#include <GsTL/cdf/interpolators.h>
#include <GsTL/univariate_stats/build_cdf.h>
#include <GsTL/univariate_stats/cdf_transform.h>


double uniform_random(){
  double p=drand48();
  if(p<0.5)
    return -3*(0.5-p);
  else
    return 3*(p-0.5);
}



struct uniform_cdf{
  typedef double value_type; 
  double prob(double z) const {
    double p = 1.0/4.0 * z + 0.5;
    if(!(p<=1.0 && p>= 0.0))
      std::cerr << "P= " << p << "  z= " << z << std::endl;
    return p;
  }
  
  double inverse(double p) const {
    return 4*(p-0.5);
  }
};




int main(){
  
  Gaussian_cdf normal_cdf(0,1);

  typedef Non_param_cdf<> non_parametric_cdf;


  std::vector<double> uniform_values;

  non_parametric_cdf from;


  for(int i=1; i<=1000; i++)
    uniform_values.push_back( uniform_random() );


  std::vector<double> tmp(uniform_values);
  
  // add two extreme values
  tmp.push_back(-4.0);
  tmp.push_back(4.0);
  
  build_cdf(tmp.begin(), tmp.end(), from, 100);

  // don't transform the extremes of the data set tmp
  cdf_transform(tmp.begin()+1, tmp.end()-1,
                from, normal_cdf);


  // second method
  std::vector<double> tmp2(uniform_values);
  uniform_cdf reference_cdf;

  cdf_transform(tmp2.begin(), tmp2.end(),
                reference_cdf, normal_cdf);
  


  std::cout << "transform\n3\nunif\nnormal1\nnormal2" << std::endl;
  for(int i=1; i<=1000; i++)
    std::cout << uniform_values[i-1] << "  " << tmp[i-1] << "  " << tmp2[i-1] << std::endl;
  

  

}
