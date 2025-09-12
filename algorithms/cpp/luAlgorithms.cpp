#include <iostream>
#include <math.h>
#include <random>
#include <string>
#include <vector>
#include <stdexcept>
#include "luAlgorithms.h"

using namespace std;

void selection_sort(vector<int>& V){
    if(V.size() <= 1) return; // Array vazio ou com 1 elemento já está ordenado
    
    for(unsigned i=0; i<V.size() - 1; i++) {
        unsigned min = i;
        for(unsigned j=i + 1; j<V.size(); j++) {
            if(V[j] < V[min]) { 
                min = j;
            }
        }
        swap(V[i], V[min]);
    }
}

void insertion_sort(vector<int>& V){
    for(unsigned i=1; i<V.size(); i++) {
        int key, j;
        key = V[i];
        j = i - 1;
        while(V[j] > key && j>=0) {
            V[j + 1] = V[j];
            j--;
        }
        if(key != V[j + 1]) V[j + 1] = key;
    }
}

void quick_sort(vector<int>& V) {
  quick_sort(V, "last");
}

void merge(vector<int>& V, int p,int q,int r) {
  int n1 = q - p + 1;
  int n2 = r - q;
  int V1[n1], V2[n2];
  for(int i=0; i<n1; i++)
    V1[i] = V[p + i];

  for(int i=0; i<n2; i++)
    V2[i] = V[i + q + 1];

  int i = 0, j = 0;
  int v_idx = p;

  while(i<n1 && j<n2){
      if(V1[i] <= V2[j]) {
          V[v_idx] = V1[i];
          i++;
      }
      else {
          V[v_idx] = V2[j];
          j++;
      }
      v_idx++;
  }
  if(n1 == i) {
      for(int j2=j; j2<n2; j2++) {
          V[v_idx] = V2[j2];
          v_idx++;
      }
  }
  else {
      for(int i2=i; i2<n1; i2++) {
          V[v_idx] = V1[i2];
          v_idx++;
      }
  }
}

void merge_sort(vector<int>& V, int p, int r) {
  cout << "inside merge sort with parameters." << endl;
  if(p>=r){
    return;//returns recursively
  }

  int m = (p+r-1)/2;
  cout << "m: " << m << endl;
  merge_sort(V ,p,m);
  merge_sort(V ,m+1,r);
  merge(V ,p,m,r);
}

void merge_sort(vector<int>& V) {
  if(V.size() <= 1) return; // Array vazio ou com 1 elemento já está ordenado
  cout << "inside merge sort. V.size() - 1 = " << V.size() - 1 << endl;
  merge_sort(V, 0, V.size() - 1);
}

void quick_sort(vector<int>& V, const string pivot_choice) {
        if(V.size() <= 1) return; // Array vazio ou com 1 elemento já está ordenado
        
        if(pivot_choice == "random") randomized_quick_sort(V, 0, V.size() - 1);
        else if(pivot_choice == "median3") median3_quick_sort(V, 0, V.size() - 1);
        else if(pivot_choice == "last") quick_sort(V, 0, V.size() - 1);
        else randomized_quick_sort(V, 0, V.size() - 1);
}

void quick_sort(vector<int>& V, int p, int r) {
    if(r > p) {
        int q = partition(V, p, r);
        quick_sort(V, p, q - 1);
        quick_sort(V, q + 1, r);
    }
}

int partition(vector<int>& V, int p, int r) {
    // pivot = V[r]
    int i = p - 1;
    for(int j=p; j<r; j++) {
        if(V[j] < V[r]) {
            i++;
            swap(V[i], V[j]);
        }
    }
    i++;
    swap(V[i], V[r]);
    return i;
}

void randomized_quick_sort(vector<int>& V, const unsigned p, const unsigned r) {
    if(r > p) {
        unsigned q = randomized_partition(V, p, r);
        randomized_quick_sort(V, p, q - 1);
        randomized_quick_sort(V, q + 1, r);
    }
}

const unsigned randomized_partition(vector<int>& V, const unsigned p, const unsigned r) {
    if(p >= r) return p; // Caso base para arrays pequenos
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(p, r); 
   
    // swap between a random position and the last element
    swap(V[dist(rng)], V[r]);
    return partition(V, p, r);
}

void median3_quick_sort(vector<int>& V, const unsigned p, const unsigned r) {
    if(r > p) {
        unsigned q = median3_partition(V, p, r);
        median3_quick_sort(V, p, q - 1);
        median3_quick_sort(V, q + 1, r);
    }
}

