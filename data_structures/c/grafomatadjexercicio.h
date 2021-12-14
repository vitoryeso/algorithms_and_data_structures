#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct GrafoMatrizAdj {
    int** arestas;
    char** rotuloVertices;
    int verticesInseridos;
    int maxNumVertices;     
    //faz mais sentido ser apenas numVertices (consertar para a prox turma)
};

//struct GrafoMatrizAdj* inicializar(int numVertices, bool ponderado);
//void print_graph(struct GrafoMatrizAdj* grafo);
//void inserirVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice);
//void inserirAresta(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso);

/*
int main() {
  struct GrafoMatrizAdj* G;
  G = inicializar(10, false);
  inserirVertice(G, "natal");
  inserirVertice(G, "paraiba");
  inserirVertice(G, "joao pessoa");
  inserirAresta(G, "natal", "paraiba", 0);
  inserirAresta(G, "natal", "joao pessoa", 0);
  inserirAresta(G, "fsadf", "fgdasgfasd", 0);

  print_graph(G);

  return 0;
}

void print_graph(struct GrafoMatrizAdj* grafo) {
  for (int i=0; i<grafo->maxNumVertices; i++) {
    printf("| %s |", grafo->rotuloVertices[i]);
  }
  printf("\n");
  for (int i=0; i<grafo->maxNumVertices; i++) {
    for (int j=0; j<grafo->maxNumVertices; j++) {
      printf(" %d ", grafo->arestas[i][j]);
    }
    printf("\n");
  }
}
*/

/**
 * Se o grafo for ponderado, inicializamos cada posicao da matriz com INT_MAX.
 * Se o grafo for não ponderado, inicializamos cada posicao da matriz com 0.
 **/
struct GrafoMatrizAdj* inicializar(int numVertices, bool ponderado) {
  struct GrafoMatrizAdj* grafo;
  grafo = (struct GrafoMatrizAdj*) malloc(sizeof(void*));
  grafo->maxNumVertices = numVertices;
  grafo->verticesInseridos = 0;
  grafo->rotuloVertices = (char**) malloc(sizeof(void*) * numVertices);
  grafo->arestas = (int**) malloc(sizeof(void*) * numVertices);
  for (int i=0; i<numVertices; i++) {
    grafo->arestas[i] = (int*) malloc(sizeof(int) * numVertices);
    for (int j=0; j<numVertices; j++) {
      if (ponderado) {
        grafo->arestas[i][j] = INT_MAX;
      }
      else {
        grafo->arestas[i][j] = 0;
      }
    }
  }
  return grafo;
}

/**
 * A principio nao temos nenhuma ordenacao usando os rotulos.
 * Portanto, usaremos busca linear
 **/
int obterIndiceVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
  for (int i=0; i<grafo->verticesInseridos; i++) {
    if (!strcmp(rotuloVertice, grafo->rotuloVertices[i])) {
      return i;
    }
  }
  return -1;
}

/**
 * Se o grafo for ponderado, usamos a variavel peso para especificar o peso da aresta.
 * Se o grafo for não ponderado, passaremos o valor 1 para a variavel peso, de modo que represente existência da aresta.
 * Se um dos rotulos dos vertices nao existir, simplesmente nao faremos nada.
 **/
void inserirAresta(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso) {
  int orig_id, dest_id;
  orig_id = obterIndiceVertice(grafo, rotuloVOrigem);
  dest_id = obterIndiceVertice(grafo, rotuloVDestino);
  if (orig_id < 0 || dest_id < 0) return;
  
  if (grafo->arestas[orig_id][dest_id] == INT_MAX) {
    grafo->arestas[orig_id][dest_id] = peso;
  }
  else {
    grafo->arestas[orig_id][dest_id] = 1;
  }
}

/**
 * Usamos a variavel grafo->verticesInseridos para controlar a quantidade de vertices que ja foi inserido no grafo.
 * Logo, ela pode/deve ser usada para especificar em qual posicao o vertice a ser inserido sera alocado.
 **/
void inserirVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
  if (grafo->verticesInseridos >= grafo->maxNumVertices) return;
  grafo->rotuloVertices[grafo->verticesInseridos++] = rotuloVertice;
}

/**
 * Tomar cuidado com as diferenças semânticas entre haver conexão entre dois vertices em um grafo ponderado,
 * e haver conexão entre dois vertices em um grafo nao-ponderado.
 **/
bool saoConectados(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino) {
  int orig_id, dest_id;
  orig_id = obterIndiceVertice(grafo, rotuloVOrigem);
  dest_id = obterIndiceVertice(grafo, rotuloVDestino);
  if (orig_id < 0 || dest_id < 0) return false;
  
  if (grafo->arestas[orig_id][dest_id] == INT_MAX) return false;
  if (grafo->arestas[orig_id][dest_id] == 0) return false;
  return true;
}
