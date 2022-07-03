#include <iostream>
#include <vector>
#include <queue>
#include "stdbool.h"

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
        //reusar implementacao de exercicios anteriores
        for (unsigned i=0; i<vertices.size(); i++) {
            if (rotuloVertice == vertices[i])
                return i;
        }
        return -1;
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
        //reusar implementacao de exercicios anteriores
        unsigned idx = obterIndiceVertice(rotuloVertice);

        if (idx <= -1) {
            vertices.append(rotuloVertice);
            vector<pair<int, int>> new_vertice;
            arestas.append(new_vertice);
        }
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
        //reusar implementacao de exercicios anteriores
        unsigned idx, jdx;
        idx = obterIndiceVertice(rotuloOrigem);
        jdx = obterIndiceVertice(rotuloDestino);

        if (idx == -1 || jdx == -1)
            return;

        pair<int, int> new_edge;
        new_edge.first = jdx;
        new_edge.second = peso;

        arestas[idx].append(new_edge);

        return 0;
    }

    //Grupo do union-find
    class Grupo {
    public:
        int pai;
        int tamanho;
    };

    //encontrar raiz 
    //pode ser com ou sem path compression
    int encontrarRaiz(Grupo* grupos, int i) {
        //IMPLEMENTAR
    }

    bool mesmoGrupo(Grupo* grupos, int a, int b) {
        //IMPLEMENTAR
    }

    int tamanhoGrupo(Grupo* grupos, int a) {
        //IMPLEMENTAR
    }

    void unirGrupos(Grupo* grupos, int a, int b) {
        //IMPLEMENTAR
    }

    //criei essa classe pra facilitar a ordenacao
    //das arestas com menor peso (que faz parte do
    //algoritmo de Kruskal)
    class Aresta {
        public:
            int origem;
            int destino;
            int peso;
            Aresta(int origem, int destino, int peso)
                : origem(origem), destino(destino), peso(peso)
            {}
    };

    //sobrescrever operator< para que a priority_queue
    //ordene como desejamos
    friend bool operator<(const Aresta& a1, const Aresta& a2) {
        //IMPLEMENTAR
    }

    //a funcao sera usada para criar uma arvore de espalhamento
    //minimo ou máximo, de acordo com o argumento
    GrafoListaAdj* KruskalMST(bool min) {
    
    }

    /*GrafoListaAdj* KruskalMST() {
        //IMPLEMENTAR
        //instanciamos um novo grafo, que armazenara
        //apenas as arestas da MST
        GrafoListaAdj* mst = new GrafoListaAdj();
        
        //adicionar vertices
        //inicializar estrutura do union-find (array de grupos)
        
       
        //inicializar priority_queue
        priority_queue<Aresta> arestasMenorPeso;
        //adicionar arestas na fila    
        
        //iterar na fila de arestas
            //se os vertices da arestas nao pertencem ao mesmo grupo
            //una-os, e adicione a aresta no MST
        return mst;
    }*/   

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};