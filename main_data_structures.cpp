#include <iostream>
#include "data_structures/cpp/vector.h"

int main(void) {
  vector V(10), U(10, 222);
  V.print_vector();
  U.print_vector();

  for(unsigned i=0; i<V.length(); i++) {
    V[i] = i + 1;
  }
  
  V.print_vector();

  std::cout << "U[3] = " << U[3] << std::endl;

  return 0;
}
