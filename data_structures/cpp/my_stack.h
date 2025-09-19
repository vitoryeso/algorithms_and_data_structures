#include "my_vector.h"

class MyStack {
private:
  MyVector V;
public:
  MyStack(): V() {}

  inline void push(const int value) {V.push_back(value);}

  const int pop() {
    int value(V.back()); 
    V.pop_back();
    return value;
  }

  inline int seek() const {return V.back();}
};
