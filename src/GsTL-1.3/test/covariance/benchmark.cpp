#include <GsTL/geometry/covariance_table_2.h>

#include <GsTL/matrix_library/tnt/stopwatch.h>

#include <iostream>


int main() {

  int nx = 100;
  int ny = 100;
  int nz = 100;

  Table_impl table(nx,ny,nz);
  
  float* array = table.raw_access();

  for(long int i=0; i<nx*ny*nz; i++) {
    array[i] = i;
  }

  TNT::stopwatch chrono;
  chrono.start();
  for(int k=0; k<nz; k++)
    for(int j=0; j<ny; j++)
      for(int i=0; i<nx; i++)
	table.value_at(i,j,k); 
  
  chrono.stop();
  std::cout << "time elapsed: " << chrono.read() << std::endl;

  chrono.reset();
  chrono.start();
  for(int k=0; k<nz; k++)
    for(int j=0; j<ny; j++)
      for(int i=0; i<nx; i++)
	table.slow_access(i,j,k); 
 
  chrono.stop();
  std::cout << "time elapsed: " << chrono.read() << std::endl;

  return 0;
}
