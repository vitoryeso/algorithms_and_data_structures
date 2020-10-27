class vector {
private:
  int* start;
  unsigned size;

  /* just copy the parameter vector to this vector. assume which the memory already been allocated. */
  void copy(const vector &V);

  /* alloc space for the vector. assume a void vector */
  void alloc(const unsigned size);

  /* delete the memory allocated by the vector */
  void del();

public:
  vector(): start(nullptr), size(0) {}

  /* init a vector with the same value */
  vector(const unsigned size, const int value);
  inline vector(const unsigned size) {if(size > 0) alloc(size);}
  vector(const vector& V);

  inline unsigned length() const {return size;}

  inline int operator[](const unsigned i) const {return start[i];}
  inline int& operator[](const unsigned i) {return start[i];}

  void print_vector() const;
};
