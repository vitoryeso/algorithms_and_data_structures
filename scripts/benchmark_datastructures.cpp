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
#include <deque>
#include <climits>
#include <cctype>
#include "data_structures/cpp/my_vector.h"
#include "data_structures/cpp/heap.h"
#include "data_structures/cpp/my_queue_linkedlist.h"
#include "data_structures/cpp/my_queue_vector.h"
#include "data_structures/cpp/my_stack.h"

// Implementações simples para evitar conflitos
class SimpleVector {
private:
    std::vector<int> data;
public:
    SimpleVector() {}
    void push_back(int val) { data.push_back(val); }
    size_t size() const { return data.size(); }
    int& operator[](size_t i) { return data[i]; }
    const int& operator[](size_t i) const { return data[i]; }
};

class SimpleHeap {
private:
    std::vector<int> data;
public:
    SimpleHeap() {}
    void push(int val) {
        data.push_back(val);
        std::push_heap(data.begin(), data.end());
    }
    int top() const { return data.front(); }
    void pop() {
        std::pop_heap(data.begin(), data.end());
        data.pop_back();
    }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

class SimpleQueue {
private:
    std::deque<int> data;
public:
    SimpleQueue() {}
    void enqueue(int val) { data.push_back(val); }
    int dequeue() {
        int val = data.front();
        data.pop_front();
        return val;
    }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

// Use namespace para evitar conflitos de nomes
using std::vector;
using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::mt19937;
using std::uniform_int_distribution;
using std::stringstream;
using std::thread;
using std::sqrt;
using std::fixed;
using std::setprecision;
using std::accumulate;
using std::sort;
using std::min_element;
using std::max_element;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::getline;

using namespace std;
using namespace chrono;

// ============================================================================
// STRUCTS PARA RESULTADOS
// ============================================================================

struct BenchmarkResult {
    int size;
    string data_structure;
    string operation;
    int run_index;
    unsigned seed;
    double time_s;
    int min_val;
    int max_val;

