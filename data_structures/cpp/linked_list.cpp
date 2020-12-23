#include <iostream>

template <typename T>
struct Node{
  T data;
  Node *next;
};

template <typename T>
class List {

private:
  Node<T>* head;
  Node<T>* tail;
public:
  List();
  ~List();

  unsigned search(T data) const;
  T at(unsigned i) const;
  void insert(T data, unsigned k);
  void remove(T data, unsigned k);
};

template <typename T>
T List<T>::at(unsigned i) const {
  //
  T k;
  return k;
}

template <typename T>
List<T>::List() {
  this->head = nullptr;
  this->tail = nullptr;
}

template <typename T>
List<T>::~List() {
  while(head != nullptr) {
    Node<T>* tmp = head;
    head = head->next;
    delete tmp;
  }
}

template <typename T>
void List<T>::insert (T data, unsigned k) {
  if(head == nullptr) { /* if the list is empty */
    Node<T> tmp = new Node<T>;
    tmp.data = data;
    tmp.next = nullptr;
    
    tail = &tmp;
    head = &tmp;
  } 
}
