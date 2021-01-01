#include "./vector.h"

/* implementing heap with array approach
 * we want keep max-heap property */
class heap {
  private:
    vector V;

    bool haveLeft(unsigned i) const;
    bool haveRight(unsigned i) const;
    void buildMaxHeap(int* arr); /* build a maxheap from an arbitrary array */
    void maxHeapify(unsigned i); /* fix a local heap to maxheap property recursively */

  public:
    inline heap(vector& V) {this->V(V);}

    void insert(int data); /* insert an element */
    inline int max() const {return V.back();} /* return max value (first element) */
    int extractMax(); /* return and remove max value */
    void changeKey(unsigned i, int k); /* change the key of the i element */
}
