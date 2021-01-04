#include "./vector.h"
#include <iostream>

void vector::copy(const vector& V) {
  for(unsigned i=0; i<size; i++) this->start[i] = V[i];
}

void vector::operator=(const vector& V) {
  if (this->size > 0) {
    this->del(); 
  }
  this->alloc(V.length()); 
  this->copy(V);
}

void vector::alloc(const unsigned size) {
  if(size == 0) return;
  else start = new int[size + 10]; 
  this->size = size;
  this->extend = 10;
}

void vector::del() {
  delete []start;
  size = 0;
}

void vector::push_back(int value) {
  if(size == 0) {
    alloc(1);
    start[0] = value;
    return;
  }
  if(extend > 0) {
    start[size] = value;
    size++;
    extend--; 
  }
  else {
    vector prov_vector(start, size);
    alloc(prov_vector.length());
    copy(prov_vector);
    start[size] = value;
    size++;
    extend--; 
  }
}

void vector::pop_back() {
  if(size > 0) {
    size--;
    extend++;
  }
}

vector::vector(const vector& V) {
  /* its a constructor. dont need to use del function */
  this->alloc(V.length());
  this->copy(V);
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



