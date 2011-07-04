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

  std::ifstream infile( "cluster.dat" );
  if( !infile ) {
    std::cerr << "can't open file" << std::endl;
    return 1;
  }

  std::vector<float> distrib;
  std::copy( std::istream_iterator<float>( infile ),  std::istream_iterator<float>(),
	     std::back_inserter( distrib ) );



  Gaussian_cdf normal(0,1);


  std::ofstream out1( "original_distribution.result" );
  std::copy( distrib.begin(), distrib.end(), 
	     std::ostream_iterator<double>( out1, "\n" ) );

  cdf_transform(distrib.begin(), distrib.end(), normal);

  std::ofstream out2( "transformed_distribution.result" );
  std::copy( distrib.begin(), distrib.end(), 
	     std::ostream_iterator<double>( out2, "\n" ) );


}
