#include "hash_chaining.h"
#include "hash_open_addressing.h"
#include "../common/questao_utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <random>
#include <sstream>

using namespace std;

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Benchmark da Questão 14: Tabelas Hash" << endl;
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
    cout << "  - Hash com Encadeamento (Chaining)" << endl;
    cout << "  - Hash com Endereçamento Aberto (Open Addressing)" << endl;
    cout << endl;
    cout << "Operações testadas:" << endl;
    cout << "  - INSERT: Inserção de elementos" << endl;
    cout << "  - SEARCH: Busca de elementos existentes" << endl;
    cout << "  - SEARCH_MISS: Busca de elementos inexistentes" << endl;
    cout << "  - REMOVE: Remoção de elementos" << endl;
}

int main(int argc, char* argv[]) {
    // Parâmetros padrão
    int max_size = 1000000;
    int min_size = 1000;
    int num_runs = 3;
    double target_load_factor = 0.7;
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
    
    // Gera tamanhos
    vector<int> sizes = generate_sizes(min_size, max_size);
    
    // Gera seeds
    vector<unsigned> seeds = generate_seeds(42, num_runs);
    
    if (!quiet) {
        cout << "========================================" << endl;
        cout << "QUESTÃO 14: Tabelas Hash" << endl;
        cout << "========================================" << endl;
        cout << "Tamanhos: " << sizes.size() << " valores de " 
             << sizes.front() << " até " << sizes.back() << endl;
        cout << "Execuções por teste: " << num_runs << endl;
        cout << "Load factor alvo: " << target_load_factor << endl;
        cout << "========================================" << endl << endl;
    }
    
    vector<BenchmarkResult> results;
    
    // Operações para testar
    vector<string> operations = {"insert", "search", "search_miss", "remove"};
    
    for (const string& operation : operations) {
        if (!quiet) {
            cout << "Testando operação: " << operation << endl;
        }
        
        for (int size : sizes) {
            // Tamanho da tabela baseado no load factor
            int table_size = static_cast<int>(size / target_load_factor);
            
            if (!quiet) {
                cout << "  Tamanho n=" << size << " (tabela=" << table_size << "): " << flush;
            }
            
            for (int run = 0; run < num_runs; run++) {
                // Gera dados
                vector<int> data = generate_random_data(size, 0, size * 10, seeds[run]);
                vector<int> search_data = generate_random_data(size, 0, size * 10, seeds[run] + 1000);
                
                Timer timer;
                
                // ============ Chaining ============
                HashTableChaining ht_chain(table_size);
                
                if (operation == "insert") {
                    timer.start();
                    for (int val : data) {
                        ht_chain.insert(val);
                    }
                    double time_chain = timer.elapsed_seconds();
                    
                    // Coleta métricas estruturais
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_chain.load_factor()
                            << ";collisions=" << ht_chain.count_collisions()
                            << ";max_chain=" << ht_chain.max_chain_length();
                    
                    results.emplace_back(size, "chaining", operation, run, seeds[run], 
                                       time_chain, 0, metadata.str());
                } else {
                    // Pre-popula para search/remove
                    for (int val : data) {
                        ht_chain.insert(val);
                    }
                    
                    // Coleta métricas estruturais após população
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_chain.load_factor()
                            << ";collisions=" << ht_chain.count_collisions()
                            << ";max_chain=" << ht_chain.max_chain_length();
                    
                    if (operation == "search") {
                        timer.start();
                        int found = 0;
                        for (int val : data) {
                            if (ht_chain.search(val)) found++;
                        }
                        double time_chain = timer.elapsed_seconds();
                        results.emplace_back(size, "chaining", operation, run, seeds[run], 
                                           time_chain, found, metadata.str());
                    } else if (operation == "search_miss") {
                        timer.start();
                        int found = 0;
                        for (int val : search_data) {
                            if (ht_chain.search(val)) found++;
                        }
                        double time_chain = timer.elapsed_seconds();
                        results.emplace_back(size, "chaining", operation, run, seeds[run], 
                                           time_chain, found, metadata.str());
                    } else if (operation == "remove") {
                        timer.start();
                        int removed = 0;
                        for (int val : data) {
                            if (ht_chain.remove(val)) removed++;
                        }
                        double time_chain = timer.elapsed_seconds();
                        results.emplace_back(size, "chaining", operation, run, seeds[run], 
                                           time_chain, removed, metadata.str());
                    }
                }
                
                // ============ Open Addressing ============
                HashTableOpenAddressing ht_open(table_size);
                
                if (operation == "insert") {
                    timer.start();
                    for (int val : data) {
                        ht_open.insert(val);
                    }
                    double time_open = timer.elapsed_seconds();
                    
                    // Coleta métricas estruturais
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_open.load_factor()
                            << ";total_probes=" << ht_open.count_total_probes()
                            << ";max_cluster=" << ht_open.max_cluster_size();
                    
                    results.emplace_back(size, "open_addressing", operation, run, seeds[run], 
                                       time_open, 0, metadata.str());
                } else {
                    // Pre-popula para search/remove
                    for (int val : data) {
                        ht_open.insert(val);
                    }
                    
                    // Coleta métricas estruturais após população
                    stringstream metadata;
                    metadata << "load_factor=" << fixed << setprecision(4) << ht_open.load_factor()
                            << ";total_probes=" << ht_open.count_total_probes()
                            << ";max_cluster=" << ht_open.max_cluster_size();
                    
                    if (operation == "search") {
                        timer.start();
                        int found = 0;
                        for (int val : data) {
                            if (ht_open.search(val)) found++;
                        }
                        double time_open = timer.elapsed_seconds();
                        results.emplace_back(size, "open_addressing", operation, run, seeds[run], 
                                           time_open, found, metadata.str());
                    } else if (operation == "search_miss") {
                        timer.start();
                        int found = 0;
                        for (int val : search_data) {
                            if (ht_open.search(val)) found++;
                        }
                        double time_open = timer.elapsed_seconds();
                        results.emplace_back(size, "open_addressing", operation, run, seeds[run], 
                                           time_open, found, metadata.str());
                    } else if (operation == "remove") {
                        timer.start();
                        int removed = 0;
                        for (int val : data) {
                            if (ht_open.remove(val)) removed++;
                        }
                        double time_open = timer.elapsed_seconds();
                        results.emplace_back(size, "open_addressing", operation, run, seeds[run], 
                                           time_open, removed, metadata.str());
                    }
                }
                
                if (!quiet) cout << "." << flush;
            }
            
            if (!quiet) {
                // Calcula estatísticas
                vector<double> t_chain, t_open;
                for (const auto& r : results) {
                    if (r.size == size && r.data_type == operation) {
                        if (r.algorithm == "chaining") t_chain.push_back(r.time_s);
                        else if (r.algorithm == "open_addressing") t_open.push_back(r.time_s);
                    }
                }
                double mean_chain = calculate_mean(t_chain);
                double mean_open = calculate_mean(t_open);
                
                cout << " ✓" << endl;
                cout << "    Chaining:        " << fixed << setprecision(6) << (mean_chain * 1000) << " ms" << endl;
                cout << "    Open Addressing: " << fixed << setprecision(6) << (mean_open * 1000) << " ms" << endl;
                cout << "    Razão (chain/open): " << fixed << setprecision(3) << (mean_chain / mean_open) << "x" << endl;
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
        if (!output_csv.empty()) {
            cout << "Resultados salvos em: " << output_csv << endl;
        }
        cout << "========================================" << endl;
    }
    
    return 0;
}
