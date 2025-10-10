#ifndef QUESTAO_UTILS_H
#define QUESTAO_UTILS_H

#include <vector>
#include <string>
#include <chrono>

// ============================================================================
// GERAÇÃO DE DADOS
// ============================================================================

std::vector<int> generate_random_data(int n, int min_val, int max_val, unsigned seed);
std::vector<int> generate_sorted_data(int n, int min_val = 0);
std::vector<int> generate_reverse_data(int n, int min_val = 0);
std::vector<int> generate_with_duplicates(int n, int num_unique, unsigned seed);
std::vector<double> generate_random_weights(int n, unsigned seed);

// ============================================================================
// TIMER E BENCHMARK
// ============================================================================

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start();
    double elapsed_seconds();
};

struct BenchmarkResult {
    int size;
    std::string algorithm;
    std::string data_type;
    int run_index;
    unsigned seed;
    double time_s;
    long long comparisons;  // Para contar comparações quando aplicável
    std::string metadata;
    
    BenchmarkResult(int s, std::string alg, std::string dt, int run, 
                   unsigned sd, double t, long long comp = 0, std::string meta = "")
        : size(s), algorithm(alg), data_type(dt), run_index(run), 
          seed(sd), time_s(t), comparisons(comp), metadata(meta) {}
};

// ============================================================================
// ESTATÍSTICAS
// ============================================================================

double calculate_mean(const std::vector<double>& data);
double calculate_median(std::vector<double> data);
double calculate_std(const std::vector<double>& data, double mean);
double calculate_min(const std::vector<double>& data);
double calculate_max(const std::vector<double>& data);

// ============================================================================
// UTILITÁRIOS DE ARQUIVO
// ============================================================================

void save_results_csv(const std::vector<BenchmarkResult>& results, 
                     const std::string& filename,
                     const std::string& hostname = "",
                     const std::string& cpu_info = "",
                     const std::string& compiler_info = "");

std::string get_hostname();
std::string get_compiler_info();
std::string get_timestamp();

// ============================================================================
// SEEDS
// ============================================================================

std::vector<unsigned> generate_seeds(unsigned base_seed, int num_runs);

// ============================================================================
// UTILITÁRIOS DE CLI
// ============================================================================

std::vector<std::string> split_string(const std::string& s, char delimiter);
std::vector<int> generate_sizes(int min_size, int max_size);

#endif // QUESTAO_UTILS_H

