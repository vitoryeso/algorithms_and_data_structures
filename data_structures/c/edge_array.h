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

void EDGARRfree_all(struct edge_arr* arr);
void EDGARRcopy(struct edge_arr* arr, struct edge_arr* arr2);
void EDGARRallocate(struct edge_arr* arr, unsigned cap);
void EDGARRduplicate_mem(struct edge_arr* arr);

void EDGARRprint_arr(struct edge_arr* arr);
void EDGARRprint_edge(struct edge* E);
void EDGARRappend(struct edge_arr* arr, struct edge E);
void EDGARRremove_end(struct edge_arr* arr);
bool EDGARRremove_elem(struct edge_arr* arr, unsigned i);
bool EDGARRinsert_elem(struct edge_arr* arr, struct edge E, unsigned i);

#endif
