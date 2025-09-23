#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <thread>
#include <cmath>
#include <sys/utsname.h>
#include "algorithms/cpp/luAlgorithms.h"

using namespace std;
using namespace chrono;

struct BenchmarkResult {
    int size;
    string algorithm;
    int run_index;
    unsigned seed;
    double time_s;
    int max_val;
    int base;
    int digits;
    int target_passes; // desejado ao varrer --passes

    BenchmarkResult(int s, string alg, int run, unsigned sd, double t,
                    int max_v, int b, int d, int tp)
        : size(s), algorithm(alg), run_index(run), seed(sd), time_s(t),
          max_val(max_v), base(b), digits(d), target_passes(tp) {}
};

vector<int> generate_uniform_vector(int n, unsigned seed, int max_val) {
    vector<int> V(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, max_val);
    for (int i = 0; i < n; i++) V[i] = dist(rng);
    return V;
}

int compute_max_value(const vector<int>& V) {
    if (V.empty()) return 0;
    return *max_element(V.begin(), V.end());
}

int compute_digits_base(int max_val, int base) {
    if (max_val <= 0) return 1;
    int d = 0;
    long long p = 1;
    while (p <= max_val) {
        d++;
        p *= base;
    }
    return d;
}

double run_counting_sort(vector<int> V) {
    if (V.empty()) return 0.0;
    int k = compute_max_value(V); // k deve ser o maior valor do vetor
    auto start = high_resolution_clock::now();
    counting_sort(V, k);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double run_radix_sort(vector<int> V, int base) {
    if (V.empty()) return 0.0;
    int max_v = compute_max_value(V);
    int d = compute_digits_base(max_v, base);
    auto start = high_resolution_clock::now();
    radix_sort(V, base, (unsigned)d);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

vector<unsigned> generate_seeds(unsigned base_seed, int num_runs) {
    vector<unsigned> seeds;
    for (int i = 0; i < num_runs; i++) seeds.push_back(base_seed + i);
    return seeds;
}

void save_csv(const vector<BenchmarkResult>& results, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }

    file << "size,algorithm,run_index,seed,time_s,max_val,base,digits,target_passes" << endl;
    for (const auto& r : results) {
        file << r.size << ","
             << r.algorithm << ","
             << r.run_index << ","
             << r.seed << ","
             << fixed << setprecision(8) << r.time_s << ","
             << r.max_val << ","
             << r.base << ","
             << r.digits << ","
             << r.target_passes
             << endl;
    }
    file.close();
    cout << "Resultados salvos em: " << filename << endl;
}

vector<string> split_string(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) tokens.push_back(token);
    return tokens;
}

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Opções:" << endl;
    cout << "  --sizes <lista>        Tamanhos separados por espaço" << endl;
    cout << "  --max-vals <lista>     Valores máximos separados por espaço (geração em [0,max_val])" << endl;
    cout << "  --runs <num>           Número de execuções por configuração (padrão: 5)" << endl;
    cout << "  --base <num>           Base do radix sort (padrão: 10)" << endl;
    cout << "  --seed <num>           Seed base (padrão: 42)" << endl;
    cout << "  --output <arquivo>     Arquivo CSV de saída (padrão: num_sorting_results.csv)" << endl;
    cout << "  --quiet                Modo silencioso" << endl;
    cout << "  --k-equals-n-plus-one  Define max_val = size + 1 para cada size (ignora --max-vals)" << endl;
    cout << "  --passes <lista>       Varrer número de passadas-alvo do radix (ex.: 2 3 4 5 6)" << endl;
    cout << "  --help, -h             Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Algoritmos comparados: counting_sort (O(n+k)) vs radix_sort (LSD, estável, base variável)" << endl;
}

