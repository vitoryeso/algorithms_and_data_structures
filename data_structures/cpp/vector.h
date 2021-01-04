class vector {
private:
  int* start;
  unsigned size;
  unsigned extend;

  /* just copy the parameter vector to this vector. assume which the memory already been allocated. */
  void copy(const vector &V);
  /* alloc space for the vector. assume a void vector */ 
  void alloc(const unsigned size);
  /* delete the memory allocated by the vector */
  void del();

public:
  vector(): start(nullptr), size(0), extend(10) {}

  /* init a vector with the same value */
  vector(const unsigned size, const int value);
  inline vector(const unsigned size) {if(size > 0) alloc(size);}
  inline vector(int* ptr, const unsigned size) {this->start = ptr; this->size = size;}
  vector(const vector& V);

  inline unsigned length() const {return size;}

  void operator=(const vector& V);
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
