/*
 * =================================================================================
 * DEPRECATED: Este script é legado e será removido em Janeiro 2026
 * =================================================================================
 *
 * SUBSTITUÍDO POR: scripts/benchmark_matmul.cpp
 *
 * Razões para depreciação:
 * - CLI limitada comparada ao sistema unificado
 * - Não suporta múltiplos algoritmos simultaneamente
 * - Não gera metadados de ambiente
 * - Não integrado ao sistema de plotting unificado
 *
 * Migração recomendada:
 *   ./bin/benchmark_matmul --algorithms naive,strassen --sizes 64 128 256
 *
 * Para mais informações, consulte BENCHMARKS_README.md
 * =================================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>
#include <algorithm>
#include "algorithms/cpp/luAlgorithms.h"

using namespace std;
using namespace chrono;

struct BenchmarkResult {
    int size;
    double naive_time;
    double strassen_time;
    int num_runs;
    
    BenchmarkResult(int s, double n, double st, int runs) 
        : size(s), naive_time(n), strassen_time(st), num_runs(runs) {}
};

class MatMulBenchmarkCPP {
private:
    unsigned cutoff;
    int min_val;
    int max_val;
    
    Matrix generate_random_matrix(int n, unsigned seed) {
        Matrix M(n);
        mt19937 rng(seed);
        uniform_int_distribution<int> dist(min_val, max_val);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                M[i][j] = dist(rng);
            }
        }
        return M;
    }
    
    double benchmark_naive(int size, unsigned seed) {
        Matrix A = generate_random_matrix(size, seed);
        Matrix B = generate_random_matrix(size, seed + 1);
        
        auto start = high_resolution_clock::now();
        Matrix C = matmul_naive(A, B);  // matmul_naive returns a Matrix, so assignment is fine
        auto end = high_resolution_clock::now();
        
        duration<double> duration = end - start;
        return duration.count();
    }
    
    double benchmark_strassen(int size, unsigned seed) {
        Matrix A = generate_random_matrix(size, seed);
        Matrix B = generate_random_matrix(size, seed + 1);
        
        auto start = high_resolution_clock::now();
        Matrix C = matmul_strassen(A, B, cutoff);  // matmul_strassen returns a Matrix, so assignment is fine
        auto end = high_resolution_clock::now();
        
        duration<double> duration = end - start;
        return duration.count();
    }
    
public:
    MatMulBenchmarkCPP(unsigned c = 64, int min_v = -10, int max_v = 10)
        : cutoff(c), min_val(min_v), max_val(max_v) {}
    
    BenchmarkResult run_benchmark_for_size(int size, int num_runs) {
        cout << "Testando n=" << size << " com " << num_runs << " execuções..." << endl;
        
        vector<double> naive_times;
        vector<double> strassen_times;
        
        for (int run = 0; run < num_runs; run++) {
            cout << "  Run " << (run + 1) << "/" << num_runs << "..." << flush;
            
            // Benchmark naive
            double naive_time = benchmark_naive(size, 42 + run);
            naive_times.push_back(naive_time);
            
            // Benchmark strassen
            double strassen_time = benchmark_strassen(size, 42 + run);
            strassen_times.push_back(strassen_time);
            
            cout << " ✓" << endl;
        }
        
        // Calcula médias
        double avg_naive = accumulate(naive_times.begin(), naive_times.end(), 0.0) / naive_times.size();
        double avg_strassen = accumulate(strassen_times.begin(), strassen_times.end(), 0.0) / strassen_times.size();
        
        cout << "  Naive: " << fixed << setprecision(6) << avg_naive << "s" << endl;
        cout << "  Strassen: " << fixed << setprecision(6) << avg_strassen << "s" << endl;
        cout << endl;
        
        return BenchmarkResult(size, avg_naive, avg_strassen, num_runs);
    }
    
    vector<BenchmarkResult> run_full_benchmark(const vector<int>& sizes, int num_runs) {
        vector<BenchmarkResult> results;
        
        cout << "=== Matrix Multiplication Benchmark ===" << endl;
        cout << "Cutoff: " << cutoff << endl;
        cout << "Valores das matrizes: [" << min_val << ", " << max_val << "]" << endl;
        cout << "Tamanhos: ";
        for (size_t i = 0; i < sizes.size(); i++) {
            cout << sizes[i];
            if (i < sizes.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Execuções por teste: " << num_runs << endl;
        cout << "========================================" << endl << endl;
        
        for (int size : sizes) {
            results.push_back(run_benchmark_for_size(size, num_runs));
        }
        
        return results;
    }
    
    void save_results_csv(const vector<BenchmarkResult>& results, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Erro ao abrir arquivo: " << filename << endl;
            return;
        }
        
        // Cabeçalho
        file << "size,naive_time,strassen_time,num_runs,cutoff,min_val,max_val" << endl;
        
        // Dados
        for (const auto& result : results) {
            file << result.size << ","
                 << fixed << setprecision(8) << result.naive_time << ","
                 << fixed << setprecision(8) << result.strassen_time << ","
                 << result.num_runs << ","
                 << cutoff << ","
                 << min_val << ","
                 << max_val << endl;
        }
        
        file.close();
        cout << "Resultados salvos em: " << filename << endl;
    }
    
    void save_results_json(const vector<BenchmarkResult>& results, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Erro ao abrir arquivo: " << filename << endl;
            return;
        }
        
        file << "{" << endl;
        file << "  \"metadata\": {" << endl;
        file << "    \"cutoff\": " << cutoff << "," << endl;
        file << "    \"min_val\": " << min_val << "," << endl;
        file << "    \"max_val\": " << max_val << "," << endl;
        file << "    \"timestamp\": \"" << __DATE__ << " " << __TIME__ << "\"" << endl;
        file << "  }," << endl;
        file << "  \"results\": [" << endl;
        
        for (size_t i = 0; i < results.size(); i++) {
            const auto& result = results[i];
            file << "    {" << endl;
            file << "      \"size\": " << result.size << "," << endl;
            file << "      \"naive_time\": " << fixed << setprecision(8) << result.naive_time << "," << endl;
            file << "      \"strassen_time\": " << fixed << setprecision(8) << result.strassen_time << "," << endl;
            file << "      \"num_runs\": " << result.num_runs << endl;
            file << "    }";
            if (i < results.size() - 1) file << ",";
            file << endl;
        }
        
        file << "  ]" << endl;
        file << "}" << endl;
        
        file.close();
        cout << "Resultados salvos em: " << filename << endl;
    }
};

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Opções:" << endl;
    cout << "  --sizes <tamanhos>     Lista de tamanhos separados por espaço (ex: 64 128 256)" << endl;
    cout << "  --max-size <max>       Tamanho máximo (gera potências de 2 até max)" << endl;
    cout << "  --min-size <min>       Tamanho mínimo (padrão: 2)" << endl;
    cout << "  --runs <num>           Número de execuções por teste (padrão: 3)" << endl;
    cout << "  --cutoff <val>         Cutoff para Strassen (padrão: 64)" << endl;
    cout << "  --min-val <val>        Valor mínimo dos elementos (padrão: -10)" << endl;
    cout << "  --max-val <val>        Valor máximo dos elementos (padrão: 10)" << endl;
    cout << "  --output <arquivo>     Arquivo CSV de saída (padrão: benchmark_results.csv)" << endl;
    cout << "  --json <arquivo>       Arquivo JSON de saída (opcional)" << endl;
    cout << "  --help, -h             Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Exemplos:" << endl;
    cout << "  " << program_name << " --sizes 64 128 256 512" << endl;
    cout << "  " << program_name << " --max-size 1024 --runs 5" << endl;
    cout << "  " << program_name << " --min-size 32 --max-size 2048 --cutoff 128" << endl;
}

int main(int argc, char* argv[]) {
    // Parâmetros padrão
    vector<int> sizes;
    int max_size = 1024;
    int min_size = 2;
    int num_runs = 3;
    unsigned cutoff = 64;
    int min_val = -10;
    int max_val = 10;
    string output_csv = "benchmark_results.csv";
    string output_json = "";
    
    // Parse dos argumentos
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--sizes") {
            sizes.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                sizes.push_back(stoi(argv[i]));
                i++;
            }
            i--; // Ajusta pois o loop principal irá incrementar
        } else if (arg == "--max-size" && i + 1 < argc) {
            max_size = stoi(argv[++i]);
        } else if (arg == "--min-size" && i + 1 < argc) {
            min_size = stoi(argv[++i]);
        } else if (arg == "--runs" && i + 1 < argc) {
            num_runs = stoi(argv[++i]);
        } else if (arg == "--cutoff" && i + 1 < argc) {
            cutoff = stoi(argv[++i]);
        } else if (arg == "--min-val" && i + 1 < argc) {
            min_val = stoi(argv[++i]);
        } else if (arg == "--max-val" && i + 1 < argc) {
            max_val = stoi(argv[++i]);
        } else if (arg == "--output" && i + 1 < argc) {
            output_csv = argv[++i];
        } else if (arg == "--json" && i + 1 < argc) {
            output_json = argv[++i];
        } else {
            cerr << "Argumento desconhecido: " << arg << endl;
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // Se sizes não foi especificado, gera potências de 2
    if (sizes.empty()) {
        int size = min_size;
        while (size <= max_size) {
            sizes.push_back(size);
            size *= 2;
        }
    }
    
    // Valida que todos os tamanhos são potências de 2
    for (int size : sizes) {
        if (size <= 0 || (size & (size - 1)) != 0) {
            cerr << "Erro: " << size << " não é uma potência de 2 positiva" << endl;
            return 1;
        }
    }
    
    // Ordena os tamanhos
    sort(sizes.begin(), sizes.end());
    
    // Executa benchmark
    MatMulBenchmarkCPP benchmark(cutoff, min_val, max_val);
    vector<BenchmarkResult> results = benchmark.run_full_benchmark(sizes, num_runs);
    
    // Salva resultados
    benchmark.save_results_csv(results, output_csv);
    if (!output_json.empty()) {
        benchmark.save_results_json(results, output_json);
    }
    
    cout << "Benchmark concluído com sucesso!" << endl;
    return 0;
}
