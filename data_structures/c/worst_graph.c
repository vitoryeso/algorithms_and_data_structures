#include "worst_graph.h"
#include "edge_array.h"
#include "str_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct graph* inicializar(int numVertices, bool ponderado);

int main() {
  struct graph* G; G = inicializar(10, true);

  STRARRprint_arr(G->nodes);
  EDGARRprint_arr(G->edges);

  return 0;
}

struct graph* inicializar(int numVertices, bool ponderado) {
  struct graph* grafo;
  grafo = (struct graph*) malloc(sizeof(void*));
  
  grafo->nodes = (struct str_arr*) malloc(sizeof(void*));
  STRARRallocate(grafo->nodes, numVertices);
  grafo->edges = (struct edge_arr*) malloc(sizeof(void*));

  return grafo;
}
