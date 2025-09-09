#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;

// Função insertion_sort do código original
void insertion_sort(vector<int>& V) {
	if (V.size() == 1) return;
	int i, j, left;
	for (i=1; i<V.size(); i++) {
		for (j=i; j>0; j--) {
			if (V[j-1] > V[j]) {
				left = V[j - 1];
				// swap(V[j], V[j - 1])
				V[j - 1] = V[j];
				V[j] = left;
			}
			else break;
		}
	}
}

// Função para gerar vetores de teste
vector<int> generate_ordered_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = i;
    }
    return v;
}

vector<int> generate_reverse_ordered_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = size - 1 - i;
    }
    return v;
}

vector<int> generate_random_vector(int size) {
    vector<int> v(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, size * 10);
    
    for (int i = 0; i < size; i++) {
        v[i] = dis(gen);
    }
    return v;
}

// Função para medir o tempo de execução
double measure_sorting_time(vector<int> v) {
    auto start = chrono::high_resolution_clock::now();
    insertion_sort(v);
    auto end = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // Converter para milissegundos
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
    // Adicionar 20k se não estiver na lista
    if (sizes.back() != 20000) {
        sizes.push_back(20000);
    }
    
    string test_types[] = {"ordenado", "invertido", "aleatorio"};
    
    // Abrir arquivo CSV para salvar resultados
    ofstream csv_file("insertion_sort_detailed_results.csv");
    csv_file << "tipo_teste,tamanho,tempo_ms" << endl;
    
    cout << "Iniciando testes detalhados de performance do insertion_sort..." << endl;
    cout << "Testando " << sizes.size() << " tamanhos diferentes de " << sizes[0] << " até " << sizes.back() << endl;
    
    int progress = 0;
    int total_tests = sizes.size() * 3; // 3 tipos de teste
    
    for (int size : sizes) {
        cout << "\nTamanho do vetor: " << size << endl;
        
        // Teste com vetor ordenado
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor ordenado..." << endl;
        vector<int> ordered_vec = generate_ordered_vector(size);
        double time_ordered = measure_sorting_time(ordered_vec);
        csv_file << "ordenado," << size << "," << time_ordered << endl;
        cout << "    Tempo: " << time_ordered << " ms" << endl;
        
        // Teste com vetor inversamente ordenado
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor inversamente ordenado..." << endl;
        vector<int> reverse_vec = generate_reverse_ordered_vector(size);
        double time_reverse = measure_sorting_time(reverse_vec);
        csv_file << "invertido," << size << "," << time_reverse << endl;
        cout << "    Tempo: " << time_reverse << " ms" << endl;
        
        // Teste com vetor aleatório
        progress++;
        cout << "  [" << progress << "/" << total_tests << "] Testando vetor aleatório..." << endl;
        vector<int> random_vec = generate_random_vector(size);
        double time_random = measure_sorting_time(random_vec);
        csv_file << "aleatorio," << size << "," << time_random << endl;
        cout << "    Tempo: " << time_random << " ms" << endl;
    }
    
    csv_file.close();
    cout << "\nTestes concluídos! Resultados salvos em insertion_sort_detailed_results.csv" << endl;
    
    return 0;
}
