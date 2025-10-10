#include "minmax.h"
#include "../common/questao_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <random>

using namespace std;

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Benchmark da Questão 10: Min/Max Simultâneo" << endl;
    cout << endl;
    cout << "Opções:" << endl;
    cout << "  --max-size <n>         Tamanho máximo do array (padrão: 10000000)" << endl;
    cout << "  --min-size <n>         Tamanho mínimo do array (padrão: 1000)" << endl;
    cout << "  --runs <n>             Número de execuções por teste (padrão: 5)" << endl;
    cout << "  --output-csv <arquivo> Arquivo CSV de saída (opcional)" << endl;
    cout << "  --quiet                Modo silencioso" << endl;
    cout << "  --help, -h             Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Este benchmark compara:" << endl;
    cout << "  - Não simultaneo (2-pass): 2n-2 comparações" << endl;
    cout << "  - Simultaneo (pairwise): ~3n/2 comparações" << endl;
    cout << "  - Variantes 'grok' (estilo alternativo)" << endl;
    cout << endl;
    cout << "Testa diferentes distribuições:" << endl;
    cout << "  - random: Valores aleatórios" << endl;
    cout << "  - sorted: Array ordenado" << endl;
    cout << "  - reverse: Array em ordem reversa" << endl;
    cout << "  - constant: Todos elementos iguais" << endl;
}

int main(int argc, char* argv[]) {
    // Parâmetros padrão
    int max_size = 10000000;
    int min_size = 1000;
    int num_runs = 5;
    string output_csv = "";
    bool quiet = false;
    
    // Parse argumentos
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--max-size" && i + 1 < argc) {
            max_size = stoi(argv[++i]);
        } else if (arg == "--min-size" && i + 1 < argc) {
            min_size = stoi(argv[++i]);
        } else if (arg == "--runs" && i + 1 < argc) {
            num_runs = stoi(argv[++i]);
        } else if (arg == "--output-csv" && i + 1 < argc) {
            output_csv = argv[++i];
        } else if (arg == "--quiet") {
            quiet = true;
        } else {
            cerr << "Argumento desconhecido: " << arg << endl;
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // Gera tamanhos
    vector<int> sizes = generate_sizes(min_size, max_size);
    
    // Gera seeds
    vector<unsigned> seeds = generate_seeds(42, num_runs);
    
    if (!quiet) {
        cout << "========================================" << endl;
        cout << "QUESTÃO 10: Min/Max Simultâneo" << endl;
        cout << "========================================" << endl;
        cout << "Tamanhos: " << sizes.size() << " valores de " 
             << sizes.front() << " até " << sizes.back() << endl;
        cout << "Execuções por teste: " << num_runs << endl;
        cout << "========================================" << endl << endl;
    }
    
    vector<BenchmarkResult> results;
    
    // Tipos de dados para testar
    vector<string> data_types = {"random", "random_paired", "sorted", "reverse", "constant"};
    
    for (const string& data_type : data_types) {
        if (!quiet) {
            cout << "Testando distribuição: " << data_type << endl;
        }
        
        for (int size : sizes) {
            if (!quiet) {
                cout << "  Tamanho n=" << size << ": " << flush;
            }
            
            for (int run = 0; run < num_runs; run++) {
                // Gera dados
                vector<int> data;
                if (data_type == "random") {
                    data = generate_random_data(size, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), seeds[run]);
                } else if (data_type == "random_paired") {
                    // Gera pares pré-ordenados para favorecer previsibilidade no branch do par
                    std::mt19937 rng(seeds[run]);
                    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                    data.resize(size);
                    for (int i = 0; i < size; i += 2) {
                        int a = dist(rng), b = dist(rng);
                        if (a <= b) { data[i] = a; if (i + 1 < size) data[i + 1] = b; }
                        else        { data[i] = b; if (i + 1 < size) data[i + 1] = a; }
                    }
                } else if (data_type == "sorted") {
                    data = generate_sorted_data(size, 0);
                } else if (data_type == "reverse") {
                    data = generate_reverse_data(size, 0);
                } else { // constant
                    data = vector<int>(size, 42);
                }
                
                Timer timer;
                
                // Separate (2 passes)
                timer.start();
                auto result_sep_pair = minmax_separate(data);
                double time_sep = timer.elapsed_seconds();
                results.emplace_back(size, "não simultaneo", data_type, run, seeds[run], time_sep, 0, "");
                
                // Simultaneous (pairwise)
                timer.start();
                auto result_sim_pair = minmax_simultaneous(data);
                double time_sim = timer.elapsed_seconds();
                results.emplace_back(size, "simultaneo", data_type, run, seeds[run], time_sim, 0, "");
                
                // Corretude (todas iguais)
                if (result_sep_pair != result_sim_pair) {
                    cerr << endl << "ERRO: Resultados diferentes!" << endl;
                    return 1;
                }
                
                if (!quiet) cout << "." << flush;
            }
            
            if (!quiet) {
                // Calcula estatísticas
                vector<double> t_sep, t_sim;
                for (const auto& r : results) {
                    if (r.size == size && r.data_type == data_type) {
                        if (r.algorithm == "não simultaneo") t_sep.push_back(r.time_s);
                        else if (r.algorithm == "simultaneo") t_sim.push_back(r.time_s);
                    }
                }
                double mean_sep = calculate_mean(t_sep);
                double mean_sim = calculate_mean(t_sim);
                
                cout << " ✓" << endl;
                cout << "    Não simultaneo (2-pass):  " << fixed << setprecision(6) << (mean_sep * 1000) << " ms" << endl;
                cout << "    Simultaneo:               " << fixed << setprecision(6) << (mean_sim * 1000) << " ms" << endl;
                cout << "    Speedup (simultaneo/nao): " << fixed << setprecision(3) << (mean_sep / mean_sim) << "x" << endl;
            }
        }
        
        if (!quiet) cout << endl;
    }
    
    // Salva resultados
    if (!output_csv.empty()) {
        save_results_csv(results, output_csv, get_hostname(), "CPU info", get_compiler_info());
    }
    
    if (!quiet) {
        cout << "========================================" << endl;
        cout << "Benchmark concluído!" << endl;
        cout << "Total de testes: " << results.size() << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}

