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

// ============================================================================
// STRUCTS PARA RESULTADOS
// ============================================================================

struct BenchmarkResult {
    int size;
    string algorithm;
    string input_pattern;
    int run_index;
    unsigned seed;
    double time_s;
    int min_val;
    int max_val;

    BenchmarkResult(int s, string alg, string pattern, int run, unsigned sd, double t,
                   int min_v, int max_v)
        : size(s), algorithm(alg), input_pattern(pattern), run_index(run), seed(sd), time_s(t),
          min_val(min_v), max_val(max_v) {}
};

struct StatsResult {
    int size;
    string algorithm;
    string input_pattern;
    int num_runs;
    double mean_time;
    double median_time;
    double std_time;
    double min_time;
    double max_time;
    int min_val;
    int max_val;
};

struct EnvironmentInfo {
    string cpu_model;
    int cpu_cores;
    string compiler_version;
    string os_info;
    string timestamp;
    string hostname;
};

// ============================================================================
// UTILITÁRIOS
// ============================================================================

vector<int> generate_test_vector(int n, const string& pattern, unsigned seed, int min_val, int max_val) {
    vector<int> V(n);
    mt19937 rng(seed);

    if (pattern == "ordered") {
        // Vetor já ordenado
        for (int i = 0; i < n; i++) {
            V[i] = min_val + i;
        }
    } else if (pattern == "reverse") {
        // Vetor em ordem reversa
        for (int i = 0; i < n; i++) {
            V[i] = max_val - i;
        }
    } else if (pattern == "random") {
        // Vetor aleatório
        uniform_int_distribution<int> dist(min_val, max_val);
        for (int i = 0; i < n; i++) {
            V[i] = dist(rng);
        }
    } else if (pattern == "nearly_sorted") {
        // Vetor quase ordenado (alguns elementos fora de ordem)
        for (int i = 0; i < n; i++) {
            V[i] = min_val + i;
        }
        // Troca alguns elementos
        uniform_int_distribution<int> dist(0, n-1);
        for (int i = 0; i < n/10; i++) { // 10% dos elementos
            int idx1 = dist(rng);
            int idx2 = dist(rng);
            swap(V[idx1], V[idx2]);
        }
    } else {
        throw runtime_error("Padrão de entrada desconhecido: " + pattern);
    }

    return V;
}

