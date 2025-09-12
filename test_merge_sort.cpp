#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include "algorithms/cpp/luAlgorithms.h"

using namespace std;

static vector<int> generate_ordered_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) v[i] = i;
    return v;
}

static vector<int> generate_reverse_ordered_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) v[i] = size - 1 - i;
    return v;
}

static vector<int> generate_random_vector(int size) {
    vector<int> v(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, size * 10);
    for (int i = 0; i < size; i++) v[i] = dis(gen);
    return v;
}

static double measure_sorting_time(void(*sort_fn)(vector<int>&), vector<int> v) {
    auto start = chrono::high_resolution_clock::now();
    cout << "len v (before run sort_fn): " << v.size() << endl;
    sort_fn(v);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // ms
}

int main() {
    // Gerar tamanhos: 10-100 (passo 10) + 100-20k (passo 600)
    vector<int> sizes;
    
    // Adicionar tamanhos pequenos: 10, 20, 30, ..., 100
    for (int size = 10; size <= 100; size += 10) {
        sizes.push_back(size);
    }
    
    // Adicionar tamanhos maiores: 700, 1300, ..., 20000
    for (int size = 700; size <= 20000; size += 600) {
        sizes.push_back(size);
    }
    if (sizes.back() != 20000) sizes.push_back(20000);

    ofstream csv_file("merge_sort_detailed_results.csv");
    csv_file << "tipo_teste,tamanho,tempo_ms" << endl;

    cout << "Iniciando testes detalhados de performance do merge_sort..." << endl;
    cout << "Testando " << sizes.size() << " tamanhos diferentes de " << sizes.front() << " até " << sizes.back() << endl;

    int progress = 0;
    int total_tests = sizes.size() * 3;

    for (int size : sizes) {
        cout << "\nTamanho do vetor: " << size << endl;

        // ordenado
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor ordenado..." << endl;
        {
            vector<int> v = generate_ordered_vector(size);
            double t = measure_sorting_time(merge_sort, v);
            csv_file << "ordenado," << size << "," << t << endl;
            cout << "    Tempo: " << t << " ms" << endl;
        }

        // invertido
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor inversamente ordenado..." << endl;
        {
            vector<int> v = generate_reverse_ordered_vector(size);
            double t = measure_sorting_time(merge_sort, v);
            csv_file << "invertido," << size << "," << t << endl;
            cout << "    Tempo: " << t << " ms" << endl;
        }

        // aleatorio
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor aleatório..." << endl;
        {
            vector<int> v = generate_random_vector(size);
            double t = measure_sorting_time(merge_sort, v);
            csv_file << "aleatorio," << size << "," << t << endl;
            cout << "    Tempo: " << t << " ms" << endl;
        }
    }

    csv_file.close();
    cout << "\nTestes concluídos! Resultados salvos em merge_sort_detailed_results.csv" << endl;
    return 0;
}