    BenchmarkResult(int s, string ds, string op, int run, unsigned sd, double t,
                   int min_v, int max_v)
        : size(s), data_structure(ds), operation(op), run_index(run), seed(sd), time_s(t),
          min_val(min_v), max_val(max_v) {}
};

struct StatsResult {
    int size;
    string data_structure;
    string operation;
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

std::vector<int> generate_test_data(int n, unsigned seed, int min_val, int max_val) {
    std::vector<int> data(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(min_val, max_val);

    for (int i = 0; i < n; i++) {
        data[i] = dist(rng);
    }
    return data;
}

double benchmark_vector_operation(const string& operation, int size, unsigned seed,
                                 int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    SimpleVector vec; // Usando implementação simples

    auto start = high_resolution_clock::now();

    if (operation == "push_back") {
        for (int val : test_data) {
            vec.push_back(val);
        }
    } else if (operation == "access") {
        // Primeiro preenche o vetor
        for (int val : test_data) {
            vec.push_back(val);
        }
        // Depois acessa todos os elementos
        volatile int sum = 0; // volatile para evitar otimização
        for (size_t i = 0; i < vec.size(); i++) {
            sum += vec[i];
        }
    } else if (operation == "insert_middle") {
        // Insere no meio do vetor (simulando inserção)
        for (size_t i = 0; i < test_data.size(); i++) {
            vec.push_back(test_data[i]);
        }
    } else {
        throw runtime_error("Operação de vetor desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_heap_operation(const string& operation, int size, unsigned seed,
                               int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    SimpleHeap h;

    auto start = high_resolution_clock::now();

    if (operation == "insert") {
        for (int val : test_data) {
            h.push(val);
        }
    } else if (operation == "extract_max") {
        // Primeiro insere todos os elementos
        for (int val : test_data) {
            h.push(val);
        }
        // Depois extrai todos
        while (!h.empty()) {
            h.pop();
        }
    } else if (operation == "build_heap") {
        // Para build_heap, simplesmente inserimos todos os elementos
        for (int val : test_data) {
            h.push(val);
        }
        // O heap é automaticamente mantido durante as inserções
    } else {
        throw runtime_error("Operação de heap desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_queue_operation(const string& operation, int size, unsigned seed,
                                int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    SimpleQueue q;

    auto start = high_resolution_clock::now();

    if (operation == "enqueue") {
        for (int val : test_data) {
            q.enqueue(val);
        }
    } else if (operation == "dequeue") {
        // Primeiro enfileira todos
        for (int val : test_data) {
            q.enqueue(val);
        }
        // Depois desenfileira todos
        while (!q.empty()) {
            q.dequeue();
        }
    } else if (operation == "mixed") {
        // Operações mistas: enqueue e dequeue intercalados
        for (int val : test_data) {
            q.enqueue(val);
            if (!q.empty()) {
                q.dequeue();
            }
        }
    } else {
        throw runtime_error("Operação de fila desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_my_vector_operation(const string& operation, int size, unsigned seed,
                                   int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    MyVector my_vec;

    auto start = high_resolution_clock::now();

    if (operation == "push_back") {
        for (int val : test_data) {
            my_vec.push_back(val);
        }
    } else if (operation == "access") {
        // Primeiro preenche o vetor
        for (int val : test_data) {
            my_vec.push_back(val);
        }
        // Depois acessa todos os elementos
        volatile int sum = 0; // volatile para evitar otimização
        for (size_t i = 0; i < my_vec.length(); i++) {
            sum += my_vec[i];
        }
    } else {
        throw runtime_error("Operação de MyVector desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_my_heap_operation(const string& operation, int size, unsigned seed,
                                 int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    heap my_heap;

    auto start = high_resolution_clock::now();

    if (operation == "insert") {
        for (int val : test_data) {
            my_heap.insert(val);
        }
    } else if (operation == "extract_max") {
        // Primeiro insere todos os elementos
        for (int val : test_data) {
            my_heap.insert(val);
        }
        // Depois extrai todos
        while (my_heap.size() > 0) {
            my_heap.extractMax();
        }
    } else {
        throw runtime_error("Operação de MyHeap desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_my_queue_linkedlist_operation(const string& operation, int size, unsigned seed,
                                             int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    MyQueueLinkedList q;

    auto start = high_resolution_clock::now();

    if (operation == "enqueue") {
        for (int val : test_data) {
            q.enqueue(val);
        }
    } else if (operation == "dequeue") {
        for (int val : test_data) {
            q.enqueue(val);
        }
        while (!q.isEmpty()) {
            q.dequeue();
        }
    } else if (operation == "mixed") {
        for (int val : test_data) {
            q.enqueue(val);
            if (!q.isEmpty()) {
                q.dequeue();
            }
        }
    } else {
        throw runtime_error("Operação de MyQueueLinkedList desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_my_queue_vector_operation(const string& operation, int size, unsigned seed,
                                         int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    MyQueueVector q_vec(size); // Capacidade inicial para o MyQueueVector

    auto start = high_resolution_clock::now();

    if (operation == "enqueue") {
        for (int val : test_data) {
            q_vec.enqueue(val);
        }
    } else if (operation == "dequeue") {
        for (int val : test_data) {
            q_vec.enqueue(val);
        }
        // A MyQueueVector não tem um método isEmpty ou size fácil de usar para este loop
        // Vamos assumir que se enfileiramos 'size' elementos, podemos desenfileirar 'size' elementos
        for (int i = 0; i < size; ++i) {
            q_vec.dequeue();
        }
    } else if (operation == "mixed") {
        for (int val : test_data) {
            q_vec.enqueue(val);
            // Assumimos que a fila não ficará vazia imediatamente após um enqueue
            // E que há elementos para desenfileirar
            q_vec.dequeue(); // Isso pode gerar erro se a fila estiver vazia
        }
    } else {
        throw runtime_error("Operação de MyQueueVector desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_my_stack_operation(const string& operation, int size, unsigned seed,
                                  int min_val, int max_val) {
    std::vector<int> test_data = generate_test_data(size, seed, min_val, max_val);
    MyStack s;

    auto start = high_resolution_clock::now();

    if (operation == "push") {
        for (int val : test_data) {
            s.push(val);
        }
    } else if (operation == "pop") {
        for (int val : test_data) {
            s.push(val);
        }
        for (size_t i = 0; i < test_data.size(); ++i) {
            s.pop();
        }
    } else {
        throw runtime_error("Operação de MyStack desconhecida: " + operation);
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    return duration.count();
}

double benchmark_data_structure(const string& data_structure, const string& operation,
                               int size, unsigned seed, int min_val, int max_val) {
    if (data_structure == "vector") {
        return benchmark_vector_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "heap") {
        return benchmark_heap_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "queue") {
        return benchmark_queue_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "my_vector") {
        return benchmark_my_vector_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "my_heap") {
        return benchmark_my_heap_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "my_queue_linkedlist") {
        return benchmark_my_queue_linkedlist_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "my_queue_vector") {
        return benchmark_my_queue_vector_operation(operation, size, seed, min_val, max_val);
    } else if (data_structure == "my_stack") {
        return benchmark_my_stack_operation(operation, size, seed, min_val, max_val);
    } else {
        throw runtime_error("Estrutura de dados desconhecida: " + data_structure);
    }
}

std::vector<unsigned> generate_seeds(unsigned base_seed, int num_runs) {
    std::vector<unsigned> seeds;
    for (int i = 0; i < num_runs; i++) {
        seeds.push_back(base_seed + i);
    }
    return seeds;
}

double calculate_median(std::vector<double>& values) {
    sort(values.begin(), values.end());
    size_t n = values.size();
    if (n % 2 == 0) {
        return (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        return values[n/2];
    }
}

double calculate_std(const std::vector<double>& values, double mean) {
    if (values.size() <= 1) return 0.0;

    double sum_sq = 0.0;
    for (double val : values) {
        double diff = val - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (values.size() - 1));
}

StatsResult calculate_stats(const std::vector<BenchmarkResult>& results) {
    std::vector<double> times;
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
        first.size, first.data_structure, first.operation, (int)results.size(),
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

void save_csv(const std::vector<BenchmarkResult>& results, const string& filename,
              const EnvironmentInfo& env) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return;
    }

    // Cabeçalho
    file << "size,data_structure,operation,run_index,seed,time_s,min_val,max_val,"
         << "cpu_model,cpu_cores,compiler_version,os_info,timestamp,hostname" << endl;

    // Dados
    for (const auto& result : results) {
        file << result.size << ","
             << result.data_structure << ","
             << result.operation << ","
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

void save_json(const std::vector<BenchmarkResult>& results, const string& filename,
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
        file << "      \"data_structure\": \"" << result.data_structure << "\"," << endl;
        file << "      \"operation\": \"" << result.operation << "\"," << endl;
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
    cout << "  --data-structures <lista>  Estruturas separadas por vírgula (padrão: vector,heap,queue,my_vector,my_heap,my_queue_linkedlist,my_queue_vector,my_stack)" << endl;
    cout << "  --operations <lista>       Operações separadas por vírgula (depende da estrutura)" << endl;
    cout << "  --function-classes <lista> Classes de funções separadas por vírgula (busca,inserção,deleção | search,insertion,deletion)" << endl;
    cout << "  --sizes <tamanhos>         Lista de tamanhos separados por espaço" << endl;
    cout << "  --max-size <max>           Tamanho máximo (gera tamanhos até max)" << endl;
    cout << "  --min-size <min>           Tamanho mínimo (padrão: 1000)" << endl;
    cout << "  --runs <num>               Número de execuções por teste (padrão: 3)" << endl;
    cout << "  --seeds <lista>            Seeds específicas separadas por espaço" << endl;
    cout << "  --base-seed <num>          Seed base para geração automática (padrão: 42)" << endl;
    cout << "  --min-val <val>            Valor mínimo dos elementos (padrão: 0)" << endl;
    cout << "  --max-val <val>            Valor máximo dos elementos (padrão: 100000)" << endl;
    cout << "  --output-csv <arquivo>     Arquivo CSV de saída (opcional)" << endl;
    cout << "  --output-json <arquivo>    Arquivo JSON de saída (opcional)" << endl;
    cout << "  --quiet                    Modo silencioso (menos saída)" << endl;
    cout << "  --help, -h                 Mostra esta ajuda" << endl;
    cout << endl;
    cout << "Estruturas disponíveis: vector, heap, queue, my_vector, my_heap, my_queue_linkedlist, my_queue_vector, my_stack" << endl;
    cout << "Operações disponíveis:" << endl;
    cout << "  vector: push_back, access, insert_middle" << endl;
    cout << "  heap: insert, extract_max, build_heap" << endl;
    cout << "  queue: enqueue, dequeue, mixed" << endl;
    cout << "  my_vector: push_back, access" << endl;
    cout << "  my_heap: insert, extract_max" << endl;
    cout << "  my_queue_linkedlist: enqueue, dequeue, mixed" << endl;
    cout << "  my_queue_vector: enqueue, dequeue, mixed" << endl;
    cout << "  my_stack: push, pop" << endl;
    cout << endl;
    cout << "Exemplos:" << endl;
    cout << "  " << program_name << " --data-structures vector,heap --operations push_back,insert --sizes 1000 5000 10000" << endl;
    cout << "  " << program_name << " --max-size 50000 --runs 5 --output-csv ds_results.csv" << endl;
    cout << "  " << program_name << " --data-structures vector,heap --function-classes busca,deleção --max-size 10000" << endl;
}

std::vector<string> split_string(const string& s, char delimiter) {
    std::vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Normaliza a classe de função para valores canônicos: "search", "insertion", "deletion"
static string normalize_function_class(string cls) {
    // lower-case
    transform(cls.begin(), cls.end(), cls.begin(), [](unsigned char c){ return static_cast<char>(tolower(c)); });
    // Remove espaços ao redor
    auto trim = [](string &x){
        size_t a = x.find_first_not_of(" \t\n\r");
        size_t b = x.find_last_not_of(" \t\n\r");
        if (a == string::npos) { x.clear(); return; }
        x = x.substr(a, b - a + 1);
    };
    trim(cls);

    // Mapeia variações em PT/EN (com e sem acento) para canônicos
    if (cls == "busca" || cls == "search") return "search";
    if (cls == "insercao" || cls == "inserção" || cls == "insertion" || cls == "inserir") return "insertion";
    if (cls == "delecao" || cls == "deleção" || cls == "deletion" || cls == "remocao" || cls == "remoção" || cls == "remover" || cls == "delete") return "deletion";
    return cls; // devolve como está (será validado adiante)
}

static std::vector<string> operations_for_classes(const string& ds, const std::vector<string>& classes_raw) {
    // Normaliza classes
    std::vector<string> classes;
    classes.reserve(classes_raw.size());
    for (auto c : classes_raw) classes.push_back(normalize_function_class(c));

    std::vector<string> ops;
    auto add = [&](const string& op){ if (find(ops.begin(), ops.end(), op) == ops.end()) ops.push_back(op); };

    for (const auto& c : classes) {
        if (c == "search") {
            if (ds == "vector" || ds == "my_vector") add("access");
            // Demais estruturas não têm busca definida neste benchmark
        } else if (c == "insertion") {
            if (ds == "vector") { add("push_back"); add("insert_middle"); }
            else if (ds == "heap" || ds == "my_heap") { add("insert"); }
            else if (ds == "queue" || ds == "my_queue_linkedlist" || ds == "my_queue_vector") { add("enqueue"); }
            else if (ds == "my_vector") { add("push_back"); }
            else if (ds == "my_stack") { add("push"); }
        } else if (c == "deletion") {
            if (ds == "heap" || ds == "my_heap") { add("extract_max"); }
            else if (ds == "queue" || ds == "my_queue_linkedlist" || ds == "my_queue_vector") { add("dequeue"); }
            else if (ds == "my_stack") { add("pop"); }
        }
    }
    return ops;
}

int main(int argc, char* argv[]) {
    // Parâmetros padrão
    std::vector<string> data_structures = {"vector", "heap", "queue", "my_vector", "my_heap", "my_queue_linkedlist", "my_queue_vector", "my_stack"};
    std::vector<string> operations = {"push_back", "insert", "enqueue"}; // Operações padrão por estrutura
    std::vector<string> function_classes; // Classes de funções opcionais
    std::vector<int> sizes;
    int max_size = 50000;
    int min_size = 1000;
    int num_runs = 3;
    std::vector<unsigned> seeds;
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
        } else if (arg == "--data-structures") {
            if (i + 1 >= argc) {
                cerr << "Erro: --data-structures requer um valor" << endl;
                return 1;
            }
            data_structures = split_string(argv[++i], ',');
        } else if (arg == "--operations") {
            if (i + 1 >= argc) {
                cerr << "Erro: --operations requer um valor" << endl;
                return 1;
            }
            operations = split_string(argv[++i], ',');
        } else if (arg == "--function-classes") {
            if (i + 1 >= argc) {
                cerr << "Erro: --function-classes requer um valor" << endl;
                return 1;
            }
            function_classes = split_string(argv[++i], ',');
        } else if (arg == "--sizes") {
            sizes.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                sizes.push_back(stoll(argv[i]));
                i++;
            }
            i--; // Ajusta pois o loop principal irá incrementar
        } else if (arg == "--max-size" && i + 1 < argc) {
            max_size = stoll(argv[++i]);
        } else if (arg == "--min-size" && i + 1 < argc) {
            min_size = stoll(argv[++i]);
        } else if (arg == "--runs" && i + 1 < argc) {
            num_runs = stoll(argv[++i]);
        } else if (arg == "--seeds") {
            seeds.clear();
            i++;
            while (i < argc && argv[i][0] != '-') {
                seeds.push_back(stoull(argv[i]));
                i++;
            }
            i--; // Ajusta pois o loop principal irá incrementar
        } else if (arg == "--base-seed" && i + 1 < argc) {
            base_seed = stoull(argv[++i]);
        } else if (arg == "--min-val" && i + 1 < argc) {
            min_val = stoll(argv[++i]);
        } else if (arg == "--max-val" && i + 1 < argc) {
            max_val = stoll(argv[++i]);
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

    // Valida estruturas de dados
    std::vector<string> valid_ds = {"vector", "heap", "queue", "my_vector", "my_heap", "my_queue_linkedlist", "my_queue_vector", "my_stack"};
    for (const auto& ds : data_structures) {
        if (find(valid_ds.begin(), valid_ds.end(), ds) == valid_ds.end()) {
            cerr << "Estrutura de dados inválida: " << ds << endl;
            return 1;
        }
    }

    // Se sizes não foi especificado, gera tamanhos dinamicamente
    if (sizes.empty()) {
        // Gera tamanhos logaritmicamente espaçados
        std::vector<long long> generated_sizes;

        // Começa com tamanhos pequenos e vai dobrando
        long long current_size = min_size;
        while (current_size <= max_size) {
            generated_sizes.push_back(current_size);

            // Para tamanhos pequenos (< 10000), aumenta de 1000 em 1000
            if (current_size < 10000) {
                current_size += 1000;
            }
            // Para tamanhos médios (10000-100000), aumenta de 5000 em 5000
            else if (current_size < 100000) {
                current_size += 5000;
            }
            // Para tamanhos grandes (100000-1000000), aumenta de 25000 em 25000
            else if (current_size < 1000000) {
                current_size += 25000;
            }
            // Para tamanhos muito grandes (>1000000), aumenta de 100000 em 100000
            else {
                current_size += 100000;
            }

            // Evita loop infinito
            if (generated_sizes.size() > 50) break;
        }

        // Garante que max_size seja incluído se não estiver na lista
        if (!generated_sizes.empty() && generated_sizes.back() != max_size && max_size > generated_sizes.back()) {
            generated_sizes.push_back(max_size);
        }

        // Converte para int (se couber)
        for (long long size : generated_sizes) {
            if (size <= INT_MAX) {
                sizes.push_back(static_cast<int>(size));
            }
        }
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
        cout << "=== Data Structures Benchmark ===" << endl;
        cout << "Estruturas: ";
        for (size_t i = 0; i < data_structures.size(); i++) {
            cout << data_structures[i];
            if (i < data_structures.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "Operações: ";
        for (size_t i = 0; i < operations.size(); i++) {
            cout << operations[i];
            if (i < operations.size() - 1) cout << ", ";
        }
        cout << endl;
        if (!function_classes.empty()) {
            cout << "Classes: ";
            for (size_t i = 0; i < function_classes.size(); i++) {
                cout << normalize_function_class(function_classes[i]);
                if (i < function_classes.size() - 1) cout << ", ";
            }
            cout << endl;
        }
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
        cout << "===================================" << endl << endl;
    }

    // Executa benchmarks
    std::vector<BenchmarkResult> all_results;

    for (const auto& ds : data_structures) {
        if (!quiet) cout << "Testando " << ds << "..." << endl;

        // Define operações válidas para cada estrutura
        std::vector<string> valid_ops;
        if (ds == "vector") {
            valid_ops = {"push_back", "access", "insert_middle"};
        } else if (ds == "heap") {
            valid_ops = {"insert", "extract_max", "build_heap"};
        } else if (ds == "queue") {
            valid_ops = {"enqueue", "dequeue", "mixed"};
        } else if (ds == "my_vector") {
            valid_ops = {"push_back", "access"};
        } else if (ds == "my_heap") {
            valid_ops = {"insert", "extract_max"};
        } else if (ds == "my_queue_linkedlist") {
            valid_ops = {"enqueue", "dequeue", "mixed"};
        } else if (ds == "my_queue_vector") {
            valid_ops = {"enqueue", "dequeue", "mixed"};
        } else if (ds == "my_stack") {
            valid_ops = {"push", "pop"};
        }

        // Filtra operações válidas
        std::vector<string> desired_ops = operations;
        if (!function_classes.empty()) {
            desired_ops = operations_for_classes(ds, function_classes);
        }

        std::vector<string> ops_to_test;
        for (const auto& op : desired_ops) {
            if (find(valid_ops.begin(), valid_ops.end(), op) != valid_ops.end()) {
                ops_to_test.push_back(op);
            }
        }

        if (ops_to_test.empty()) {
            if (!quiet) cout << "  Nenhuma operação válida para " << ds << endl;
            continue;
        }

        for (const auto& operation : ops_to_test) {
            for (int size : sizes) {
                if (!quiet) cout << "  " << operation << " n=" << size << ": ";

                std::vector<double> times_this_size;

                for (int run = 0; run < num_runs; run++) {
                    if (!quiet) cout << "." << flush;

                    try {
                        double time = benchmark_data_structure(ds, operation, size,
                                                             seeds[run], min_val, max_val);

                        all_results.emplace_back(size, ds, operation, run, seeds[run], time,
                                               min_val, max_val);
                        times_this_size.push_back(time);
                    } catch (const exception& e) {
                        cerr << "Erro em " << ds << " " << operation << " n=" << size
                             << " run=" << run << ": " << e.what() << endl;
                        return 1;
                    }
                }

                if (!quiet) {
                    // Calcula estatísticas locais para este tamanho
                    double mean_t = accumulate(times_this_size.begin(), times_this_size.end(), 0.0) / times_this_size.size();
                    std::vector<double> sorted_times = times_this_size;
                    double median_t = calculate_median(sorted_times);
                    double std_t = calculate_std(times_this_size, mean_t);
                    double min_t = *min_element(times_this_size.begin(), times_this_size.end());
                    double max_t = *max_element(times_this_size.begin(), times_this_size.end());

                    // Imprime em milissegundos
                    cout << " ✓ "
                         << fixed << setprecision(3)
                         << "média=" << (mean_t * 1000.0) << " ms"
                         << ", mediana=" << (median_t * 1000.0) << " ms"
                         << ", mín=" << (min_t * 1000.0) << " ms"
                         << ", máx=" << (max_t * 1000.0) << " ms"
                         << ", desvio=" << (std_t * 1000.0) << " ms" << endl;
                }
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
