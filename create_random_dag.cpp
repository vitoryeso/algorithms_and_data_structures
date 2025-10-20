#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include "./utils/graph_utils.h"

using namespace std;

// Função auxiliar para parsear string de camadas ocultas como "[30, 30, 30]"
vector<int> parseHiddenLayers(const string& layersStr) {
    vector<int> layers;
    if (layersStr.empty() || layersStr[0] != '[' || layersStr.back() != ']') {
        return layers;
    }

    string content = layersStr.substr(1, layersStr.size() - 2);
    if (content.empty()) {
        return layers;
    }

    stringstream ss(content);
    string token;
    while (getline(ss, token, ',')) {
        // Remove espaços em branco
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (!token.empty()) {
            layers.push_back(stoi(token));
        }
    }
    return layers;
}

// Implementação completa de Grafo DAG com lista de adjacências
class GrafoDAG {
private:
    vector<string> vertices;
    vector<vector<pair<int, int>>> arestas;

    // Reimplementação dos métodos privados para acesso
    int obterIndiceVertice(string rotuloVertice) {
        for (unsigned i = 0; i < vertices.size(); i++) {
            if (vertices[i] == rotuloVertice) {
                return i;
            }
        }
        return -1;
    }

    // DFS para ordenação topológica
    void dfsTopologico(int v, vector<bool>& visitado, stack<int>& pilha) {
        visitado[v] = true;

        // Visita todos os vizinhos
        for (auto& aresta : arestas[v]) {
            int vizinho = aresta.first;
            if (!visitado[vizinho]) {
                dfsTopologico(vizinho, visitado, pilha);
            }
        }

        // Adiciona à pilha após visitar todos os vizinhos
        pilha.push(v);
    }

public:
    // Construtor
    GrafoDAG() {}

    // Método para criar vértice (override para manter consistência)
    void inserirVertice(string rotuloVertice) {
        if (obterIndiceVertice(rotuloVertice) != -1) {
            return;
        }
        vertices.push_back(rotuloVertice);
        vector<pair<int, int>> new_vertice_edges;
        arestas.push_back(new_vertice_edges);
    }

