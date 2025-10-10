#include "hash_linear_probing.h"
#include "hash_double_hashing.h"
#include "../common/questao_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Benchmark da Questão 15: Hash Probing" << endl;
    cout << endl;
    cout << "Opções:" << endl;
    cout << "  --max-size <n>         Tamanho máximo (padrão: 1000000)" << endl;
    cout << "  --min-size <n>         Tamanho mínimo (padrão: 1000)" << endl;
    cout << "  --runs <n>             Número de execuções por teste (padrão: 3)" << endl;
    cout << "  --load-factor <f>      Load factor para testes (padrão: 0.7)" << endl;
    cout << "  --output-csv <arquivo> Arquivo CSV de saída (opcional)" << endl;
    cout << "  --quiet                Modo silencioso" << endl;
    cout << "  --help, -h             Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Este benchmark compara:" << endl;
    cout << "  - Linear Probing" << endl;
    cout << "  - Double Hashing" << endl;
    cout << endl;
    cout << "Operações testadas:" << endl;
    cout << "  - INSERT: Inserção de elementos" << endl;
    cout << "  - SEARCH: Busca de elementos existentes" << endl;
    cout << "  - SEARCH_MISS: Busca de elementos inexistentes" << endl;
    cout << "  - REMOVE: Remoção de elementos" << endl;
}

