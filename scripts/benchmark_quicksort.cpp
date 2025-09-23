#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "../algorithms/cpp/luAlgorithms.h"

using namespace std;

// Função para exibir ajuda
void printHelp() {
    cout << "Uso: benchmark_quicksort [opções]" << endl;
    cout << "Opções:" << endl;
    cout << "  --pivot <choice>      Escolha do pivô: last, random, median3 (padrão: last)" << endl;
    cout << "  --level <num>         Nível de distribuição (1-6, padrão: 1)" << endl;
    cout << "  --sizes <lista>       Tamanhos separados por vírgula (padrão: 1000,5000,10000,50000,100000,500000,1000000)" << endl;
    cout << "  --runs <num>          Número de execuções para média (padrão: 1)" << endl;
    cout << "  --output <formato>    Formato de saída: text, csv (padrão: text)" << endl;
    cout << "  --help                Exibir esta ajuda" << endl;
}

// Função para parsear tamanhos
vector<size_t> parseSizes(const string& sizesStr) {
    vector<size_t> sizes;
    stringstream ss(sizesStr);
    string token;
    while (getline(ss, token, ',')) {
        sizes.push_back(stoul(token));
    }
    return sizes;
}

// Função para gerar um vetor com distribuição normal de tamanho n, com nível de desvio padrão
vector<int> generateRandomVector(size_t n, int level = 1) {
    vector<int> v(n);
    random_device rd;
    mt19937 gen(rd());

    // Níveis de desvio padrão: 100, 500, 1000, 5000, 10000, 50000
    vector<double> std_devs = {100.0, 500.0, 1000.0, 5000.0, 10000.0, 50000.0};
    double std_dev = std_devs[min(level - 1, (int)std_devs.size() - 1)];

    normal_distribution<double> dis(0.0, std_dev);
    for (size_t i = 0; i < n; ++i) {
        double val = dis(gen);
        // Clamp to int range to avoid overflow
        val = max(-2147483648.0, min(2147483647.0, val));
        v[i] = static_cast<int>(round(val));
    }
    return v;
}

// Função para copiar um vetor
vector<int> copyVector(const vector<int>& v) {
    return vector<int>(v);
}

// Função de benchmark para um tipo de Quick Sort com nível de distribuição
void benchmarkQuickSort(const string& pivot_choice, const vector<size_t>& sizes, int level, const string& output_format) {
    bool is_csv = (output_format == "csv");

    if (!is_csv) {
        cout << "Benchmarking Quick Sort com pivot: " << pivot_choice << " (Nível " << level << ", Std Dev: " << (level == 1 ? 100 : level == 2 ? 500 : level == 3 ? 1000 : level == 4 ? 5000 : level == 5 ? 10000 : 50000) << ")" << endl;
        cout << setw(15) << "Tamanho" << setw(20) << "Tempo (ms)" << setw(20) << "Status" << endl;
        cout << string(55, '-') << endl;
    } else {
        cout << "pivot,level,size,time_ms,status" << endl;
    }

    for (size_t size : sizes) {
        vector<int> v = generateRandomVector(size, level);
        vector<int> v_copy = copyVector(v);

        auto start = chrono::high_resolution_clock::now();
        quick_sort(v_copy, pivot_choice);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;
        double time_ms = duration.count();

        // Verificar se está ordenado
        bool is_sorted = std::is_sorted(v_copy.begin(), v_copy.end());

        string status = is_sorted ? "Ordenado" : "ERRO: Nao ordenado!";

        if (!is_csv) {
            cout << setw(15) << size << setw(20) << fixed << setprecision(4) << time_ms << setw(20) << status << endl;
        } else {
            cout << pivot_choice << "," << level << "," << size << "," << fixed << setprecision(4) << time_ms << "," << status << endl;
        }
    }
    if (!is_csv) cout << endl;
}

int main(int argc, char* argv[]) {
    string pivot = "last";
    int level = 1;
    string sizesStr = "1000,5000,10000,50000,100000,500000,1000000";
    int runs = 1;
    string output = "text";

    // Parsing de argumentos
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        } else if (strcmp(argv[i], "--pivot") == 0 && i + 1 < argc) {
            pivot = argv[++i];
        } else if (strcmp(argv[i], "--level") == 0 && i + 1 < argc) {
            level = stoi(argv[++i]);
        } else if (strcmp(argv[i], "--sizes") == 0 && i + 1 < argc) {
            sizesStr = argv[++i];
        } else if (strcmp(argv[i], "--runs") == 0 && i + 1 < argc) {
            runs = stoi(argv[++i]);
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output = argv[++i];
        }
    }

    // Validar argumentos
    if (pivot != "last" && pivot != "random" && pivot != "median3") {
        cerr << "Erro: Pivot inválido. Use --help para ajuda." << endl;
        return 1;
    }
    if (level < 1 || level > 6) {
        cerr << "Erro: Nível deve ser entre 1 e 6." << endl;
        return 1;
    }
    if (runs < 1) {
        cerr << "Erro: Runs deve ser pelo menos 1." << endl;
        return 1;
    }

    vector<size_t> sizes = parseSizes(sizesStr);

    if (output != "csv") {
        cout << "Executando benchmark com: pivot=" << pivot << ", level=" << level << ", sizes=[" << sizesStr << "], runs=" << runs << ", output=" << output << endl;
    }

    // Executar benchmarks
    for (int run = 0; run < runs; ++run) {
        if (output != "csv") {
            if (runs > 1) {
                cout << "=== Execução " << (run + 1) << " ===" << endl;
            }
            cout << "=== Nível de Distribuição " << level << " ===" << endl;
        }

        benchmarkQuickSort(pivot, sizes, level, output);

        if (output != "csv") cout << endl;
    }

    return 0;
}