int main(int argc, char* argv[]) {
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000};
    vector<int> max_vals = {100, 1000, 10000, 100000, 1000000};
    int num_runs = 5;
    int base = 10;
    unsigned base_seed = 42;
    string output_csv = "num_sorting_results.csv";
    bool quiet = false;
    bool k_equals_n_plus_one = false;
    vector<int> radix_passes; // se vazio, usa base fixa

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--sizes" && i + 1 < argc) {
            sizes.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                sizes.push_back(stoi(argv[i]));
                i++;
            }
            i--;
        } else if (arg == "--max-vals" && i + 1 < argc) {
            max_vals.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                max_vals.push_back(stoi(argv[i]));
                i++;
            }
            i--;
        } else if (arg == "--runs" && i + 1 < argc) {
            num_runs = stoi(argv[++i]);
        } else if (arg == "--base" && i + 1 < argc) {
            base = stoi(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            base_seed = stoul(argv[++i]);
        } else if (arg == "--output" && i + 1 < argc) {
            output_csv = argv[++i];
        } else if (arg == "--quiet") {
            quiet = true;
        } else if (arg == "--k-equals-n-plus-one") {
            k_equals_n_plus_one = true;
        } else if (arg == "--passes") {
            radix_passes.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                radix_passes.push_back(stoi(argv[i]));
                i++;
            }
            i--;
        } else {
            cerr << "Argumento desconhecido: " << arg << endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    if (base < 2) {
        cerr << "Erro: base do radix deve ser >= 2" << endl;
        return 1;
    }

    vector<unsigned> seeds = generate_seeds(base_seed, num_runs);
    vector<BenchmarkResult> all_results;

    if (!k_equals_n_plus_one) {
        for (int max_val : max_vals) {
            if (!quiet) cout << "max_val=" << max_val << endl;

            for (int size : sizes) {
                if (!quiet) cout << "  n=" << size << ": ";

                for (int run = 0; run < num_runs; run++) {
                    if (!quiet) cout << "." << flush;

                    unsigned seed = seeds[run];
                    vector<int> V = generate_uniform_vector(size, seed, max_val);

                    double t_count = run_counting_sort(V);
                    all_results.emplace_back(size, "counting_sort", run, seed, t_count,
                                             max_val, /*base*/ 0, /*digits*/ 0, /*target*/ 0);

                    if (!radix_passes.empty()) {
                        for (int tgt : radix_passes) {
                            if (tgt < 1) continue;
                            int base_for_tgt = (int) std::max(2.0, floor(pow((double)max_val, 1.0 / (double)tgt)));
                            int digits = compute_digits_base(max_val, base_for_tgt);
                            double t_radix = run_radix_sort(V, base_for_tgt);
                            all_results.emplace_back(size, "radix_sort", run, seed, t_radix,
                                                     max_val, base_for_tgt, digits, tgt);
                        }
                    } else {
                        int digits = compute_digits_base(max_val, base);
                        double t_radix = run_radix_sort(V, base);
                        all_results.emplace_back(size, "radix_sort", run, seed, t_radix,
                                                 max_val, base, digits, /*target*/ 0);
                    }
                }

                if (!quiet) cout << " ✓" << endl;
            }
            if (!quiet) cout << endl;
        }
    } else {
        if (!quiet) cout << "Usando max_val = size + 1" << endl;
        for (int size : sizes) {
            int max_val = size + 1;
            if (!quiet) cout << "  n=" << size << " (max_val=" << max_val << "): ";
            for (int run = 0; run < num_runs; run++) {
                if (!quiet) cout << "." << flush;

                unsigned seed = seeds[run];
                vector<int> V = generate_uniform_vector(size, seed, max_val);

                double t_count = run_counting_sort(V);
                all_results.emplace_back(size, "counting_sort", run, seed, t_count,
                                         max_val, /*base*/ 0, /*digits*/ 0, /*target*/ 0);

                if (!radix_passes.empty()) {
                    for (int tgt : radix_passes) {
                        if (tgt < 1) continue;
                        int base_for_tgt = (int) std::max(2.0, floor(pow((double)max_val, 1.0 / (double)tgt)));
                        int digits = compute_digits_base(max_val, base_for_tgt);
                        double t_radix = run_radix_sort(V, base_for_tgt);
                        all_results.emplace_back(size, "radix_sort", run, seed, t_radix,
                                                 max_val, base_for_tgt, digits, tgt);
                    }
                } else {
                    int digits = compute_digits_base(max_val, base);
                    double t_radix = run_radix_sort(V, base);
                    all_results.emplace_back(size, "radix_sort", run, seed, t_radix,
                                             max_val, base, digits, /*target*/ 0);
                }
            }
            if (!quiet) cout << " ✓" << endl;
        }
    }

    save_csv(all_results, output_csv);
    if (!quiet) cout << "Benchmark concluído com sucesso!" << endl;
    return 0;
}


