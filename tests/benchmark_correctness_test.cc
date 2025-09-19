#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "../algorithms/cpp/luAlgorithms.h"

using namespace std;

// ============================================================================
// UTILITÁRIOS PARA TESTES
// ============================================================================

static Matrix generate_random_square_matrix(int n, int min_val = -10, int max_val = 10, unsigned seed = 42) {
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
    Matrix C(n);
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            long long aik = A[i][k];
            for (int j = 0; j < n; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
    return C;
}

static void expect_equal_matrix(const Matrix& X, const Matrix& Y, double tolerance = 0.0) {
    ASSERT_EQ(X.size(), Y.size()) << "Linhas diferentes";
    if (X.empty()) return;
    ASSERT_EQ(X[0].size(), Y[0].size()) << "Colunas diferentes";
    for (size_t i = 0; i < X.size(); i++) {
        for (size_t j = 0; j < X[0].size(); j++) {
            if (tolerance > 0.0) {
                EXPECT_NEAR(X[i][j], Y[i][j], tolerance)
                    << "Diferença em (" << i << "," << j << ")";
            } else {
                ASSERT_EQ(X[i][j], Y[i][j])
                    << "Diferença em (" << i << "," << j << ")";
            }
        }
    }
}

// ============================================================================
// CLASSES DE TESTE PARAMÉTRICO
// ============================================================================

class MatrixMulCorrectnessTest : public ::testing::TestWithParam<tuple<int, unsigned>> {
protected:
    int n;
    unsigned seed;

    void SetUp() override {
        tie(n, seed) = GetParam();
    }
};

class SortingCorrectnessTest : public ::testing::TestWithParam<tuple<int, unsigned, string>> {
protected:
    int n;
    unsigned seed;
    string pattern;

    void SetUp() override {
        tie(n, seed, pattern) = GetParam();
    }

    vector<int> generate_test_vector() {
        vector<int> V(n);
        mt19937 rng(seed);
        uniform_int_distribution<int> dist(-1000, 1000);

        if (pattern == "ordered") {
            for (int i = 0; i < n; i++) V[i] = i;
        } else if (pattern == "reverse") {
            for (int i = 0; i < n; i++) V[i] = n - i - 1;
        } else if (pattern == "random") {
            for (int i = 0; i < n; i++) V[i] = dist(rng);
        } else if (pattern == "nearly_sorted") {
            for (int i = 0; i < n; i++) V[i] = i;
            // Swap some elements
            for (int i = 0; i < n/20; i++) {
                int idx1 = dist(rng) % n;
                int idx2 = dist(rng) % n;
                swap(V[idx1], V[idx2]);
            }
        }

        return V;
    }
};

// ============================================================================
// TESTES DE CORREÇÃO PARA MULTIPLICAÇÃO DE MATRIZES
// ============================================================================

TEST_P(MatrixMulCorrectnessTest, AllAlgorithmsProduceSameResult) {
    // Generate test matrices
    Matrix A = generate_random_square_matrix(n, -10, 10, seed);
    Matrix B = generate_random_square_matrix(n, -10, 10, seed + 1);

    // Compute reference result
    Matrix reference = matmul_reference(A, B);

    // Test naive algorithm
    Matrix naive_result = matmul_naive(A, B);
    expect_equal_matrix(naive_result, reference);

    // Test Strassen algorithm (if n is power of 2)
    if ((n & (n - 1)) == 0) {  // Check if power of 2
        Matrix strassen_result = matmul_strassen(A, B, 64);
        expect_equal_matrix(strassen_result, reference);
    }

    // Test blocked algorithms
    Matrix blocked_result = matmul_blocked(A, B, 64);
    expect_equal_matrix(blocked_result, reference);

    Matrix blocked_opt_result = matmul_blocked_optimized(A, B, 1000);
    expect_equal_matrix(blocked_opt_result, reference);
}

TEST_P(MatrixMulCorrectnessTest, StrassenVsNaiveSmallMatrices) {
    if (n > 64) return;  // Skip large matrices for this test

    Matrix A = generate_random_square_matrix(n, -5, 5, seed);
    Matrix B = generate_random_square_matrix(n, -5, 5, seed + 1);

    Matrix naive_result = matmul_naive(A, B);
    Matrix strassen_result = matmul_strassen(A, B, 32);

    expect_equal_matrix(strassen_result, naive_result);
}

TEST_P(MatrixMulCorrectnessTest, BlockedAlgorithmsVsNaive) {
    Matrix A = generate_random_square_matrix(n, -10, 10, seed);
    Matrix B = generate_random_square_matrix(n, -10, 10, seed + 1);

    Matrix naive_result = matmul_naive(A, B);

    // Test with different block sizes
    vector<size_t> block_sizes = {16, 32, 64, 128};
    for (size_t bs : block_sizes) {
        if (bs <= n) {
            Matrix blocked_result = matmul_blocked(A, B, bs);
            expect_equal_matrix(blocked_result, naive_result);

            Matrix blocked_opt_result = matmul_blocked_optimized(A, B, bs);
            expect_equal_matrix(blocked_opt_result, naive_result);
        }
    }
}

// ============================================================================
// TESTES DE CORREÇÃO PARA ALGORITMOS DE ORDENAÇÃO
// ============================================================================

TEST_P(SortingCorrectnessTest, AllSortingAlgorithmsProduceSameResult) {
    vector<int> original = generate_test_vector();
    vector<int> reference = original;
    sort(reference.begin(), reference.end());

    // Test selection sort
    {
        vector<int> test_vec = original;
        selection_sort(test_vec);
        ASSERT_EQ(test_vec, reference) << "Selection sort failed for pattern: " << pattern;
    }

    // Test insertion sort
    {
        vector<int> test_vec = original;
        insertion_sort(test_vec);
        ASSERT_EQ(test_vec, reference) << "Insertion sort failed for pattern: " << pattern;
    }

    // Test quick sort
    {
        vector<int> test_vec = original;
        quick_sort(test_vec);
        ASSERT_EQ(test_vec, reference) << "Quick sort failed for pattern: " << pattern;
    }

    // Test merge sort
    {
        vector<int> test_vec = original;
        merge_sort(test_vec);
        ASSERT_EQ(test_vec, reference) << "Merge sort failed for pattern: " << pattern;
    }
}

TEST_P(SortingCorrectnessTest, SortingPreservesSize) {
    vector<int> original = generate_test_vector();
    size_t original_size = original.size();

    // Test all sorting algorithms preserve size
    vector<string> algorithms = {"selection", "insertion", "quick", "merge"};

    for (const string& alg : algorithms) {
        vector<int> test_vec = original;

        if (alg == "selection") selection_sort(test_vec);
        else if (alg == "insertion") insertion_sort(test_vec);
        else if (alg == "quick") quick_sort(test_vec);
        else if (alg == "merge") merge_sort(test_vec);

        ASSERT_EQ(test_vec.size(), original_size)
            << alg << " sort changed vector size for pattern: " << pattern;
    }
}

TEST_P(SortingCorrectnessTest, SortingWithDuplicates) {
    vector<int> test_vec = generate_test_vector();

    // Add some duplicates
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, n-1);
    for (int i = 0; i < n/10; i++) {
        int idx = dist(rng);
        test_vec.push_back(test_vec[idx]);
    }

    vector<int> reference = test_vec;
    sort(reference.begin(), reference.end());

    // Test merge sort with duplicates (most stable)
    vector<int> merge_vec = test_vec;
    merge_sort(merge_vec);

    ASSERT_EQ(merge_vec, reference) << "Merge sort failed with duplicates";
}

