#include <iostream>
#include "queue_linkedlist.h"

queue::queue() {
  _head = nullptr; 
  _tail = nullptr;
}

queue::~queue() {
  while( _head != nullptr ) {
    Node* tmp = _head;
    _head = _head->next; 
    delete tmp;
  }
}

void queue::enqueue(int value) {
  /* adding the new node in the back of the list */
  Node* tmp = new Node; 
  tmp->data = value;
  tmp->next = nullptr;

  if ( this->isEmpty() ) {
    _head = tmp;
    _tail = tmp;
  }
  else {
    _tail->next = tmp;
    _tail = tmp;
  }

  return;
}

int queue::dequeue() {
  /* removing from the head of the list */
  if ( this->isEmpty() ) {
    throw "DEQUEUE ERROR!\n TRYING GET A LIST ELEMENT FROM AN EMPTY LIST";
  }
  
  Node* tmp = _head;
  int tmp_val = tmp->data;

  _head = _head->next;

  /* list is empty */
  if ( _head == nullptr ) {
    _tail = nullptr;
  }

  delete tmp;
  return tmp_val;
}

bool queue::isEmpty() const {
  return _head == nullptr; 
}

unsigned queue::size() const {
  Node* tmp = _head;
  unsigned count(0);

  while ( tmp != nullptr ) {
    count++; 
    tmp = tmp->next;
  }

  return count;
}

int queue::head() const {
  return _head->data; 
}

int queue::tail() const {
  return _tail->data;
}
