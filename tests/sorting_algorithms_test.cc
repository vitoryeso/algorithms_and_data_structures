#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>
#include "../algorithms/cpp/luAlgorithms.h"

using namespace std;

class SortingAlgorithmsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Seed para gerar números aleatórios consistentes
        rng.seed(42);
    }

    // Gerador de números aleatórios
    mt19937 rng;
    
    // Tamanhos de teste
    const int TINY_SIZE = 10;
    const int LOW_SIZE = 1000;
    const int MEDIUM_SIZE = 1000000; // 1M elementos (aproximadamente 4MB)
    
    // Função para gerar array aleatório
    vector<int> generateRandomArray(int size) {
        vector<int> arr(size);
        uniform_int_distribution<int> dist(1, 1000);
        for (int i = 0; i < size; i++) {
            arr[i] = dist(rng);
        }
        return arr;
    }
    
    // Função para gerar array já ordenado
    vector<int> generateSortedArray(int size) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
        return arr;
    }
    
    // Função para gerar array inversamente ordenado
    vector<int> generateReverseSortedArray(int size) {
        vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
        return arr;
    }
    
    // Função para verificar se array está ordenado
    bool isSorted(const vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); i++) {
            if (arr[i] < arr[i-1]) {
                return false;
            }
        }
        return true;
    }
    
    // Função para verificar se arrays são iguais (ignorando ordem)
    bool arraysEqual(const vector<int>& arr1, const vector<int>& arr2) {
        if (arr1.size() != arr2.size()) return false;
        
        vector<int> sorted1 = arr1;
        vector<int> sorted2 = arr2;
        sort(sorted1.begin(), sorted1.end());
        sort(sorted2.begin(), sorted2.end());
        
        return sorted1 == sorted2;
    }
    
    // Template para testar algoritmo de ordenação
    template<typename SortFunction>
    void testSortingAlgorithm(SortFunction sortFunc, const string& algorithmName) {
        cout << "\n=== Testando " << algorithmName << " ===" << endl;
        
        // Teste com array vazio
        {
            cout << "  - Array vazio..." << flush;
            vector<int> empty;
            vector<int> original = empty;
            sortFunc(empty);
            EXPECT_TRUE(isSorted(empty)) << algorithmName << " falhou com array vazio";
            EXPECT_TRUE(arraysEqual(original, empty)) << algorithmName << " modificou array vazio";
            cout << " OK" << endl;
        }
        
        // Teste com array de 1 elemento
        {
            cout << "  - Array de 1 elemento..." << flush;
            vector<int> single = {42};
            vector<int> original = single;
            sortFunc(single);
            EXPECT_TRUE(isSorted(single)) << algorithmName << " falhou com array de 1 elemento";
            EXPECT_TRUE(arraysEqual(original, single)) << algorithmName << " modificou array de 1 elemento";
            cout << " OK" << endl;
        }
        
        // Teste TINY - Array aleatório
        {
            cout << "  - TINY (aleatório)..." << flush;
            vector<int> arr = generateRandomArray(TINY_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array aleatório TINY";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array aleatório TINY";
            cout << " OK" << endl;
        }
        
        // Teste TINY - Array já ordenado
        {
            cout << "  - TINY (já ordenado)..." << flush;
            vector<int> arr = generateSortedArray(TINY_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array já ordenado TINY";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array já ordenado TINY";
            cout << " OK" << endl;
        }
        
        // Teste TINY - Array inversamente ordenado
        {
            cout << "  - TINY (inversamente ordenado)..." << flush;
            vector<int> arr = generateReverseSortedArray(TINY_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array inversamente ordenado TINY";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array inversamente ordenado TINY";
            cout << " OK" << endl;
        }
        
        // Teste LOW - Array aleatório
        {
            cout << "  - LOW (aleatório)..." << flush;
            vector<int> arr = generateRandomArray(LOW_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array aleatório LOW";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array aleatório LOW";
            cout << " OK" << endl;
        }
        
        // Teste LOW - Array já ordenado
        {
            cout << "  - LOW (já ordenado)..." << flush;
            vector<int> arr = generateSortedArray(LOW_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array já ordenado LOW";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array já ordenado LOW";
            cout << " OK" << endl;
        }
        
        // Teste LOW - Array inversamente ordenado
        {
            cout << "  - LOW (inversamente ordenado)..." << flush;
            vector<int> arr = generateReverseSortedArray(LOW_SIZE);
            vector<int> original = arr;
            sortFunc(arr);
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array inversamente ordenado LOW";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array inversamente ordenado LOW";
            cout << " OK" << endl;
        }
        
        // Teste MEDIUM - Array aleatório (apenas para algoritmos eficientes)
        if (algorithmName == "merge_sort" || algorithmName == "quick_sort" || 
            algorithmName == "counting_sort" || algorithmName == "bucket_sort" || 
            algorithmName == "radix_sort") {
            cout << "  - MEDIUM (aleatório)..." << flush;
            vector<int> arr = generateRandomArray(MEDIUM_SIZE);
            vector<int> original = arr;
            
            auto start = chrono::high_resolution_clock::now();
            sortFunc(arr);
            auto end = chrono::high_resolution_clock::now();
            
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            
            EXPECT_TRUE(isSorted(arr)) << algorithmName << " falhou com array aleatório MEDIUM";
            EXPECT_TRUE(arraysEqual(original, arr)) << algorithmName << " perdeu elementos no array aleatório MEDIUM";
            
            cout << " OK (" << duration.count() << "ms)" << endl;
        }
        
        cout << "=== " << algorithmName << " concluído ===" << endl;
    }
};

// Testes para Selection Sort
TEST_F(SortingAlgorithmsTest, SelectionSort) {
    testSortingAlgorithm([](vector<int>& arr) { selection_sort(arr); }, "selection_sort");
}

// Testes para Insertion Sort
TEST_F(SortingAlgorithmsTest, InsertionSort) {
    testSortingAlgorithm([](vector<int>& arr) { insertion_sort(arr); }, "insertion_sort");
}

// Testes para Merge Sort
TEST_F(SortingAlgorithmsTest, MergeSort) {
    testSortingAlgorithm([](vector<int>& arr) { merge_sort(arr); }, "merge_sort");
}

// Testes para Quick Sort (pivô último elemento)
TEST_F(SortingAlgorithmsTest, QuickSortLast) {
    testSortingAlgorithm([](vector<int>& arr) { quick_sort(arr, "last"); }, "quick_sort_last");
}

// Testes para Quick Sort (pivô aleatório)
TEST_F(SortingAlgorithmsTest, QuickSortRandom) {
    testSortingAlgorithm([](vector<int>& arr) { quick_sort(arr, "random"); }, "quick_sort_random");
}

// Testes para Quick Sort (mediana de 3)
TEST_F(SortingAlgorithmsTest, QuickSortMedian3) {
    testSortingAlgorithm([](vector<int>& arr) { quick_sort(arr, "median3"); }, "quick_sort_median3");
}

// Testes para Counting Sort
TEST_F(SortingAlgorithmsTest, CountingSort) {
    testSortingAlgorithm([](vector<int>& arr) { counting_sort(arr); }, "counting_sort");
}

// Testes para Bucket Sort
TEST_F(SortingAlgorithmsTest, BucketSort) {
    testSortingAlgorithm([](vector<int>& arr) { bucket_sort(arr); }, "bucket_sort");
}

// Testes para Radix Sort
TEST_F(SortingAlgorithmsTest, RadixSort) {
    testSortingAlgorithm([](vector<int>& arr) { radix_sort(arr); }, "radix_sort");
}

// Teste de performance comparativa
TEST_F(SortingAlgorithmsTest, PerformanceComparison) {
    const int testSize = 10000;
    vector<int> randomArray = generateRandomArray(testSize);
    
    // Teste com diferentes algoritmos
    vector<pair<string, function<void(vector<int>&)>>> algorithms = {
        {"selection_sort", [](vector<int>& arr) { selection_sort(arr); }},
        {"insertion_sort", [](vector<int>& arr) { insertion_sort(arr); }},
        {"merge_sort", [](vector<int>& arr) { merge_sort(arr); }},
        {"quick_sort_last", [](vector<int>& arr) { quick_sort(arr, "last"); }},
        {"quick_sort_random", [](vector<int>& arr) { quick_sort(arr, "random"); }},
        {"quick_sort_median3", [](vector<int>& arr) { quick_sort(arr, "median3"); }},
        {"counting_sort", [](vector<int>& arr) { counting_sort(arr); }},
        {"bucket_sort", [](vector<int>& arr) { bucket_sort(arr); }},
        {"radix_sort", [](vector<int>& arr) { radix_sort(arr); }}
    };
    
    cout << "\n=== Performance Comparison (Size: " << testSize << ") ===" << endl;
    
    for (auto& [name, func] : algorithms) {
        vector<int> testArray = randomArray;
        
        auto start = chrono::high_resolution_clock::now();
        func(testArray);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        EXPECT_TRUE(isSorted(testArray)) << name << " falhou no teste de performance";
        
        cout << name << ": " << duration.count() << "μs" << endl;
    }
}