// ============================================================================
// INSTANCIAÇÃO DOS TESTES PARAMÉTRICOS
// ============================================================================

// Tamanhos para testes de multiplicação de matrizes (potências de 2 pequenas)
INSTANTIATE_TEST_SUITE_P(
    SmallPowersOfTwo,
    MatrixMulCorrectnessTest,
    ::testing::Combine(
        ::testing::Values(2, 4, 8, 16, 32, 64),
        ::testing::Values(42, 123, 456, 789, 999)
    )
);

// Tamanhos para testes de ordenação
INSTANTIATE_TEST_SUITE_P(
    SmallToMediumSizes,
    SortingCorrectnessTest,
    ::testing::Combine(
        ::testing::Values(10, 50, 100, 500, 1000),
        ::testing::Values(42, 123, 456),
        ::testing::Values("ordered", "reverse", "random", "nearly_sorted")
    )
);

// ============================================================================
// TESTES ADICIONAIS DE SANIDADE
// ============================================================================

TEST(MatrixMulSanity, EdgeCases) {
    // Test 1x1 matrices
    Matrix A1 = {{5}};
    Matrix B1 = {{3}};
    Matrix C1 = matmul_naive(A1, B1);
    ASSERT_EQ(C1[0][0], 15);

    // Test 2x2 matrices
    Matrix A2 = {{1, 2}, {3, 4}};
    Matrix B2 = {{5, 6}, {7, 8}};
    Matrix C2 = matmul_naive(A2, B2);
    Matrix expected = {{19, 22}, {43, 50}};
    expect_equal_matrix(C2, expected);
}

TEST(SortingSanity, EdgeCases) {
    // Empty vector
    vector<int> empty_vec;
    merge_sort(empty_vec);
    ASSERT_TRUE(empty_vec.empty());

    // Single element
    vector<int> single_vec = {42};
    merge_sort(single_vec);
    ASSERT_EQ(single_vec, vector<int>({42}));

    // Already sorted
    vector<int> sorted_vec = {1, 2, 3, 4, 5};
    vector<int> sorted_copy = sorted_vec;
    merge_sort(sorted_vec);
    ASSERT_EQ(sorted_vec, sorted_copy);
}
