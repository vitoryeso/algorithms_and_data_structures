#include "questao_utils.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sys/utsname.h>

using namespace std;

// ============================================================================
// GERAÇÃO DE DADOS
// ============================================================================

vector<int> generate_random_data(int n, int min_val, int max_val, unsigned seed) {
    vector<int> data(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(min_val, max_val);
    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }
    return data;
}

vector<int> generate_sorted_data(int n, int min_val) {
    vector<int> data(n);
    for (int i = 0; i < n; i++) {
        data[i] = min_val + i;
    }
    return data;
}

vector<int> generate_reverse_data(int n, int min_val) {
    vector<int> data(n);
    for (int i = 0; i < n; i++) {
        data[i] = min_val + (n - 1 - i);
    }
    return data;
}

vector<int> generate_with_duplicates(int n, int num_unique, unsigned seed) {
    vector<int> data(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, num_unique - 1);
    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }
    return data;
}

vector<double> generate_random_weights(int n, unsigned seed) {
    vector<double> weights(n);
    mt19937 rng(seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    // Gera pesos aleatórios
    for (int i = 0; i < n; i++) {
        weights[i] = dist(rng);
    }
    
    // Normaliza para somar 1.0
    double sum = accumulate(weights.begin(), weights.end(), 0.0);
    for (int i = 0; i < n; i++) {
        weights[i] /= sum;
    }
    
    return weights;
}

// ============================================================================
// TIMER E BENCHMARK
// ============================================================================

void Timer::start() {
    start_time = chrono::high_resolution_clock::now();
}

double Timer::elapsed_seconds() {
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;
    return duration.count();
}

// ============================================================================
// ESTATÍSTICAS
// ============================================================================

double calculate_mean(const vector<double>& data) {
    if (data.empty()) return 0.0;
    return accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double calculate_median(vector<double> data) {
    if (data.empty()) return 0.0;
    sort(data.begin(), data.end());
    size_t n = data.size();
    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        return data[n/2];
    }
}

double calculate_std(const vector<double>& data, double mean) {
    if (data.size() <= 1) return 0.0;
    double sum_sq = 0.0;
    for (double val : data) {
        double diff = val - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (data.size() - 1));
}

double calculate_min(const vector<double>& data) {
    if (data.empty()) return 0.0;
    return *min_element(data.begin(), data.end());
}

double calculate_max(const vector<double>& data) {
    if (data.empty()) return 0.0;
    return *max_element(data.begin(), data.end());
}

// ============================================================================
// UTILITÁRIOS DE ARQUIVO
// ============================================================================

void save_results_csv(const vector<BenchmarkResult>& results, 
                     const string& filename,
                     const string& hostname,
                     const string& cpu_info,
                     const string& compiler_info) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }
    
    // Cabeçalho
    file << "size,algorithm,data_type,run_index,seed,time_s,comparisons,metadata,"
         << "hostname,cpu_info,compiler_info,timestamp" << endl;
    
    string timestamp = get_timestamp();
    
    // Dados
    for (const auto& result : results) {
        file << result.size << ","
             << result.algorithm << ","
             << result.data_type << ","
             << result.run_index << ","
             << result.seed << ","
             << fixed << setprecision(9) << result.time_s << ","
             << result.comparisons << ","
             << "\"" << result.metadata << "\","
             << "\"" << hostname << "\","
             << "\"" << cpu_info << "\","
             << "\"" << compiler_info << "\","
             << "\"" << timestamp << "\"" << endl;
    }
    
    file.close();
    cout << "Resultados salvos em: " << filename << endl;
}

string get_hostname() {
    struct utsname uname_info;
    if (uname(&uname_info) == 0) {
        return string(uname_info.nodename);
    }
    return "unknown";
}

string get_compiler_info() {
    #ifdef __clang__
        return "Clang " + to_string(__clang_major__) + "." +
               to_string(__clang_minor__) + "." + to_string(__clang_patchlevel__);
    #elif defined(__GNUC__)
        return "GCC " + to_string(__GNUC__) + "." +
               to_string(__GNUC_MINOR__) + "." + to_string(__GNUC_PATCHLEVEL__);
    #else
        return "Unknown";
    #endif
}

string get_timestamp() {
    auto now = chrono::system_clock::now();
    auto now_c = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// ============================================================================
// SEEDS
// ============================================================================

vector<unsigned> generate_seeds(unsigned base_seed, int num_runs) {
    vector<unsigned> seeds;
    for (int i = 0; i < num_runs; i++) {
        seeds.push_back(base_seed + i);
    }
    return seeds;
}

// ============================================================================
// UTILITÁRIOS DE CLI
// ============================================================================

vector<string> split_string(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<int> generate_sizes(int min_size, int max_size) {
    vector<int> sizes;
    int current = min_size;
    
    while (current <= max_size) {
        sizes.push_back(current);
        
        if (current < 1000) {
            current += 100;
        } else if (current < 10000) {
            current += 1000;
        } else if (current < 100000) {
            current += 10000;
        } else if (current < 1000000) {
            current += 100000;
        } else {
            current += 1000000;
        }
        
        if (sizes.size() > 100) break; // Limita número de tamanhos
    }
    
    // Garante que max_size seja incluído
    if (!sizes.empty() && sizes.back() != max_size && max_size > sizes.back()) {
        sizes.push_back(max_size);
    }
    
    return sizes;
}