int main(int argc, char* argv[]) {
    int max_size = 1000000;
    int min_size = 1000;
    int num_runs = 3;
    double target_load_factor = 0.7;
    string output_csv = "";
    bool quiet = false;
    
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
        } else if (arg == "--load-factor" && i + 1 < argc) {
            target_load_factor = stod(argv[++i]);
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
    
    vector<int> sizes = generate_sizes(min_size, max_size);
    vector<unsigned> seeds = generate_seeds(42, num_runs);
    
    if (!quiet) {
        cout << "========================================" << endl;
        cout << "QUESTÃO 15: Hash Probing" << endl;
        cout << "========================================" << endl;
        cout << "Tamanhos: " << sizes.size() << " valores de " 
             << sizes.front() << " até " << sizes.back() << endl;
        cout << "Execuções por teste: " << num_runs << endl;
        cout << "Load factor alvo: " << target_load_factor << endl;
        cout << "========================================" << endl << endl;
    }
    
    vector<BenchmarkResult> results;
    vector<string> operations = {"insert", "search", "search_miss", "remove"};
    
    for (const string& operation : operations) {
        if (!quiet) {
            cout << "Testando operação: " << operation << endl;
        }
        
        for (int size : sizes) {
            int table_size = static_cast<int>(size / target_load_factor);
            
            if (!quiet) {
                cout << "  Tamanho n=" << size << " (tabela=" << table_size << "): " << flush;
            }
            
            for (int run = 0; run < num_runs; run++) {
                vector<int> data = generate_random_data(size, 0, size * 10, seeds[run]);
                vector<int> search_data = generate_random_data(size, 0, size * 10, seeds[run] + 1000);
                
                Timer timer;
                
                // ============ Linear Probing ============
                HashTableLinearProbing ht_linear(table_size);
                
                if (operation == "insert") {
                    ht_linear.reset_probe_count();
                    timer.start();
                    for (int val : data) {
                        ht_linear.insert(val);
                    }
                    double time_linear = timer.elapsed_seconds();
                    
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_linear.load_factor()
                            << ";probes=" << ht_linear.get_total_probes()
                            << ";avg_probes=" << setprecision(2) << ht_linear.average_probes_per_search()
                            << ";max_cluster=" << ht_linear.max_cluster_size();
                    
                    results.emplace_back(size, "linear_probing", operation, run, seeds[run], 
                                       time_linear, ht_linear.get_total_probes(), metadata.str());
                } else {
                    for (int val : data) {
                        ht_linear.insert(val);
                    }
                    
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_linear.load_factor()
                            << ";avg_probes=" << setprecision(2) << ht_linear.average_probes_per_search()
                            << ";max_cluster=" << ht_linear.max_cluster_size();
                    
                    ht_linear.reset_probe_count();
                    
                    if (operation == "search") {
                        timer.start();
                        int found = 0;
                        for (int val : data) {
                            if (ht_linear.search(val)) found++;
                        }
                        double time_linear = timer.elapsed_seconds();
                        metadata << ";probes=" << ht_linear.get_total_probes();
                        results.emplace_back(size, "linear_probing", operation, run, seeds[run], 
                                           time_linear, ht_linear.get_total_probes(), metadata.str());
                    } else if (operation == "search_miss") {
                        timer.start();
                        int found = 0;
                        for (int val : search_data) {
                            if (ht_linear.search(val)) found++;
                        }
                        double time_linear = timer.elapsed_seconds();
                        metadata << ";probes=" << ht_linear.get_total_probes();
                        results.emplace_back(size, "linear_probing", operation, run, seeds[run], 
                                           time_linear, ht_linear.get_total_probes(), metadata.str());
                    } else if (operation == "remove") {
                        timer.start();
                        int removed = 0;
                        for (int val : data) {
                            if (ht_linear.remove(val)) removed++;
                        }
                        double time_linear = timer.elapsed_seconds();
                        results.emplace_back(size, "linear_probing", operation, run, seeds[run], 
                                           time_linear, removed, metadata.str());
                    }
                }
                
                // ============ Double Hashing ============
                HashTableDoubleHashing ht_double(table_size);
                
                if (operation == "insert") {
                    ht_double.reset_probe_count();
                    timer.start();
                    for (int val : data) {
                        ht_double.insert(val);
                    }
                    double time_double = timer.elapsed_seconds();
                    
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_double.load_factor()
                            << ";probes=" << ht_double.get_total_probes()
                            << ";avg_probes=" << setprecision(2) << ht_double.average_probes_per_search()
                            << ";max_cluster=" << ht_double.max_cluster_size();
                    
                    results.emplace_back(size, "double_hashing", operation, run, seeds[run], 
                                       time_double, ht_double.get_total_probes(), metadata.str());
                } else {
                    for (int val : data) {
                        ht_double.insert(val);
                    }
                    
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_double.load_factor()
                            << ";avg_probes=" << setprecision(2) << ht_double.average_probes_per_search()
                            << ";max_cluster=" << ht_double.max_cluster_size();
                    
                    ht_double.reset_probe_count();
                    
                    if (operation == "search") {
                        timer.start();
                        int found = 0;
                        for (int val : data) {
                            if (ht_double.search(val)) found++;
                        }
                        double time_double = timer.elapsed_seconds();
                        metadata << ";probes=" << ht_double.get_total_probes();
                        results.emplace_back(size, "double_hashing", operation, run, seeds[run], 
                                           time_double, ht_double.get_total_probes(), metadata.str());
                    } else if (operation == "search_miss") {
                        timer.start();
                        int found = 0;
                        for (int val : search_data) {
                            if (ht_double.search(val)) found++;
                        }
                        double time_double = timer.elapsed_seconds();
                        metadata << ";probes=" << ht_double.get_total_probes();
                        results.emplace_back(size, "double_hashing", operation, run, seeds[run], 
                                           time_double, ht_double.get_total_probes(), metadata.str());
                    } else if (operation == "remove") {
                        timer.start();
                        int removed = 0;
                        for (int val : data) {
                            if (ht_double.remove(val)) removed++;
                        }
                        double time_double = timer.elapsed_seconds();
                        results.emplace_back(size, "double_hashing", operation, run, seeds[run], 
                                           time_double, removed, metadata.str());
                    }
                }
                
                if (!quiet) cout << "." << flush;
            }
            
            if (!quiet) {
                vector<double> t_linear, t_double;
                for (const auto& r : results) {
                    if (r.size == size && r.data_type == operation) {
                        if (r.algorithm == "linear_probing") t_linear.push_back(r.time_s);
                        else if (r.algorithm == "double_hashing") t_double.push_back(r.time_s);
                    }
                }
                double mean_linear = calculate_mean(t_linear);
                double mean_double = calculate_mean(t_double);
                
                cout << " ✓" << endl;
                cout << "    Linear Probing:  " << fixed << setprecision(6) << (mean_linear * 1000) << " ms" << endl;
                cout << "    Double Hashing:  " << fixed << setprecision(6) << (mean_double * 1000) << " ms" << endl;
                cout << "    Razão (linear/double): " << fixed << setprecision(3) << (mean_linear / mean_double) << "x" << endl;
            }
        }
        
        if (!quiet) cout << endl;
    }
    
    if (!output_csv.empty()) {
        save_results_csv(results, output_csv, get_hostname(), "CPU info", get_compiler_info());
    }
    
    if (!quiet) {
        cout << "========================================" << endl;
        cout << "Benchmark concluído!" << endl;
        cout << "Total de testes: " << results.size() << endl;
        if (!output_csv.empty()) {
            cout << "Resultados salvos em: " << output_csv << endl;
        }
        cout << "========================================" << endl;
    }
    
    return 0;
}

