#include "./queue_vector.h"
#include <iostream>

queue::queue(const unsigned capacity) {
  if(capacity > 0) {
    vector prov(capacity);
    V = prov;
    size = 0;
  }
}

void queue::enqueue(const int value) {
  unsigned prov(V.length());
  if(size < prov) {
    V[prov - size - 1] = value;
    size++;
    return;
  } 
}

int queue::dequeue() {
  if(size <= 0) {
    std::cerr << "error: void queue\n";
    return -1;
  }
  else {
    int prov(V[V.length() - 1]);
    V.pop_back();  
    size--;
    return prov;
  }
}

const int queue::next() {
  return V[V.length() - 1]; 
}
