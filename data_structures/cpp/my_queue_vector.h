#include "my_vector.h"

class MyQueueVector {
private:
  MyVector V;
  unsigned size;
public:
  MyQueueVector(): V(), size(0) {}
  MyQueueVector(const unsigned capacity);

  void enqueue(const int value);
  int dequeue();

  const int next();
};
