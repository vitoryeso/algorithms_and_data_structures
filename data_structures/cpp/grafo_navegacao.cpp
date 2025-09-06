#include <iostream>
#include <vector>
#include <queue>
#include "../../utils/graph_utils.h"
using namespace std;


class GrafoListaAdj {
private:
    vector<string> vertices;

    //first é o indice do vertice, second é o peso (caso o grafo seja ponderado)
    vector<vector<pair<int, int>>> arestas;

    /**
    * A principio nao temos nenhuma ordenacao usando os rotulos.
    * Portanto, usaremos busca linear.
    **/
    int obterIndiceVertice(string rotuloVertice) {
        //IMPLEMENTAR
        for (unsigned i=0; i<vertices.size(); i++) {
            if (vertices[i] == rotuloVertice) {
                return i;
            }
        }
        return -1;
    }

    /**
    * O argumento indicesVerticesVisitados serve para controlar quais
    * vertices já foram visitados.
    * Lembrando que DFS é uma função recursiva.
    **/
    void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
        //IMPLEMENTAR
        int idx = obterIndiceVertice(rotuloVOrigem);
        if (idx == -1) {
            cout << "vertice nao existe!" << endl;
            return;
        }

        // se o rotulo ja tiver sido visitado retorna
        if (indicesVerticesVisitados[idx]) {
            return;
        }
        indicesVerticesVisitados[idx] = true;
        int edge_idx;
        for (unsigned i=0; i<arestas[idx].size(); i++) {
            edge_idx = arestas[idx][i].first; 
            dfs(vertices[edge_idx], indicesVerticesVisitados);
        }
    }

