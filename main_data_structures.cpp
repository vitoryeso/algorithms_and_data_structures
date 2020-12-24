#include <iostream>
#include "data_structures/cpp/queue_linkedlist.h"

int main(void) {
  queue Q;
  Q.enqueue(1);
  Q.enqueue(2);
  Q.enqueue(3);
  Q.enqueue(4);

  std::cout << "Queue is empty?: " << Q.isEmpty() << std::endl;
  std::cout << "Queue size: " << Q.size() << std::endl;

  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;
  std::cout << Q.dequeue() << std::endl;

  std::cout << "Queue is empty?: " << Q.isEmpty() << std::endl;
  std::cout << "Queue size: " << Q.size() << std::endl;

  return 0;
}
