#ifndef _WORST_GRAPH_
#define _WORST_GRAPH_

#include "str_array.h"
#include "edge_array.h"

struct graph {
  struct str_arr* nodes;
  struct edge_arr* edges;
};

void WGprint_graph(struct graph* grafo);

/**
* Se o grafo for ponderado, inicializamos cada posicao da matriz com INT_MAX.
* * Se o grafo for não ponderado, inicializamos cada posicao da matriz com 0.
* **/
struct graph* WGinicializar(int numVertices, bool ponderado);

/**
 *  * A principio nao temos nenhuma ordenacao usando os rotulos.
 *  * Portanto, usaremos busca linear
 *  **/
int WGobterIndiceVertice(struct graph* grafo, char* rotuloVertice);

/**
 *  * Se o grafo for ponderado, usamos a variavel peso para especificar o peso da aresta.
 *  * Se o grafo for não ponderado, passaremos o valor 1 para a variavel peso, de modo que represente existência da aresta.
 *  * Se um dos rotulos dos vertices nao existir, simplesmente nao faremos nada.
 *  **/
void WGinserirAresta(struct graph* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso);

/**
 *  * Usamos a variavel grafo->verticesInseridos para controlar a quantidade de vertices que ja foi inserido no grafo.
 *  * Logo, ela pode/deve ser usada para especificar em qual posicao o vertice a ser inserido sera alocado.
 *  **/
void WGinserirVertice(struct graph* grafo, char* rotuloVertice);

/**
 *  * Tomar cuidado com as diferenças semânticas entre haver conexão entre dois vertices em um grafo ponderado,
 *  * e haver conexão entre dois vertices em um grafo nao-ponderado.
 *  **/
bool WGsaoConectados(struct graph* grafo, char* rotuloVOrigem, char* rotuloVDestino);

#endif
