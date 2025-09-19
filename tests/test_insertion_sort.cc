#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include "../algorithms/cpp/luAlgorithms.h" // Inclua o cabeçalho correto

// A função insertion_sort que está sendo testada
// (Assumindo que ela é definida em algum lugar ou incluída via um .h)
// Para o propósito deste teste, vou incluir a definição diretamente, como estava no arquivo original.
// REMOVIDO: A definição de insertion_sort está em luAlgorithms.cpp

// Helper functions (could be moved to a test_utils.h if many tests use them)
static std::vector<int> generate_ordered_vector(int size) {
    std::vector<int> v(size);
    for (int i = 0; i < size; i++) v[i] = i;
    return v;
}

static std::vector<int> generate_reverse_ordered_vector(int size) {
    std::vector<int> v(size);
    for (int i = 0; i < size; i++) v[i] = size - 1 - i;
    return v;
}

static std::vector<int> generate_random_vector(int size) {
    std::vector<int> v(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size * 10);
    for (int i = 0; i < size; i++) v[i] = dis(gen);
    return v;
}

// Test Fixture for Insertion Sort (if needed for common setup/teardown)
class InsertionSortTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for tests, if any
    }

    void TearDown() override {
        // Common teardown for tests, if any
    }
};

// Basic test for an empty vector
TEST_F(InsertionSortTest, EmptyVector) {
    std::vector<int> v = {};
    insertion_sort(v);
    EXPECT_TRUE(v.empty());
}

// Basic test for a single-element vector
TEST_F(InsertionSortTest, SingleElementVector) {
    std::vector<int> v = {42};
    insertion_sort(v);
    std::vector<int> expected = {42};
    EXPECT_EQ(v, expected);
}

// Test with an already sorted vector
TEST_F(InsertionSortTest, AlreadySortedVector) {
    std::vector<int> v = generate_ordered_vector(100);
    insertion_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

// Test with a reverse ordered vector
TEST_F(InsertionSortTest, ReverseOrderedVector) {
    std::vector<int> v = generate_reverse_ordered_vector(100);
    insertion_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

// Test with a random vector
TEST_F(InsertionSortTest, RandomVector) {
    std::vector<int> v = generate_random_vector(100);
    insertion_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

// Test with duplicate elements
TEST_F(InsertionSortTest, DuplicateElements) {
    std::vector<int> v = {5, 2, 8, 2, 5, 1, 8, 3};
    insertion_sort(v);
    std::vector<int> expected = {1, 2, 2, 3, 5, 5, 8, 8};
    EXPECT_EQ(v, expected);
}

// Performance test (optional, as GTest is mainly for correctness)
TEST_F(InsertionSortTest, PerformanceLargeRandomVector) {
    std::vector<int> v = generate_random_vector(10000); // 10,000 elements
    auto start = std::chrono::high_resolution_clock::now();
    insertion_sort(v);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start; // in milliseconds

    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
    // You might add an assertion for performance, e.g., EXPECT_LT(duration.count(), some_threshold_ms);
    // But this depends heavily on the machine, so often just checking correctness is enough.
    std::cout << "\nInsertion Sort 10000 elements (random) took: " << duration.count() << " ms" << std::endl;
}
