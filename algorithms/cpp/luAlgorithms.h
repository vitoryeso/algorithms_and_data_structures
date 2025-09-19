#ifndef _LUALGORITHMS_H_
#define _LUALGORITHMS_H_

#include <string>
#include <vector>
#include <initializer_list>

using namespace std;

// Classe Matrix para multiplicação de matrizes com armazenamento contíguo
class Matrix {
public:
    std::vector<long long> data;
    size_t rows, cols;

    // Construtor para matriz quadrada n×n
    Matrix(int n) : data(n * n, 0), rows(n), cols(n) {}

    // Construtor para matriz com dimensões específicas
    Matrix(size_t r, size_t c) : data(r * c, 0), rows(r), cols(c) {}

    // Construtor a partir de vector<vector> (para compatibilidade)
    template<typename T>
    Matrix(const std::vector<std::vector<T>>& vec) : rows(0), cols(0) {
        if (!vec.empty()) {
            rows = vec.size();
            cols = vec[0].size();
            data.reserve(rows * cols);
            for (const auto& row : vec) {
                data.insert(data.end(), row.begin(), row.end());
            }
        }
    }

    // Construtor com lista de inicialização
    Matrix(std::initializer_list<std::initializer_list<long long>> init) {
        rows = init.size();
        if (rows > 0) {
            cols = init.begin()->size();
            data.reserve(rows * cols);
            for (const auto& row : init) {
                data.insert(data.end(), row.begin(), row.end());
            }
        }
    }

    // Operadores para acesso aos elementos
    long long& operator()(size_t r, size_t c) {
        return data[r * cols + c];
    }

    const long long& operator()(size_t r, size_t c) const {
        return data[r * cols + c];
    }

    // Métodos para compatibilidade com vector (LEGACY - serão removidos)
    struct RowProxy {
        Matrix* matrix;
        size_t row;
        long long& operator[](size_t col) { return (*matrix)(row, col); }
        const long long& operator[](size_t col) const { return (*matrix)(row, col); }
        size_t size() const { return matrix->cols; }
    };

    RowProxy operator[](size_t r) {
        return RowProxy{this, r};
    }

    const RowProxy operator[](size_t r) const {
        return RowProxy{const_cast<Matrix*>(this), r};
    }

    // Métodos para compatibilidade com vector
    size_t size() const { return rows; }
    bool empty() const { return data.empty(); }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    // Novo método para obter ponteiro para dados
    long long* data_ptr() { return data.data(); }
    const long long* data_ptr() const { return data.data(); }
};

void selection_sort(vector<int>& V);

void insertion_sort(vector<int>& V);

void merge(vector<int>& V, int p, int q, int r);
void merge_sort(vector<int>& V, int p, int r);
void merge_sort(vector<int>& V);

void quick_sort(vector<int>& V, const string pivot_choice);
void quick_sort(vector<int>& V);

void quick_sort(vector<int>& V, int p, int r);
void randomized_quick_sort(vector<int>& V, const unsigned p, const unsigned r);
void median3_quick_sort(vector<int>& V, const unsigned p, const unsigned r);

int partition(vector<int>& V, int p, int r);
const unsigned randomized_partition(vector<int>& V, const unsigned p, const unsigned r);
const unsigned median3_partition(vector<int>& V, const unsigned p, const unsigned r);

void counting_sort(vector<int>& V, const int k);
void counting_sort(vector<int>& V);
vector<int> counting_sort_pairs(vector<pair<int, int>>& V, const int k);

void bucket_sort(vector<int>& V, const int k, const unsigned n_buckets);
void bucket_sort(vector<int>& V);

vector<pair<int, int>> get_pairs(vector<int>& V, const unsigned base, const unsigned d);
void radix_sort(vector<int>& V, const int base, const unsigned d);
void radix_sort(vector<int>& V);

const int binary_search(vector<int>& V, const int value);
const int binary_search(vector<int>& V, const unsigned p, const unsigned r, const int value);

// Multiplicação de matrizes (convencional e Strassen)
Matrix matmul_naive(const Matrix& A, const Matrix& B);
Matrix matmul_strassen(const Matrix& A, const Matrix& B, const unsigned cutoff = 64);

// NOVAS FUNÇÕES OTIMIZADAS
Matrix matmul_blocked(const Matrix& A, const Matrix& B, size_t block_size = 64);
Matrix matmul_blocked_optimized(const Matrix& A, const Matrix& B, size_t block_size = 1000);
void matmul_blocked_inplace(const Matrix& A, const Matrix& B, Matrix& C, size_t block_size = 64);

#endif
