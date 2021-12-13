#include "worst_graph.h"
#include "edge_array.h"
#include "str_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/*
int main() {
  struct graph* G; 
  G = inicializar(10, true);

  inserirVertice(G, "natal");
  inserirVertice(G, "parnamirim");
  inserirVertice(G, "macaiba");
  inserirVertice(G, "mossoro");
  inserirVertice(G, "currais novos");
  inserirVertice(G, "acari");
  inserirVertice(G, "martins");
  inserirVertice(G, "acu");

  print_graph(G);


  return 0;
}
*/

void print_graph(struct graph* grafo) {
  printf("NODES: ");
  STRARRprint_arr(grafo->nodes);
  printf("EDGES: ");
  EDGARRprint_arr(grafo->edges);
  printf("\n");
}

bool saoConectados(struct graph* grafo, char* rotuloVOrigem, char* rotuloVDestino) {
  int id_origem, id_destino;
  id_origem = obterIndiceVertice(grafo, rotuloVOrigem);
  if (id_origem == -1) return false;
  id_destino = obterIndiceVertice(grafo, rotuloVDestino);
  if (id_destino == -1) return false;

  for (char i=0; i<grafo->edges->len; i++) {
    if (grafo->edges->edges[i].orig_id == id_origem && grafo->edges->edges[i].dest_id == id_destino)
      return true;
    if (grafo->edges->edges[i].dest_id == id_origem && grafo->edges->edges[i].orig_id == id_destino)
      return true;
  }
  return false;
}

void inserirVertice(struct graph* grafo, char* rotuloVertice) {
  STRARRappend(grafo->nodes, rotuloVertice);
}

int obterIndiceVertice(struct graph* grafo, char* rotuloVertice) {
  for (char i=0; i<grafo->nodes->len; i++) {
    if (strcmp(rotuloVertice, grafo->nodes->names[i]) == 0) {
      return i;
    }
  }
  return -1;
}

void inserirAresta(struct graph* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso) {
  int id_origem, id_destino;
  id_origem = obterIndiceVertice(grafo, rotuloVOrigem);
  if (id_origem == -1) return;
  id_destino = obterIndiceVertice(grafo, rotuloVDestino);
  if (id_destino == -1) return;

  struct edge E;
  E.dest_id = id_destino;
  E.orig_id = id_origem;
  E.weight = peso;
  // append edge to the edge array
  EDGARRappend(grafo->edges, E);
}

struct graph* inicializar(int numVertices, bool ponderado) {
  struct graph* grafo;
  grafo = (struct graph*) malloc(sizeof(void*));
  
  grafo->nodes = (struct str_arr*) malloc(sizeof(void*));
  STRARRallocate(grafo->nodes, numVertices);
  grafo->edges = (struct edge_arr*) malloc(sizeof(void*));

  return grafo;
}
