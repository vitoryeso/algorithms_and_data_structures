#include <iostream>
//#include "data_structures/cpp/queue_linkedlist.h"
#include "data_structures/cpp/heap.h"

int main(void) {
  vector V;
  V.push_back(20);
  V.push_back(10);
  V.push_back(1);
  V.push_back(3);
  V.push_back(4);
  V.push_back(5);
  V.push_back(3);
  V.push_back(29);
  V.push_back(10);
  V.push_back(14);

  heap h(V);

  h.print();

  vector sortedVec = heapSort(V);
  sortedVec.print_vector();

  return 0;
}
