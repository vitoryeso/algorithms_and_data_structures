    #include <gtest/gtest.h>
    #include <vector>
    #include <chrono>
    #include "../algorithms/cpp/luAlgorithms.h"

    // Helper function to compare two matrices
    bool compareMatrices(const Matrix& m1, const Matrix& m2) {
        if (m1.size() != m2.size() || m1(0,0) != m2(0,0) /* simplistic check for square matrix size */) {
            return false;
        }
        for (size_t i = 0; i < m1.size(); ++i) {
            for (size_t j = 0; j < m1.size(); ++j) {
                if (m1(i, j) != m2(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    TEST(MatrixMultiplicationTest, SmallMatrices) {
        Matrix A{{1, 2, 3, 4},
                 {5, 6, 7, 8},
                 {9, 10, 11, 12},
                 {13, 14, 15, 16}};

        Matrix B{{16, 15, 14, 13},
                 {12, 11, 10, 9},
                 {8, 7, 6, 5},
                 {4, 3, 2, 1}};

        // Expected result for A * B
        Matrix expected_C{{
            {80, 70, 60, 50},
            {240, 214, 188, 162},
            {400, 358, 316, 274},
            {560, 502, 444, 386}
        }};

        // Test naive multiplication
        Matrix C_naive = matmul_naive(A, B);
        EXPECT_TRUE(compareMatrices(expected_C, C_naive)) << "Naive multiplication result is incorrect.";

        // Test blocked multiplication
        Matrix C_blocked = matmul_blocked(A, B, 2);
        EXPECT_TRUE(compareMatrices(expected_C, C_blocked)) << "Blocked multiplication result is incorrect.";

        // Test optimized blocked multiplication
        Matrix C_blocked_opt = matmul_blocked_optimized(A, B, 2);
        EXPECT_TRUE(compareMatrices(expected_C, C_blocked_opt)) << "Optimized blocked multiplication result is incorrect.";
    }

    // You can add more tests for different matrix sizes, edge cases, etc.
