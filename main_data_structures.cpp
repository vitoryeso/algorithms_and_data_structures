#include <iostream>
//#include "data_structures/cpp/queue_linkedlist.h"
//#include "data_structures/cpp/heap.h"
#include  "data_structures/cpp/trees.h"

int main(void) {
  BST tree; 
  tree.insert(10);
  tree.insert(1);
  tree.insert(15);
  tree.insert(40);
  tree.insert(4);
  tree.insert(19);
  tree.insert(61);

  std::cout << "inserting 10, 1, 15, 40, 4, 19, 61 " << std::endl;

  std::string prov = tree.have(15) ? "yes" : "no";
  std::cout << "have 15 ? " << prov << std::endl;

  prov = tree.have(1) ? "yes" : "no";
  std::cout << "have 1 ? " << prov << std::endl;

  prov = tree.have(18) ? "yes" : "no";
  std::cout << "have 18 ? " << prov << std::endl;

  int provint = tree.findMin();
  std::cout << "min: " << provint << std::endl;

  provint = tree.findMax();
  std::cout << "max: " << provint << std::endl;

  return 0;
}