const unsigned median3_partition(vector<int>& V, const unsigned p, const unsigned r) {
    if(p >= r) return p; // Caso base para arrays pequenos
    
    int provId = p + (r - p)/2;
    if(V[p] > V[provId]) { 
        if(V[p] < V[r]) swap(V[p], V[r]);
        else if(V[provId] > V[r]) swap(V[provId], V[r]);
    }
    return partition(V, p, r);
}
           
const int binary_search(vector<int>& V, const int value) {
    return binary_search(V, 0, V.size(), value); 
}

const int binary_search(vector<int>& V, const unsigned p, const unsigned r, const int value) {
  if(r-p == 0) {
    if(V[p] == value) return p;
    else return -1;
  }
  if(r-p < 0) {
    return -1;
  }
  else {
    int q = floor((r - p)/2) + p;
    if(V[q] == value) return q;
    else if(V[q] > value){ 
      return binary_search(V, p, q-1, value);
    }
    else if(V[q] < value) {
      return binary_search(V, q+1, r, value);
    }
    return -1;
  }
}

void counting_sort(vector<int>& V, const int k) {
    vector<int> prov(k + 1, 0), prov2(V);
    for(unsigned i=0; i<V.size(); i++) prov[V.at(i)]++;
    for(unsigned i=1; i<(unsigned)k + 1; i++) prov[i] += prov[i - 1];
    for(int i=V.size() - 1; i>=0; i--) {
        V[prov.at(prov2.at(i)) - 1] = prov2[i];
        prov[prov2.at(i)]--;
    }
}

void counting_sort(vector<int>& V) {
     cerr << "digite o valor k(valor maximo pros elementos do vetor): ";
     int k;
     cin >> k;
     counting_sort(V, k);
}

void bucket_sort(vector<int>& V, const int k, const unsigned n_buckets) {
	vector<vector<int>> buckets(n_buckets);
	unsigned j;
	float prov( (float)k / n_buckets);
	for(unsigned i=0; i<V.size(); i++) {
		j = floor(V[i] / prov);
		buckets[j].push_back(V[i]);
	}
	for(unsigned i=0; i<n_buckets; i++) counting_sort(buckets[i], ceil(prov * (i + 1)));
	j = 0;
	for(unsigned i=0; i<n_buckets; i++) {
		for(unsigned n=0; n<buckets[i].size(); n++) {
			V[j] = buckets[i].at(n);
			j++;
		}
	}
}

void bucket_sort(vector<int>& V) {
    cerr << "digite o valor k(valor maximo pros elementos do vetor): ";
    int k;
    cin >> k;
    cerr << "digite o numero de buckets(baldes): ";
    unsigned n_buckets;
    cin >> n_buckets; 
    if(n_buckets >= (unsigned)k) counting_sort(V, k);
    else bucket_sort(V, k, n_buckets);
}

vector<int> counting_sort_pairs(vector<pair<int, int>>& V, const int k) {
    vector<int> prov1(k + 1, 0), prov3(V.size()); 
    vector<pair<int, int>> prov2(V);
    for(unsigned i=0; i<V.size(); i++) prov1[prov2[i].first]++;
    for(unsigned i=1; i<(unsigned)k + 1; i++) prov1[i] += prov1[i - 1]; 
    for(int i=V.size() - 1; i>=0; i--) {
        prov3[prov1.at(prov2.at(i).first) - 1] = prov2[i].second;
        prov1[prov2.at(i).first]--; 
    }
    return prov3;
}

vector<pair<int, int>> get_pairs(vector<int>& V, const unsigned base,  const unsigned d) {
    vector<pair<int, int>> prov;
    unsigned digit;
    for(unsigned i=0; i<V.size(); i++) {
        digit = (long int) (V[i] / pow(base, d)) % base; 
        prov.push_back(make_pair(digit, V[i]));
    }
    return prov; 
} 

void radix_sort(vector<int>& V, const int base, const unsigned d) {
    vector<pair<int, int>> prov1;
    vector<int> prov2(V);
    for(unsigned i=0; i<d; i++) {
        prov1 = get_pairs(prov2, base, i);
        //some stable method
        prov2 = counting_sort_pairs(prov1, base);
    } 
    V = prov2;
}

void radix_sort(vector<int>& V) {
     cerr << "digite a base dos inteiros do arranjo(ex: 10): ";
     int base;
     cin >> base;
     cerr << "digite a quantidade maxima de digitos dos numeros: ";
     unsigned d;
     cin >> d;
     radix_sort(V, base, d);
}


