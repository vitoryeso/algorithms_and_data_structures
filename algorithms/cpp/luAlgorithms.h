#ifndef _LUALGORITHMS_H_
#define _LUALGORITHMS_H_

#include <string>
#include <vector>
#include <initializer_list>

using namespace std;

// Classe Matrix para multiplicação de matrizes
class Matrix {
public:
    vector<vector<long long>> data;
    
    // Construtor para matriz quadrada n×n
    Matrix(int n) : data(n, vector<long long>(n, 0)) {}
    
    // Construtor para matriz com dimensões específicas (para compatibilidade)
    Matrix(size_t rows, const vector<long long>& row) : data(rows, row) {}
    
    // Construtor a partir de iteradores (para compatibilidade com código existente)
    template<typename Iter>
    Matrix(Iter begin, Iter end) : data(begin, end) {}
    
    // Construtor com lista de inicialização
    Matrix(initializer_list<initializer_list<long long>> init) {
        data.reserve(init.size());
        for (const auto& row : init) {
            data.emplace_back(row);
        }
    }
    
    // Operadores para acesso aos elementos
    vector<long long>& operator[](size_t i) { return data[i]; }
    const vector<long long>& operator[](size_t i) const { return data[i]; }
    
    // Métodos para compatibilidade com vector
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
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

#endif
