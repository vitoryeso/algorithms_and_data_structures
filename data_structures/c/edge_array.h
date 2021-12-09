#ifndef _EDGE_ARRAY_
#define _EDGE_ARRAY_

struct edge {
  unsigned orig_id;
  unsigned dest_id;
  int weight;
};

struct edge_arr {
  struct edge* edges;
  unsigned len;
  unsigned cap;
};

void free_all(struct edge_arr* arr);
void copy(struct edge_arr* arr, struct edge_arr* arr2);
void allocate(struct edge_arr* arr, unsigned cap);
void duplicate_mem(struct edge_arr* arr);

void print_arr(struct edge_arr* arr);
void print_edge(struct edge* E);
void append(struct edge_arr* arr, struct edge E);
void remove_end(struct edge_arr* arr);
bool remove_elem(struct edge_arr* arr, unsigned i);
bool insert_elem(struct edge_arr* arr, struct edge E, unsigned i);

#endif
