#include "./vector.h"
#include <iostream>

void vector::copy(const vector& V) {
  for(unsigned i=0; i<size; i++) this->start[i] = V[i];
}

void vector::alloc(const unsigned size) {
  if(size == 0) return;
  else start = new int[size]; 
  this->size = size;
}

void vector::del() {
  delete []start;
  size = 0;
}

vector::vector(const vector& V) {
  if(V.length() == 0) return;
  if(this->size > 0) {
    del();
  }
  alloc(V.length());
  copy(V);
}

vector::vector(const unsigned size, const int value) {
  if(size == 0) return; 
  else {
    alloc(size);
    for(unsigned i=0; i<size; i++) {
      start[i] = value;
    }
  }
}

void vector::print_vector() const {
  std::cout << "V = [";
  if(size == 0) {
    std::cout << "];\n";
  } 
  else {
    for(unsigned i=0; i<size; i++) {
      if(i == size-1) {
        std::cout << start[i] << "];\n";
      }
      else {
        std::cout << start[i] << " ";
      }
    }
  }
}