double benchmark_sorting_algorithm(const string& algorithm, int size, const string& pattern,
                                  unsigned seed, int min_val, int max_val) {
    vector<int> V = generate_test_vector(size, pattern, seed, min_val, max_val);
    vector<int> V_copy = V; // Cópia para algoritmos que modificam o vetor

    auto start = high_resolution_clock::now();

    if (algorithm == "selection_sort") {
        selection_sort(V_copy);
    } else if (algorithm == "insertion_sort") {
        insertion_sort(V_copy);
    } else if (algorithm == "quick_sort") {
        quick_sort(V_copy);
    } else if (algorithm == "merge_sort") {
        merge_sort(V_copy);
    } else if (algorithm == "counting_sort") {
        // Para counting sort, precisamos determinar o range
        int k = max_val - min_val + 1;
        counting_sort(V_copy, k);
    } else if (algorithm == "bucket_sort") {
        // Para bucket sort, precisamos de parâmetros
        int k = max_val - min_val + 1;
        unsigned n_buckets = min(100u, (unsigned)sqrt(size));
        bucket_sort(V_copy, k, n_buckets);
    } else if (algorithm == "radix_sort") {
        // Para radix sort, assumimos base 10 e dígitos suficientes
        radix_sort(V_copy, 10, 10);
    } else {
        throw runtime_error("Algoritmo desconhecido: " + algorithm);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

vector<unsigned> generate_seeds(unsigned base_seed, int num_runs) {
    vector<unsigned> seeds;
    for (int i = 0; i < num_runs; i++) {
        seeds.push_back(base_seed + i);
    }
    return seeds;
}

double calculate_median(vector<double>& values) {
    sort(values.begin(), values.end());
    size_t n = values.size();
    if (n % 2 == 0) {
        return (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        return values[n/2];
    }
}

double calculate_std(const vector<double>& values, double mean) {
    if (values.size() <= 1) return 0.0;

    double sum_sq = 0.0;
    for (double val : values) {
        double diff = val - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (values.size() - 1));
}

StatsResult calculate_stats(const vector<BenchmarkResult>& results) {
    vector<double> times;
    for (const auto& result : results) {
        times.push_back(result.time_s);
    }

    double mean_t = accumulate(times.begin(), times.end(), 0.0) / times.size();
    double median_t = calculate_median(times);
    double std_t = calculate_std(times, mean_t);
    double min_t = *min_element(times.begin(), times.end());
    double max_t = *max_element(times.begin(), times.end());

    const auto& first = results[0];
    return StatsResult{
        first.size, first.algorithm, first.input_pattern, (int)results.size(),
        mean_t, median_t, std_t, min_t, max_t, first.min_val, first.max_val
    };
}

EnvironmentInfo collect_environment_info() {
    EnvironmentInfo info;

    // CPU info
    info.cpu_cores = thread::hardware_concurrency();

    // OS info
    struct utsname uname_info;
    uname(&uname_info);
    info.os_info = string(uname_info.sysname) + " " + uname_info.release;
    info.hostname = uname_info.nodename;

    // Timestamp
    auto now = system_clock::now();
    auto now_c = system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    info.timestamp = ss.str();

    // Compiler version (GCC/Clang detection)
    #ifdef __clang__
        info.compiler_version = "Clang " + to_string(__clang_major__) + "." +
                               to_string(__clang_minor__) + "." + to_string(__clang_patchlevel__);
    #elif defined(__GNUC__)
        info.compiler_version = "GCC " + to_string(__GNUC__) + "." +
                               to_string(__GNUC_MINOR__) + "." + to_string(__GNUC_PATCHLEVEL__);
    #else
        info.compiler_version = "Unknown";
    #endif

    // CPU model (simplified)
    info.cpu_model = "Unknown"; // Would need platform-specific code for full detection

    return info;
}

// ============================================================================
// SAÍDA DE DADOS
// ============================================================================

void save_csv(const vector<BenchmarkResult>& results, const string& filename,
              const EnvironmentInfo& env) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }

    // Cabeçalho
    file << "size,algorithm,input_pattern,run_index,seed,time_s,min_val,max_val,"
         << "cpu_model,cpu_cores,compiler_version,os_info,timestamp,hostname" << endl;

    // Dados
    for (const auto& result : results) {
        file << result.size << ","
             << result.algorithm << ","
             << result.input_pattern << ","
             << result.run_index << ","
             << result.seed << ","
             << fixed << setprecision(8) << result.time_s << ","
             << result.min_val << ","
             << result.max_val << ","
             << "\"" << env.cpu_model << "\","
             << env.cpu_cores << ","
             << "\"" << env.compiler_version << "\","
             << "\"" << env.os_info << "\","
             << "\"" << env.timestamp << "\","
             << "\"" << env.hostname << "\"" << endl;
    }

    file.close();
    cout << "Resultados salvos em: " << filename << endl;
}

void save_json(const vector<BenchmarkResult>& results, const string& filename,
               const EnvironmentInfo& env) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }

    file << "{" << endl;
    file << "  \"metadata\": {" << endl;
    file << "    \"cpu_model\": \"" << env.cpu_model << "\"," << endl;
    file << "    \"cpu_cores\": " << env.cpu_cores << "," << endl;
    file << "    \"compiler_version\": \"" << env.compiler_version << "\"," << endl;
    file << "    \"os_info\": \"" << env.os_info << "\"," << endl;
    file << "    \"timestamp\": \"" << env.timestamp << "\"," << endl;
    file << "    \"hostname\": \"" << env.hostname << "\"" << endl;
    file << "  }," << endl;
    file << "  \"results\": [" << endl;

    for (size_t i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        file << "    {" << endl;
        file << "      \"size\": " << result.size << "," << endl;
        file << "      \"algorithm\": \"" << result.algorithm << "\"," << endl;
        file << "      \"input_pattern\": \"" << result.input_pattern << "\"," << endl;
        file << "      \"run_index\": " << result.run_index << "," << endl;
        file << "      \"seed\": " << result.seed << "," << endl;
        file << "      \"time_s\": " << fixed << setprecision(8) << result.time_s << "," << endl;
        file << "      \"min_val\": " << result.min_val << "," << endl;
        file << "      \"max_val\": " << result.max_val << endl;
        file << "    }";
        if (i < results.size() - 1) file << ",";
        file << endl;
    }

    file << "  ]" << endl;
    file << "}" << endl;

    file.close();
    cout << "Resultados salvos em: " << filename << endl;
}

