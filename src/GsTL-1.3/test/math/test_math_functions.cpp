#include <GsTL/math/math_functions.h>

#include <iostream>

int main() {
  std::cout << "round(0.2)= " << GsTL::round( 0.2 ) << std::endl;
  std::cout << "round(-0.2)= " << GsTL::round( -0.2 ) << std::endl;
  std::cout << "round(-1.6)= " << GsTL::round( -1.6 ) << std::endl;
  std::cout << "round(0.7)= " << GsTL::round( 0.7 ) << std::endl;
  std::cout << "round(-2.5)= " << GsTL::round( -2.5 ) << std::endl;
  return 0;
}
