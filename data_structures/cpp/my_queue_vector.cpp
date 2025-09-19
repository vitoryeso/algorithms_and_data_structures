#include "./my_queue_vector.h"
#include <iostream>

MyQueueVector::MyQueueVector(const unsigned capacity) {
  if(capacity > 0) {
    MyVector prov(capacity);
    V = prov;
    size = 0;
  }
}

void MyQueueVector::enqueue(const int value) {
  unsigned prov(V.length());
  if(size < prov) {
    V[prov - size - 1] = value;
    size++;
    return;
  } 
}

int MyQueueVector::dequeue() {
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

const int MyQueueVector::next() {
  return V[V.length() - 1]; 
}