// ============================================================================
// CLI E MAIN
// ============================================================================

void print_usage(const string& program_name) {
    cout << "Uso: " << program_name << " [opções]" << endl;
    cout << endl;
    cout << "Opções:" << endl;
    cout << "  --algorithms <lista>     Algoritmos separados por vírgula (padrão: selection_sort,insertion_sort,quick_sort,merge_sort)" << endl;
    cout << "  --patterns <lista>       Padrões de entrada separados por vírgula (padrão: ordered,reverse,random,nearly_sorted)" << endl;
    cout << "  --sizes <tamanhos>       Lista de tamanhos separados por espaço" << endl;
    cout << "  --max-size <max>         Tamanho máximo (gera tamanhos até max)" << endl;
    cout << "  --min-size <min>         Tamanho mínimo (padrão: 100)" << endl;
    cout << "  --runs <num>             Número de execuções por teste (padrão: 3)" << endl;
    cout << "  --seeds <lista>          Seeds específicas separadas por espaço" << endl;
    cout << "  --base-seed <num>        Seed base para geração automática (padrão: 42)" << endl;
    cout << "  --min-val <val>          Valor mínimo dos elementos (padrão: 0)" << endl;
    cout << "  --max-val <val>          Valor máximo dos elementos (padrão: 100000)" << endl;
    cout << "  --output-csv <arquivo>   Arquivo CSV de saída (opcional)" << endl;
    cout << "  --output-json <arquivo>  Arquivo JSON de saída (opcional)" << endl;
    cout << "  --quiet                  Modo silencioso (menos saída)" << endl;
    cout << "  --help, -h               Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Algoritmos disponíveis: selection_sort, insertion_sort, quick_sort, merge_sort, counting_sort, bucket_sort, radix_sort" << endl;
    cout << "Padrões de entrada: ordered, reverse, random, nearly_sorted" << endl;
    cout << endl;
    cout << "Exemplos:" << endl;
    cout << "  " << program_name << " --algorithms quick_sort,merge_sort --patterns ordered,random --sizes 1000 5000 10000" << endl;
    cout << "  " << program_name << " --max-size 10000 --runs 5 --output-csv sorting_results.csv" << endl;
    cout << "  " << program_name << " --algorithms insertion_sort,selection_sort --patterns ordered,reverse --seeds 1 2 3 4 5" << endl;
}

