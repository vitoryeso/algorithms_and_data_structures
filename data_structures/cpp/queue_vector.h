#include "vector.h"

class queue {
private:
  vector V;
  unsigned size;
public:
  queue(): V(), size(0) {}
  queue(const unsigned capacity);

  void enqueue(const int value);
  int dequeue();

  const int next();
};
