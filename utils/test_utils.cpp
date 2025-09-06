#include "test_utils.h"
#include "../data_structures/cpp/graph_adj_list.cpp"

void inserirVertices(GrafoListaAdj* grafo, int ini, int fim) {
    for (int i = ini; i <= fim; i++) {
        std::string rotulo;
        std::stringstream sstm;
        sstm << "v" << i;
        rotulo = sstm.str();
        grafo->inserirVertice(rotulo);
    }
}