vector<string> split_string(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    // Parâmetros padrão
    vector<string> algorithms = {"selection_sort", "insertion_sort", "quick_sort", "merge_sort"};
    vector<string> patterns = {"ordered", "reverse", "random", "nearly_sorted"};
    vector<int> sizes;
    int max_size = 10000;
    int min_size = 100;
    int num_runs = 3;
    vector<unsigned> seeds;
    unsigned base_seed = 42;
    int min_val = 0;
    int max_val = 100000;
    string output_csv = "";
    string output_json = "";
    bool quiet = false;

    // Parse dos argumentos
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--algorithms") {
            if (i + 1 >= argc) {
                cerr << "Erro: --algorithms requer um valor" << endl;
                return 1;
            }
            algorithms = split_string(argv[++i], ',');
        } else if (arg == "--patterns") {
            if (i + 1 >= argc) {
                cerr << "Erro: --patterns requer um valor" << endl;
                return 1;
            }
            patterns = split_string(argv[++i], ',');
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
        } else if (arg == "--seeds") {
            seeds.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                seeds.push_back(stoul(argv[i]));
                i++;
            }
            i--; // Ajusta pois o loop principal irá incrementar
        } else if (arg == "--base-seed" && i + 1 < argc) {
            base_seed = stoul(argv[++i]);
        } else if (arg == "--min-val" && i + 1 < argc) {
            min_val = stoi(argv[++i]);
        } else if (arg == "--max-val" && i + 1 < argc) {
            max_val = stoi(argv[++i]);
        } else if (arg == "--output-csv" && i + 1 < argc) {
            output_csv = argv[++i];
        } else if (arg == "--output-json" && i + 1 < argc) {
            output_json = argv[++i];
        } else if (arg == "--quiet") {
            quiet = true;
        } else {
            cerr << "Argumento desconhecido: " << arg << endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    // Valida algoritmos
    vector<string> valid_algorithms = {"selection_sort", "insertion_sort", "quick_sort", "merge_sort",
                                      "counting_sort", "bucket_sort", "radix_sort"};
    for (const auto& alg : algorithms) {
        if (find(valid_algorithms.begin(), valid_algorithms.end(), alg) == valid_algorithms.end()) {
            cerr << "Algoritmo inválido: " << alg << endl;
            return 1;
        }
    }

    // Valida padrões
    vector<string> valid_patterns = {"ordered", "reverse", "random", "nearly_sorted"};
    for (const auto& pat : patterns) {
        if (find(valid_patterns.begin(), valid_patterns.end(), pat) == valid_patterns.end()) {
            cerr << "Padrão inválido: " << pat << endl;
            return 1;
        }
    }

    // Se sizes não foi especificado, gera tamanhos
    if (sizes.empty()) {
        sizes = {100, 500, 1000, 2500, 5000, 7500, 10000};
        // Filtra tamanhos dentro do range
        vector<int> filtered_sizes;
        for (int size : sizes) {
            if (size >= min_size && size <= max_size) {
                filtered_sizes.push_back(size);
            }
        }
        sizes = filtered_sizes;
    }

    // Gera seeds se não foram especificadas
    if (seeds.empty()) {
        seeds = generate_seeds(base_seed, num_runs);
    } else {
        num_runs = seeds.size();
    }

    // Coleta informações do ambiente
    EnvironmentInfo env = collect_environment_info();

    if (!quiet) {
        cout << "=== Sorting Algorithms Benchmark ===" << endl;
        cout << "Algoritmos: ";
        for (size_t i = 0; i < algorithms.size(); i++) {
            cout << algorithms[i];
            if (i < algorithms.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Padrões de entrada: ";
        for (size_t i = 0; i < patterns.size(); i++) {
            cout << patterns[i];
            if (i < patterns.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Tamanhos: ";
        for (size_t i = 0; i < sizes.size(); i++) {
            cout << sizes[i];
            if (i < sizes.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Execuções por teste: " << num_runs << endl;
        cout << "Seeds: ";
        for (size_t i = 0; i < seeds.size(); i++) {
            cout << seeds[i];
            if (i < seeds.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Valores dos elementos: [" << min_val << ", " << max_val << "]" << endl;
        cout << "====================================" << endl << endl;
    }

    // Executa benchmarks
    vector<BenchmarkResult> all_results;

    for (const auto& algorithm : algorithms) {
        if (!quiet) cout << "Executando " << algorithm << "..." << endl;

        for (const auto& pattern : patterns) {
            for (int size : sizes) {
                if (!quiet) cout << "  " << pattern << " n=" << size << ": ";

                for (int run = 0; run < num_runs; run++) {
                    if (!quiet) cout << "." << flush;

                    try {
                        double time = benchmark_sorting_algorithm(algorithm, size, pattern,
                                                                seeds[run], min_val, max_val);

                        all_results.emplace_back(size, algorithm, pattern, run, seeds[run], time,
                                               min_val, max_val);
                    } catch (const exception& e) {
                        cerr << "Erro em " << algorithm << " " << pattern << " n=" << size
                             << " run=" << run << ": " << e.what() << endl;
                        return 1;
                    }
                }

                if (!quiet) cout << " ✓" << endl;
            }
        }

        if (!quiet) cout << endl;
    }

    // Salva resultados
    if (!output_csv.empty()) {
        save_csv(all_results, output_csv, env);
    }

    if (!output_json.empty()) {
        save_json(all_results, output_json, env);
    }

    if (!quiet) {
        cout << "Benchmark concluído com sucesso!" << endl;
        cout << "Total de execuções: " << all_results.size() << endl;
    }

    return 0;
}