    // Método para inserir aresta direcionada
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso = 1) {
        int idx_beg = obterIndiceVertice(rotuloVOrigem);
        int idx_end = obterIndiceVertice(rotuloVDestino);
        if (idx_beg == -1 || idx_end == -1)
            return;

        pair<int, int> edge;
        edge.first = idx_end;
        edge.second = peso;
        arestas[idx_beg].push_back(edge);
    }

    // Cria uma MLP (Multi-Layer Perceptron) fully-connected
    void criarMLP(int n_channels_input, vector<int> hidden_layers, int n_channels_output) {
        // Constrói as camadas da MLP
        vector<int> layers_neurons;
        layers_neurons.push_back(n_channels_input);
        for (int neurons : hidden_layers) {
            layers_neurons.push_back(neurons);
        }
        layers_neurons.push_back(n_channels_output);

        int total_layers = layers_neurons.size();

        // Cria vértices por camadas
        for (int camada = 0; camada < total_layers; camada++) {
            for (int neuronio = 0; neuronio < layers_neurons[camada]; neuronio++) {
                string rotulo;
                if (camada == 0) {
                    rotulo = "input_" + to_string(neuronio);
                } else if (camada == total_layers - 1) {
                    rotulo = "output_" + to_string(neuronio);
                } else {
                    rotulo = "hidden" + to_string(camada) + "_" + to_string(neuronio);
                }
                inserirVertice(rotulo);
            }
        }

        // Conecta camadas fully-connected (cada neurônio conecta para todos da próxima camada)
        int vertex_idx = 0;
        for (int camada = 0; camada < total_layers - 1; camada++) {
            int neurons_current = layers_neurons[camada];
            int neurons_next = layers_neurons[camada + 1];
            int start_current = vertex_idx;
            int start_next = vertex_idx + neurons_current;

            // Cada neurônio da camada atual conecta para TODOS da próxima camada
            for (int n_current = 0; n_current < neurons_current; n_current++) {
                int idx_current = start_current + n_current;
                for (int n_next = 0; n_next < neurons_next; n_next++) {
                    int idx_next = start_next + n_next;
                    inserirArestaDirecionada(vertices[idx_current], vertices[idx_next], 1);
                }
            }

            vertex_idx += neurons_current;
        }
    }

    // Ordenação topológica usando DFS
    vector<string> ordenacaoTopologicaDFS() {
        vector<bool> visitado(vertices.size(), false);
        stack<int> pilha;

        // Chama DFS para todos os vértices não visitados
        for (size_t i = 0; i < vertices.size(); i++) {
            if (!visitado[i]) {
                dfsTopologico(i, visitado, pilha);
            }
        }

        // Converte pilha para vetor (ordem topológica)
        vector<string> ordem;
        while (!pilha.empty()) {
            ordem.push_back(vertices[pilha.top()]);
            pilha.pop();
        }

        return ordem;
    }

    // Ordenação topológica usando Kahn (BFS)
    vector<string> ordenacaoTopologicaKahn() {
        vector<int> grauEntrada(vertices.size(), 0);
        queue<int> fila;

        // Calcula grau de entrada para cada vértice
        for (size_t i = 0; i < vertices.size(); i++) {
            for (auto& aresta : arestas[i]) {
                grauEntrada[aresta.first]++;
            }
        }

        // Adiciona vértices com grau 0 à fila
        for (size_t i = 0; i < vertices.size(); i++) {
            if (grauEntrada[i] == 0) {
                fila.push(i);
            }
        }

        vector<string> ordem;

        while (!fila.empty()) {
            int u = fila.front();
            fila.pop();
            ordem.push_back(vertices[u]);

            // Para cada vizinho, diminui grau de entrada
            for (auto& aresta : arestas[u]) {
                int v = aresta.first;
                grauEntrada[v]--;
                if (grauEntrada[v] == 0) {
                    fila.push(v);
                }
            }
        }

        // Verifica se há ciclo (se nem todos os vértices foram incluídos)
        if (ordem.size() != vertices.size()) {
            cout << "Erro: Grafo contém ciclo!" << endl;
            return vector<string>();
        }

        return ordem;
    }

    // Imprime o grafo
    void imprimirGrafo() {
        cout << "=== GRAFO DAG ===" << endl;
        for (size_t i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << " ->";
            if (arestas[i].empty()) {
                cout << " (sem conexões)";
            } else {
                for (auto& aresta : arestas[i]) {
                    cout << " " << vertices[aresta.first] << "(" << aresta.second << ")";
                }
            }
            cout << endl;
        }

        // Conta arestas
        size_t totalArestas = 0;
        for (auto& lista : arestas) {
            totalArestas += lista.size();
        }
        cout << endl << "📊 Estatísticas:" << endl;
        cout << "   Total de vértices: " << vertices.size() << endl;
        cout << "   Total de arestas: " << totalArestas << endl;
        cout << "   Grau médio: " << fixed << setprecision(2) << (double)totalArestas / vertices.size() << endl;
    }

    // Exporta grafo em formato GraphViz (.dot)
    bool exportarGraphViz(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Erro: Não foi possível criar o arquivo " << filename << endl;
            return false;
        }

        file << "digraph DAG {" << endl;
        file << "    rankdir=LR;" << endl;  // Layout da esquerda para direita
        file << "    node [shape=circle, style=filled, fillcolor=lightblue];" << endl;
        file << endl;

        // Agrupar nós por camada para melhor visualização (MLP)
        // Determinar camadas dinamicamente baseado nos nomes dos vértices
        map<int, vector<string>> camadas; // camada -> lista de vértices

        for (const string& vertice : vertices) {
            if (vertice.find("input_") == 0) {
                // Camada de entrada
                camadas[0].push_back(vertice);
            } else if (vertice.find("output_") == 0) {
                // Camada de saída (última camada)
                int max_layer = 0;
                for (auto& p : camadas) {
                    max_layer = max(max_layer, p.first);
                }
                camadas[max_layer + 1].push_back(vertice);
            } else if (vertice.find("hidden") == 0) {
                // Camadas ocultas: hidden{NUMERO}_...
                size_t underscore_pos = vertice.find('_');
                if (underscore_pos != string::npos) {
                    string layer_str = vertice.substr(6, underscore_pos - 6); // "hidden" tem 6 chars
                    int layer_num = stoi(layer_str);
                    camadas[layer_num].push_back(vertice);
                }
            }
        }

        // Escrever as camadas no arquivo .dot
        for (const auto& camada : camadas) {
            file << "    // Camada " << camada.first << endl;
            file << "    { rank=same;";
            for (const string& vertice : camada.second) {
                file << " " << vertice << ";";
            }
            file << " }" << endl;
        }

        file << endl;

        // Adicionar arestas
        for (size_t i = 0; i < vertices.size(); i++) {
            for (auto& aresta : arestas[i]) {
                file << "    " << vertices[i] << " -> " << vertices[aresta.first] << ";" << endl;
            }
        }

        file << "}" << endl;
        file.close();

        cout << "✅ Grafo exportado para " << filename << endl;
        cout << "💡 Para visualizar: dot -Tpng " << filename << " -o grafo.png && xdg-open grafo.png" << endl;
        return true;
    }

    // Getters
    vector<string> getVertices() { return vertices; }
    vector<vector<pair<int, int>>> getArestas() { return arestas; }
};

