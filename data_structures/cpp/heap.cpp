#include "./heap.h"
#include <algorithm> //swap function
#include <iostream> // cerr and endl

bool heap::haveLeft(const unsigned i) const {
  if ( i*2 + 1 < this->V.length() ) {
    return true;
  }
  return false;
}

bool heap::haveRight(const unsigned i) const {
  if ( i*2 + 2 < this->V.length() ) {
    return true;
  }
  return false;
}

void heap::maxHeapify(unsigned i) {
  /* wrong call */
  if ( i >= this->V.length() ) {
    std::cerr << "WRONG CALL: maxHeapify(" << i << ")" << std::endl; 
    return;
  }
  
  if ( !haveLeft(i) ) {
    return;    
  }
  else {
    if ( !haveRight(i) ) {
      /* compare parent and left child */
      if ( this->V[i] < this->V[i*2 + 1] ) {
        std::swap(this->V[i], this->V[i*2 + 1]);
      }
      maxHeapify(i*2 + 1);
    }
    else {
      /* cool lines i guess */
      unsigned offset = this->V[i*2 + 1] > this->V[i*2 + 2] ? 1 : 2; 
      if ( this->V[i] < this->V[i*2 + offset] ) {
        std::swap(this->V[i], this->V[i*2 + offset]);
      }
      maxHeapify(i*2 + 1);
      maxHeapify(i*2 + 2);
    }
  }
  return; 
}

void heap::insert(const int data) {
  this->V.push_back(data);
  if (this->V.length() == 1) return;

  int j = this->V.length() - 1;
  while (j>0) {
    maxHeapify(j);
    j = (j-1)/2; 
  }
  maxHeapify(0);
}

int heap::extractMax() {
  int maxValue = this->V[0];
  std::swap(this->V[0], this->V[this->V.length() - 1]);
  this->V.pop_back();

  if (this->size() > 0)
    maxHeapify(0);

  return maxValue;
}

void heap::changeKey(unsigned i, int k) {
  this->V[i] = k;
  int j= (int) i;

  while (j>0) {
    maxHeapify(j);
    j = (j-1)/2; 
  }
  maxHeapify(0);
}

void heap::buildMaxHeap(vector V) {
  this->V = V;

  for (int i=(V.length()-1)/2; i>=0; i--) {
    maxHeapify(i); 
  }
}

vector& heapSort(vector&  V) {
  heap h(V);  

  V.clear();
  
  while (h.size() > 0) V.push_back(h.extractMax());

  return V;
}