public:
    /**
    * Lembrem-se:
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vértice, precisaremos
    *          inserir um vetor para representar as conexões daquele
    *          vértice na lista de adjacências
    **/
    void inserirVertice(string rotuloVertice) {
        //IMPLEMENTAR
        if (obterIndiceVertice(rotuloVertice) != -1) {
            return;
        }

        vertices.push_back(rotuloVertice);
        vector<pair<int, int>> new_vertice_edges;
        arestas.push_back(new_vertice_edges);
    }

    /**
    * Sempre que o grafo for não ponderado, adicionaremos o peso 1,
    * por conveniência.
    **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, peso);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, peso);
    }

    /**
    * Inserir uma aresta entre rotuloVOrigem e rotuloVDestino com o peso
    * especificado.
    **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        //IMPLEMENTAR
        int idx_beg = obterIndiceVertice(rotuloVOrigem);
        int idx_end = obterIndiceVertice(rotuloVDestino);
        if (idx_beg == -1 || idx_end == -1)
            return;

        pair<int, int> edge;

        edge.first = idx_end;
        edge.second = peso;

        arestas[idx_beg].push_back(edge);
    }

    /**
    * Verifica se vértice rotuloVOrigem e vértice rotuloVDestino são
    * conectados (vizinhos).
    **/
    bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        int idx_beg = obterIndiceVertice(rotuloVOrigem);
        int idx_end = obterIndiceVertice(rotuloVDestino);
        if (idx_beg == -1 || idx_end == -1)
            return false;

        for (unsigned i=0; i<arestas[idx_beg].size(); i++) {
            if (arestas[idx_beg][i].first == idx_end) {
                return true;
            }
        }
        return false;
    }

    /**
    * Verifica se há algum caminho entre vértice rotuloVOrigem e 
    * vértice rotuloVDestino.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        int idx_beg = obterIndiceVertice(rotuloVOrigem);
        int idx_end = obterIndiceVertice(rotuloVDestino);
        if (idx_beg == -1 || idx_end == -1)
            return false;
        if (idx_beg == idx_end) {
            for (unsigned i=0; i<arestas[idx_beg].size(); i++) {
                if (arestas[idx_beg][i].first == idx_end) return true;
            }
            return false;
        }

        bool *visitados = new bool[vertices.size()];
        for (unsigned i=0; i<vertices.size(); i++) {
            visitados[i] = false;
        }
        dfs(rotuloVOrigem, visitados);

        if (visitados[idx_end]) {
            return true;
        }
        else return false;
    }

    /**
    * Muda os rótulos do vértices dos diferentes componentes para
    * nomes que representam cores.
    * Dica: procura componentes partindo do vértice v0 ou v1, em ordem
    * crescente (mas você pode usar outra ordem se desejar). 
    * Retorna a quantidade de componentes.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    int colorir() {
        //IMPLEMENTAR
        unsigned len_vertices = vertices.size();

        bool *visitados = new bool[len_vertices];
        bool *last_visitados = new bool[len_vertices];
        char colors = 0;

        if (len_vertices <= 0)
            return 0;

        for (unsigned i=0; i<len_vertices; i++) {
            visitados[i] = false;
            last_visitados[i] = false;
        }

        for (unsigned i=0; i<len_vertices; i++) {
            if (!visitados[i]) {
                dfs(vertices[i], visitados);
                last_visitados = arr_xor(visitados, last_visitados, len_vertices);
                for (unsigned j=0; j<len_vertices; j++) {
                    if (last_visitados[j]) {
                        vertices[j] = colors + 1;
                    }
                }
                last_visitados = arr_or(visitados, last_visitados, len_vertices);
                colors += 1;
            }
        }
        return colors;
    }

    /**
    * Usa a abordagem de navegação BFS para listar as distâncias
    * entre o vértice rotuloVOrigem e cada um dos demais vértices.
    * A função BFS consegue listar a menor distância entre um vértice
    * e os demais se o grafo for NÃO-PONDERADO.
    * Retorna um vetor de inteiros com a quantidade de arestas 
    * (distância) e o vértice rotuloVOrigem e cada um dos demais vértices.
    * Não é uma função recursiva. 
    * É necessário utilizar a ED fila.
    **/
    int* bfs(string rotuloVOrigem) {
        //IMPLEMENTAR
        int idx = obterIndiceVertice(rotuloVOrigem);
        if (idx == -1) {
            return nullptr;
        }

        unsigned len_vertices = vertices.size();
        bool *visitados = new bool[len_vertices];
        int *distances = new int[len_vertices];
        for (unsigned i=0; i<len_vertices; i++) {
            visitados[i] = false;
            distances[i] = 0;
        }

        queue<int> q;
        q.push(idx);
        visitados[idx] = true;

        while (!q.empty()) {
            vector<pair<int, int>> neighbors;
            int next_idx = q.front();
            q.pop();

            neighbors = arestas[next_idx];

            for (unsigned i=0; i<neighbors.size(); i++) {
                if (!visitados[neighbors[i].first]) {
                    q.push(neighbors[i].first);
                    visitados[neighbors[i].first] = true;
                    distances[neighbors[i].first] = distances[next_idx] + 1;
                }
            }
        }
        return distances;
    }

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};
/*
int main() {
    GrafoListaAdj grafo;
    grafo.inserirVertice("v0");
    grafo.inserirVertice("v1");
    grafo.inserirVertice("v2");
    grafo.inserirVertice("v3");
    cout << "v0 -> v1 ?" << grafo.saoConectados("v0", "v1") << endl;
    cout << "v0 -> v2 ?" << grafo.saoConectados("v0", "v2") << endl;;
    cout << "v0 -> v3 ?" << grafo.saoConectados("v0", "v3") << endl;;
    grafo.inserirArestaNaoDirecionada("v0", "v1", 10);
    grafo.inserirArestaNaoDirecionada("v0", "v2", 1);

    cout << "oi" << endl;
    cout << "v0 -> v1 ?" << grafo.saoConectados("v0", "v1") << endl;
    cout << "v0 -> v2 ?" << grafo.saoConectados("v0", "v2") << endl;;
    cout << "v0 -> v3 ?" << grafo.saoConectados("v0", "v3") << endl;;
}
*/

