#include <iostream>
#include "data_structures/cpp/queue.h"

int main(void) {
  queue Q(5);
  Q.enqueue(1);
  Q.enqueue(2);
  Q.enqueue(3);
  Q.enqueue(4);
  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;

  vector V(10), U(10, 222);
  V.print_vector();
  U.print_vector();

  for(unsigned i=0; i<V.length(); i++) {
    V[i] = i + 1;
  }
  
  V.print_vector();

  for(unsigned i=0; i<100; i++) V.push_back(99);
    
  V.print_vector();

  std::cout << "U[3] = " << U[3] << std::endl;

  return 0;
}