static void assert_square_same_dim_or_throw(const Matrix& A, const Matrix& B) {
    if (A.empty() || B.empty()) throw invalid_argument("Matrizes vazias nao sao suportadas");
    size_t a_rows = A.size();
    size_t a_cols = A[0].size();
    size_t b_rows = B.size();
    size_t b_cols = B[0].size();
    for (size_t i = 0; i < a_rows; i++) if (A[i].size() != a_cols) throw invalid_argument("Matriz A irregular");
    for (size_t i = 0; i < b_rows; i++) if (B[i].size() != b_cols) throw invalid_argument("Matriz B irregular");
    if (a_rows != a_cols) throw invalid_argument("A deve ser quadrada");
    if (b_rows != b_cols) throw invalid_argument("B deve ser quadrada");
    if (a_rows != b_rows) throw invalid_argument("Dimensoes diferentes para matrizes quadradas");
}

Matrix matmul_naive(const Matrix& A, const Matrix& B) {
    assert_square_same_dim_or_throw(A, B);
    // if size is 1x1, multiply the matrices using ops
    if (A.size() == 1 && B.size() == 1) {
        return Matrix({{A[0][0] * B[0][0]}});
    }

    // if size is 2x2, multiply the matrices using ops
    if (A.size() == 2 && B.size() == 2) {
        // Definindo matriz de resultado aqui para evitar criação de vector temporário
        Matrix result(2, vector<long long>(2));
        
        // Calcular elementos diretamente sem operações intermediárias
        const long long a00 = A[0][0], a01 = A[0][1];
        const long long a10 = A[1][0], a11 = A[1][1];
        const long long b00 = B[0][0], b01 = B[0][1];
        const long long b10 = B[1][0], b11 = B[1][1];
        
        result[0][0] = a00 * b00 + a01 * b10;
        result[0][1] = a00 * b01 + a01 * b11;
        result[1][0] = a10 * b00 + a11 * b10;
        result[1][1] = a10 * b01 + a11 * b11;
        
        return result;
    }

    // defining the 4 submatrices of A and B
    size_t n = A.size();
    size_t half = n / 2;
    
    // Create submatrices by extracting quadrants
    Matrix A11(half), A12(half), A21(half), A22(half);
    Matrix B11(half), B12(half), B21(half), B22(half);
    
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < half; j++) {
            // Upper-left quadrant
            A11[i][j] = A[i][j];
            B11[i][j] = B[i][j];
            
            // Upper-right quadrant
            A12[i][j] = A[i][j + half];
            B12[i][j] = B[i][j + half];
            
            // Lower-left quadrant
            A21[i][j] = A[i + half][j];
            B21[i][j] = B[i + half][j];
            
            // Lower-right quadrant
            A22[i][j] = A[i + half][j + half];
            B22[i][j] = B[i + half][j + half];
        }
    }

    // C11 = A11*B11 + A12*B21
    // C12 = A11*B12 + A12*B22
    // C21 = A21*B11 + A22*B21
    // C22 = A21*B21 + A22*B22

    // Recursively multiply the submatrices
    Matrix C11_row = matmul_naive(A11, B11);
    Matrix C11_col = matmul_naive(A12, B21);

    Matrix C12_row = matmul_naive(A11, B12);
    Matrix C12_col = matmul_naive(A12, B22);

    Matrix C21_row = matmul_naive(A21, B11);
    Matrix C21_col = matmul_naive(A22, B21);

    Matrix C22_row = matmul_naive(A21, B12);
    Matrix C22_col = matmul_naive(A22, B22);

    // Define the submatrices of C
    Matrix C11(half), C12(half), C21(half), C22(half);
    
    // Add the products for the correct result (C = A*B in block form)
    // Add matrices element by element
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < half; j++) {
            C11[i][j] = C11_row[i][j] + C11_col[i][j];
            C12[i][j] = C12_row[i][j] + C12_col[i][j];
            C21[i][j] = C21_row[i][j] + C21_col[i][j];
            C22[i][j] = C22_row[i][j] + C22_col[i][j];
        }
    }
    
    // Assemble final result matrix
    Matrix C(n);
    for (size_t i = 0; i < half; i++) {
        for (size_t j = 0; j < half; j++) {
            C[i][j] = C11[i][j];                    // Upper-left
            C[i][j + half] = C12[i][j];             // Upper-right
            C[i + half][j] = C21[i][j];             // Lower-left
            C[i + half][j + half] = C22[i][j];      // Lower-right
        }
    }
    
    return C;
}

Matrix matmul_strassen(const Matrix& A, const Matrix& B, const unsigned cutoff) {
    (void)cutoff;
    assert_square_same_dim_or_throw(A, B);
    throw runtime_error("matmul_strassen not implemented");
}