// Estrutura para armazenar os argumentos parseados
struct Args {
    int n_input = -1;
    string layers = "";
    int n_output = -1;
    bool help = false;
};

// Função para mostrar ajuda
void show_help() {
    cout << "=== CRIADOR DE GRAFO MLP (MULTI-LAYER PERCEPTRON) ===" << endl;
    cout << "Este programa cria um grafo MLP fully-connected sem ativações" << endl;
    cout << "e realiza ordenação topológica." << endl << endl;
    cout << "Uso:" << endl;
    cout << "  ./bin/create_dag --n_input <num> --layers <[n1,n2,n3]> --n_output <num>" << endl;
    cout << "  ./bin/create_dag --help" << endl << endl;
    cout << "Opções:" << endl;
    cout << "  --n_input <num>     Número de neurônios na camada de entrada" << endl;
    cout << "  --layers <string>   Camadas ocultas no formato [n1,n2,n3] ou [] para nenhuma" << endl;
    cout << "  --n_output <num>    Número de neurônios na camada de saída" << endl;
    cout << "  --help              Mostra esta ajuda" << endl << endl;
    cout << "Exemplos:" << endl;
    cout << "  ./bin/create_dag --n_input 5 --layers [10,10,10] --n_output 3" << endl;
    cout << "  ./bin/create_dag --n_input 3 --layers [] --n_output 2" << endl;
}

