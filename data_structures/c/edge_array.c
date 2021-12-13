#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./edge_array.h"

void EDGARRfree_all(struct edge_arr* arr) {
  if (arr->cap > 0) {
    free(arr->edges);
  }
  arr->len = 0;
  arr->cap = 0;
}

/* arr => arr2 */
void EDGARRcopy(struct edge_arr* arr, struct edge_arr* arr2) {
  for(char i=0; i<arr->len; i++) {
    arr2->edges[i] = arr->edges[i];
    arr2->len = arr2->len >= arr->len ? arr2->len : arr->len;
  }
}

void EDGARRallocate(struct edge_arr* arr, unsigned cap) {
  arr->edges = ( struct edge* ) malloc(cap * sizeof( void * ));
  arr->cap = cap;
  arr->len = 0;
}

void EDGARRduplicate_mem(struct edge_arr* arr) {
  if (arr->len <= 0 || arr->cap <= 0) {
    EDGARRallocate(arr, 5);
    return;
  }
    
  struct edge_arr* prov;
  prov = (struct edge_arr*) malloc(sizeof(void*));

  unsigned prov_cap = arr->cap;

  EDGARRallocate(prov, arr->cap);
  EDGARRcopy(arr, prov);

  EDGARRfree_all(arr);

  EDGARRallocate(arr, prov_cap * 2);
  EDGARRcopy(prov, arr);
}

void EDGARRprint_edge(struct edge* E) {
  printf("%d => %d; peso %d\n", E->orig_id, E->dest_id, E->weight);
}

void EDGARRprint_arr(struct edge_arr* arr) {
  if (arr->len <= 0) {
    printf("array vazio!\n");
    return ;
  }
  for(char i=0; i<arr->len; i++) {
    EDGARRprint_edge(&arr->edges[i]);
  }
}

void EDGARRappend(struct edge_arr* arr, struct edge E) {
  if (arr->len + 1 > arr->cap) {
    EDGARRduplicate_mem(arr);
  }
  arr->edges[arr->len] = E;
  arr->len += 1;
}

void EDGARRremove_end(struct edge_arr* arr) {
  if (arr->len >0)
    arr->len -= 1;
}

bool EDGARRremove_elem(struct edge_arr* arr, unsigned i) {
  if (arr->len <= i) return false;
  arr->len -= 1;
  for (char j=i; j<arr->len; j++) {
    arr->edges[j] = arr->edges[j + 1];
  }
  return true;
}

bool EDGARRinsert_elem(struct edge_arr* arr, struct edge E, unsigned i) {
  if (arr->len <= i) return false;
  if (arr->len + 1 >= arr->cap) EDGARRduplicate_mem(arr);
  for (char j=arr->len - 1; j>0; j--) {
    arr->edges[j + 1] = arr->edges[j];
  }
  arr->edges[i] = E;
  arr->len += 1;
  return true;
}

/*
int main() {
  struct edge_arr* lu_edges;
  struct edge* lu_edge;

  lu_edge->dest_id = 0;
  lu_edge->orig_id = 3;
  lu_edge->weight = 10;

  lu_edges = (struct edge_arr*) malloc(sizeof(void*));
  allocate(lu_edges, 5);
  print_arr(lu_edges);
  printf("cap %d\n", lu_edges->cap);
  printf("len %d\n", lu_edges->len);

  append(lu_edges, *lu_edge);
  print_arr(lu_edges);
  printf("cap %d\n", lu_edges->cap); printf("len %d\n", lu_edges->len);
  
  return 0;
}
*/

