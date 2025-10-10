#include "minmax.h"
#include <gtest/gtest.h>
#include <vector>
#include <limits>

using namespace std;

// ============================================================================
// TESTES BÁSICOS
// ============================================================================

TEST(MinMaxTest, EmptyArray) {
    vector<int> empty;
    EXPECT_THROW(find_minmax_separate(empty), runtime_error);
    EXPECT_THROW(find_minmax_simultaneous(empty), runtime_error);
}

TEST(MinMaxTest, SingleElement) {
    vector<int> arr = {42};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 42);
    EXPECT_EQ(sep.max, 42);
    EXPECT_EQ(sep.comparisons, 0);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 42);
    EXPECT_EQ(sim.max, 42);
}

TEST(MinMaxTest, TwoElements) {
    vector<int> arr = {10, 20};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 10);
    EXPECT_EQ(sep.max, 20);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 10);
    EXPECT_EQ(sim.max, 20);
}

TEST(MinMaxTest, TwoElementsReverse) {
    vector<int> arr = {20, 10};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 10);
    EXPECT_EQ(sep.max, 20);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 10);
    EXPECT_EQ(sim.max, 20);
}

// ============================================================================
// TESTES DE CORRETUDE
// ============================================================================

TEST(MinMaxTest, SmallArrayOdd) {
    vector<int> arr = {5, 2, 8, 1, 9};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 1);
    EXPECT_EQ(sep.max, 9);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 1);
    EXPECT_EQ(sim.max, 9);
}

TEST(MinMaxTest, SmallArrayEven) {
    vector<int> arr = {5, 2, 8, 1, 9, 3};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 1);
    EXPECT_EQ(sep.max, 9);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 1);
    EXPECT_EQ(sim.max, 9);
}

TEST(MinMaxTest, AllSame) {
    vector<int> arr = {7, 7, 7, 7, 7};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 7);
    EXPECT_EQ(sep.max, 7);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 7);
    EXPECT_EQ(sim.max, 7);
}

TEST(MinMaxTest, Sorted) {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 1);
    EXPECT_EQ(sep.max, 10);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 1);
    EXPECT_EQ(sim.max, 10);
}

TEST(MinMaxTest, Reverse) {
    vector<int> arr = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 1);
    EXPECT_EQ(sep.max, 10);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 1);
    EXPECT_EQ(sim.max, 10);
}

TEST(MinMaxTest, NegativeNumbers) {
    vector<int> arr = {-5, -2, -8, -1, -9};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, -9);
    EXPECT_EQ(sep.max, -1);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, -9);
    EXPECT_EQ(sim.max, -1);
}

TEST(MinMaxTest, MixedPositiveNegative) {
    vector<int> arr = {-5, 2, -8, 1, 9, -3};
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, -8);
    EXPECT_EQ(sep.max, 9);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, -8);
    EXPECT_EQ(sim.max, 9);
}

// ============================================================================
// TESTES DE COMPARAÇÕES
// ============================================================================

TEST(MinMaxTest, ComparisonsSmallArrayEven) {
    vector<int> arr = {5, 2, 8, 1, 9, 3}; // n=6
    
    MinMaxResult sep = find_minmax_separate(arr);
    // Abordagem separada: 2n-2 = 2*6-2 = 10 comparações
    EXPECT_EQ(sep.comparisons, 10);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    // Abordagem simultânea: 3*floor(n/2) = 3*3 = 9 comparações (para n par)
    // Ou no máximo 3*floor(n/2) + caso inicial
    EXPECT_LE(sim.comparisons, sep.comparisons);
}

TEST(MinMaxTest, ComparisonsSmallArrayOdd) {
    vector<int> arr = {5, 2, 8, 1, 9}; // n=5
    
    MinMaxResult sep = find_minmax_separate(arr);
    // Abordagem separada: 2n-2 = 2*5-2 = 8 comparações
    EXPECT_EQ(sep.comparisons, 8);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    // Abordagem simultânea: 3*floor(n/2) = 3*2 = 6 comparações para os pares
    // + 2 para o elemento ímpar = 8, mas a inicialização pode variar
    EXPECT_LE(sim.comparisons, sep.comparisons);
}

TEST(MinMaxTest, SimultaneousUsesFewerComparisons) {
    vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3}; // n=10
    
    MinMaxResult sep = find_minmax_separate(arr);
    MinMaxResult sim = find_minmax_simultaneous(arr);
    
    // Simultânea deve usar menos ou igual comparações
    EXPECT_LE(sim.comparisons, sep.comparisons);
    
    // Para n=10: separada usa 18, simultânea usa no máximo 15
    EXPECT_EQ(sep.comparisons, 18);
    EXPECT_LE(sim.comparisons, 15);
}

// ============================================================================
// TESTES DAS VERSÕES SEM CONTAGEM
// ============================================================================

TEST(MinMaxTest, MinMaxSeparateSimple) {
    vector<int> arr = {5, 2, 8, 1, 9};
    
    auto result = minmax_separate(arr);
    EXPECT_EQ(result.first, 1);
    EXPECT_EQ(result.second, 9);
}

TEST(MinMaxTest, MinMaxSimultaneousSimple) {
    vector<int> arr = {5, 2, 8, 1, 9};
    
    auto result = minmax_simultaneous(arr);
    EXPECT_EQ(result.first, 1);
    EXPECT_EQ(result.second, 9);
}

TEST(MinMaxTest, BothVersionsAgree) {
    vector<int> arr = {42, 17, 93, 3, 56, 28, 71};
    
    auto sep = minmax_separate(arr);
    auto sim = minmax_simultaneous(arr);
    
    EXPECT_EQ(sep.first, sim.first);
    EXPECT_EQ(sep.second, sim.second);
}

// ============================================================================
// TESTES DE STRESS (arrays maiores)
// ============================================================================

TEST(MinMaxTest, LargeArray) {
    vector<int> arr(10000);
    for (int i = 0; i < 10000; i++) {
        arr[i] = i * 2 + 1;
    }
    
    MinMaxResult sep = find_minmax_separate(arr);
    EXPECT_EQ(sep.min, 1);
    EXPECT_EQ(sep.max, 19999);
    
    MinMaxResult sim = find_minmax_simultaneous(arr);
    EXPECT_EQ(sim.min, 1);
    EXPECT_EQ(sim.max, 19999);
    
    // Verifica eficiência
    EXPECT_LE(sim.comparisons, sep.comparisons);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