// Função para parsear argumentos nomeados
Args parse_args(int argc, char* argv[]) {
    Args args;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            args.help = true;
            return args;
        } else if (arg == "--n_input") {
            if (i + 1 < argc) {
                args.n_input = atoi(argv[++i]);
            } else {
                cerr << "Erro: --n_input requer um valor" << endl;
                exit(1);
            }
        } else if (arg == "--layers") {
            if (i + 1 < argc) {
                args.layers = argv[++i];
            } else {
                cerr << "Erro: --layers requer um valor" << endl;
                exit(1);
            }
        } else if (arg == "--n_output") {
            if (i + 1 < argc) {
                args.n_output = atoi(argv[++i]);
            } else {
                cerr << "Erro: --n_output requer um valor" << endl;
                exit(1);
            }
        } else {
            cerr << "Erro: Argumento desconhecido: " << arg << endl;
            cerr << "Use --help para ver as opções disponíveis" << endl;
            exit(1);
        }
    }

    return args;
}

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);

    if (args.help) {
        show_help();
        return 0;
    }

    cout << "=== CRIADOR DE GRAFO MLP (MULTI-LAYER PERCEPTRON) ===" << endl;
    cout << "Este programa cria um grafo MLP fully-connected sem ativações" << endl;
    cout << "e realiza ordenação topológica." << endl << endl;

    // Parâmetros de entrada
    int n_channels_input, n_channels_output;
    string hidden_layers_str;

    // Verifica se todos os argumentos obrigatórios foram fornecidos
    if (args.n_input == -1 || args.layers == "" || args.n_output == -1) {
        // Modo interativo
        cout << "Digite o número de canais de entrada: ";
        cin >> n_channels_input;

        cout << "Digite as camadas ocultas (exemplo: [30, 30, 30]): ";
        cin >> hidden_layers_str;

        cout << "Digite o número de canais de saída: ";
        cin >> n_channels_output;
    } else {
        // Parâmetros via linha de comando
        n_channels_input = args.n_input;
        hidden_layers_str = args.layers;
        n_channels_output = args.n_output;
    }

    // Validação dos parâmetros
    if (n_channels_input < 1 || n_channels_output < 1) {
        cout << "Erro: Número de canais deve ser pelo menos 1" << endl;
        return 1;
    }

    // Parse das camadas ocultas
    vector<int> hidden_layers = parseHiddenLayers(hidden_layers_str);
    if (hidden_layers.empty() && hidden_layers_str != "[]") {
        cout << "Erro: Formato inválido para camadas ocultas. Use [n1, n2, n3] ou [] para nenhuma" << endl;
        return 1;
    }

    // Verifica se todas as camadas têm pelo menos 1 neurônio
    for (int neurons : hidden_layers) {
        if (neurons < 1) {
            cout << "Erro: Cada camada deve ter pelo menos 1 neurônio" << endl;
            return 1;
        }
    }

    // Cria o grafo MLP
    GrafoDAG grafo;
    grafo.criarMLP(n_channels_input, hidden_layers, n_channels_output);

    // Imprime informações
    grafo.imprimirGrafo();

    // Perguntar se quer exportar
    cout << endl << "🔍 Deseja exportar o grafo em formato GraphViz? (s/n): ";
    char resposta;
    cin >> resposta;

    if (resposta == 's' || resposta == 'S') {
        string filename = "grafo_dag.dot";
        grafo.exportarGraphViz(filename);
    }

    cout << endl << "=== ORDENAÇÃO TOPOLÓGICA ===" << endl;

    // Ordenação usando DFS
    cout << "Usando DFS:" << endl;
    vector<string> ordemDFS = grafo.ordenacaoTopologicaDFS();
    for (size_t i = 0; i < ordemDFS.size(); i++) {
        cout << ordemDFS[i];
        if (i < ordemDFS.size() - 1) cout << " -> ";
    }
    cout << endl << endl;

    // Ordenação usando Kahn
    cout << "Usando Kahn (BFS):" << endl;
    vector<string> ordemKahn = grafo.ordenacaoTopologicaKahn();
    for (size_t i = 0; i < ordemKahn.size(); i++) {
        cout << ordemKahn[i];
        if (i < ordemKahn.size() - 1) cout << " -> ";
    }
    cout << endl << endl;

    // Verifica se as ordenações são iguais
    bool iguais = (ordemDFS.size() == ordemKahn.size());
    if (iguais) {
        for (size_t i = 0; i < ordemDFS.size(); i++) {
            if (ordemDFS[i] != ordemKahn[i]) {
                iguais = false;
                break;
            }
        }
    }

    if (iguais) {
        cout << "✅ As duas ordenações são idênticas!" << endl;
    } else {
        cout << "ℹ️ As ordenações são diferentes (válidas ambas)." << endl;
    }

    return 0;
}
