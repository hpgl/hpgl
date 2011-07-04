#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/cdf/categ_non_param_cdf.h>
#include <GsTL/cdf/non_param_pdf.h>

#include <GsTL/univariate_stats.h>

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>

int main() {

  std::cout << "____________________________________" << std::endl
	    << "  Testing transformations from cdf to pdf and inversely" <<std::endl;
 
  int zval[5] = {1,2,3,4,5};
  double prob[5] = {0.2, 0.3, 0.6, 0.8, 1};

  typedef Non_param_pdf<int> Pdf;
  typedef Categ_non_param_cdf<int> Cdf;

  Cdf categ_cdf( 5, prob);
  
  Pdf pdf(zval, zval+5); 
  cdf_to_pdf(pdf, categ_cdf);

  for(int i=0; i<=4; i++)
    std::cout << pdf.prob(i) << "  " ;
  std::cout << std::endl;



  Cdf back(5);
  pdf_to_cdf(back,pdf);

  for(int i=0; i<=4; i++)
    std::cout << back.prob(i) << "  " ;
  std::cout << std::endl;



  cdf_to_pdf(pdf, back);
  for(int i=0; i<=4; i++)
    std::cout << pdf.prob(i) << "  " ;
  std::cout << std::endl;


  Pdf new_pdf(categ_cdf);
  for(int i=0; i<=4; i++)
    std::cout << new_pdf.prob(i) << "  " ;
  std::cout << std::endl;


  std::cout << std::endl
	    << "____________________________________" << std::endl
	    << "  Testing transformations from cdf to pdf and inversely "<<std::endl;
  
  Gaussian_cdf normal(0,1);
  std::vector<double> range(1000);
  for(std::vector<double>::iterator it=range.begin(); it!=range.end(); it++)
    *it = drand48()*100;

  std::ofstream out1( "uniform.dbg" );
  std::copy( range.begin(), range.end(), 
	     std::ostream_iterator<double>( out1, "\n" ) );
  cdf_transform(range.begin(), range.end(), normal);

  std::ofstream out2( "nscore.dbg" );
  std::copy( range.begin(), range.end(), 
	     std::ostream_iterator<double>( out2, "\n" ) );

  std::cout << std::endl
	    << "____________________________________" << std::endl
	    << "  Testing cdf correction "<<std::endl;
  
  double order_relation[5] = {0.2, 0.3, -0.2, 0.1, 1.2};
  Cdf bad_cdf(5, order_relation);
  std::cout << "Is bad cdf ok? " << is_valid_cdf(bad_cdf) << std::endl;
  make_cdf_valid(bad_cdf);

  std::cout << "Corrected cdf: " << std::endl;
  for(int i=0; i<bad_cdf.size(); i++)
    std::cout << bad_cdf.prob(i) << "  " ;
  std::cout << std::endl;


  std::cout << std::endl
	    << "____________________________________" << std::endl
	    << "  Testing cdf construction "<<std::endl;
  double val_range[9] = {1,1,1,4,7,12,31,55,60};
  double z_values[7] = {1,4,7,12,31,55,60};
  Non_param_cdf<> new_cdf( z_values, z_values+7);
  build_cdf( val_range, val_range+9, 
	     new_cdf.z_begin(), new_cdf.z_end(), new_cdf.p_begin() );
  Non_param_cdf<>::z_iterator z_it = new_cdf.z_begin();
  for( ; z_it != new_cdf.z_end(); z_it++ ) {
    std::cout << "P( " << *z_it << " ) = " << new_cdf.prob( *z_it ) << std::endl;
  }

  std::cout << std::endl;
  build_cdf( val_range, val_range+9, new_cdf, true );
  z_it = new_cdf.z_begin();
  for( ; z_it != new_cdf.z_end(); z_it++ ) {
    std::cout << "P( " << *z_it << " ) = " << new_cdf.prob( *z_it ) << std::endl;
  }
}
