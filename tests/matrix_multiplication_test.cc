#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "../algorithms/cpp/luAlgorithms.h"

using namespace std;

static Matrix generate_random_square_matrix(int n, int min_val = -10, int max_val = 10, unsigned seed = 42) {
    // Gera matriz quadrada n x n
    Matrix M(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(min_val, max_val);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] = dist(rng);
        }
    }
    return M;
}

static Matrix matmul_reference(const Matrix& A, const Matrix& B) {
    int n = (int)A.size();
    int m = (int)A[0].size();
    int p = (int)B[0].size();
    // Nos testes quadrados, p == n
    Matrix C(n);
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            long long aik = A[i][k];
            for (int j = 0; j < p; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
    return C;
}

static void expect_equal_matrix(const Matrix& X, const Matrix& Y) {
    ASSERT_EQ(X.size(), Y.size()) << "Linhas diferentes";
    if (X.empty()) return;
    ASSERT_EQ(X[0].size(), Y[0].size()) << "Colunas diferentes";
    for (size_t i = 0; i < X.size(); i++) {
        for (size_t j = 0; j < X[0].size(); j++) {
            ASSERT_EQ(X[i][j], Y[i][j]) << "Diferença em (" << i << "," << j << ")";
        }
    }
}

TEST(MatrixMul, NaiveSmallCases) {
    // 1) casos triviais
    Matrix A0(0), B0(0); // 0x0
    EXPECT_THROW(matmul_naive(A0, B0), std::invalid_argument);

    Matrix A1 = {{1}}; Matrix B1 = {{2}};
    expect_equal_matrix(matmul_naive(A1, B1), Matrix{{2}});

    Matrix A2 = {{1,2},{3,4}}; Matrix B2 = {{5,6},{7,8}};
    expect_equal_matrix(matmul_naive(A2, B2), Matrix{{19,22},{43,50}});
}

TEST(MatrixMul, NaiveRandomSquarePowersOfTwo) {
    vector<int> ns = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    for (int n : ns) {
        Matrix A = generate_random_square_matrix(n, -5, 5, 123 + n);
        Matrix B = generate_random_square_matrix(n, -5, 5, 321 + n);
        Matrix R = matmul_reference(A, B);
        Matrix C(n);
        ASSERT_NO_THROW(C = matmul_naive(A, B));
        expect_equal_matrix(C, R);
    }
}

// Ímpares serão tratados futuramente (quando implementação suportar n ímpar)
TEST(DISABLED_MatrixMul, NaiveRandomSquareOddNs) {
    vector<int> ns = {1,3,5,7};
    for (int n : ns) {
        Matrix A = generate_random_square_matrix(n, -5, 5, 123 + n);
        Matrix B = generate_random_square_matrix(n, -5, 5, 321 + n);
        Matrix R = matmul_reference(A, B);
        Matrix C = matmul_naive(A, B);
        expect_equal_matrix(C, R);
    }
}

TEST(MatrixMul, NaiveIncompatibleDims) {
    Matrix A = {{1,2,3}}; // 1x3
    Matrix B = {{4,5}};   // 1x2 (incompatível)
    EXPECT_THROW(matmul_naive(A, B), std::invalid_argument);
}

// Strassen: começar desabilitado até implementação
TEST(DISABLED_MatrixMul, StrassenMatchesNaiveOnPowersOfTwo) {
    vector<int> ns = {1, 2, 4, 8, 16, 32};
    for (int n : ns) {
        Matrix A = generate_random_square_matrix(n, -5, 5, 42 + n);
        Matrix B = generate_random_square_matrix(n, -5, 5, 24 + n);
        Matrix R = matmul_naive(A, B);
        Matrix S = matmul_strassen(A, B);
        expect_equal_matrix(S, R);
    }
}

TEST(MatrixMul, NaiveRejectsNonSquare) {
    // Matrizes retangulares (não quadradas)
    Matrix A = {{1,2,3},{4,5,6}}; // 2x3
    Matrix B = {{1,2},{3,4},{5,6}}; // 3x2
    EXPECT_THROW(matmul_naive(A, B), std::invalid_argument);
}


