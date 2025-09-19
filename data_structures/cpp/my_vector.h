#ifndef MY_VECTOR_H
#define MY_VECTOR_H

class MyVector {
private:
  int* start;
  unsigned size;
  unsigned extend;

  /* just copy the parameter vector to this vector. assume which the memory already been allocated. */
  void copy(const MyVector &V);
  /* alloc space for the vector. assume a void vector */ 
  void alloc(const unsigned size);
  /* delete the memory allocated by the vector */
  void del();

public:
  MyVector(): start(nullptr), size(0), extend(10) {}

  /* init a MyVector with the same value */
  MyVector(const unsigned size, const int value);
  inline MyVector(const unsigned size) {if(size > 0) alloc(size);}
  inline MyVector(int* ptr, const unsigned size) {this->start = ptr; this->size = size;}
  MyVector(const MyVector& V);

  inline unsigned length() const {return size;}

  void operator=(const MyVector& V);
  inline int operator[](const unsigned i) const {return start[i];}
  inline int& operator[](const unsigned i) {return start[i];}

  void push_back(const int value);
  void pop_back();

  inline const int back() const {return start[size-1];}
  inline const int front() const {return start[0];}

  inline bool empty() const {return size == 0 ? true : false;} 
  inline void clear() {if(size > 0) del();}
  void print_vector() const;
};

#endif
