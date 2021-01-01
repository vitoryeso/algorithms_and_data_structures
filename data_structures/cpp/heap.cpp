#include "./heap.h"

bool heap::haveLeft(const unsigned i) const {
  if ( i*2 + 1 < this->V.size ) {
    return true;
  }
  return false;
}

bool heap::haveRight(const unsigned i) const {
  if ( i*2 + 2 < this->V.size ) {
    return true;
  }
  return false;
}

void heap::maxHeapify(unsigned i) {
  /* wrong call */
  if ( i >= this->V.size ) {
    cerr << "WRONG CALL: maxHeapify(" << i << ")" << endl; 
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

void buildMaxHeap( 
