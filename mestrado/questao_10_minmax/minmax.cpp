#include "minmax.h"
#include <stdexcept>
#include <limits>

using namespace std;

// ============================================================================
// ABORDAGEM NÃO-SIMULTÂNEA
// ============================================================================

MinMaxResult find_minmax_separate(const vector<int>& arr) {
    // TODO: IMPLEMENTAR
    // 
    // Dicas:
    // 1. Verificar se array está vazio
    // 2. Inicializar min com o primeiro elemento
    // 3. Percorrer array comparando cada elemento com min (contar comparações)
    // 4. Inicializar max com o primeiro elemento
    // 5. Percorrer array comparando cada elemento com max (contar comparações)
    // 6. Retornar MinMaxResult(min, max, comparisons)

    if (arr.empty()) {
        throw runtime_error("Array vazio");
    }
    
    int min = arr[0];
    int max = arr[0];
    long long comparisons = 0;
    
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        comparisons++;
        
        if (arr[i] > max) {
            max = arr[i];
        }
        comparisons++;
    }
    
    return MinMaxResult(min, max, comparisons);
}

// ============================================================================
// ABORDAGEM SIMULTÂNEA
// ============================================================================

MinMaxResult find_minmax_simultaneous(const vector<int>& arr) {
    if (arr.empty()) {
        throw runtime_error("Array vazio");
    }

    const size_t n = arr.size();
    long long comparisons = 0;
    int currentMin = 0;
    int currentMax = 0;
    size_t start = 0;

    if (n % 2 == 1) {
        currentMin = arr[0];
        currentMax = arr[0];
        start = 1;
    } else {
        bool c = arr[0] < arr[1];
        currentMin = c ? arr[0] : arr[1];
        currentMax = c ? arr[1] : arr[0];
        comparisons = 1; // comparação inicial do par (0,1)
        start = 2;
    }

    for (size_t i = start; i + 1 < n; i += 2) {
        bool pairCmp = arr[i] < arr[i + 1];
        int lo = pairCmp ? arr[i] : arr[i + 1];
        int hi = pairCmp ? arr[i + 1] : arr[i];
        // 1 (pair) + 1 (lo vs min) + 1 (hi vs max)
        currentMin = (lo < currentMin) ? lo : currentMin;
        currentMax = (hi > currentMax) ? hi : currentMax;
        comparisons += 3;
    }

    return MinMaxResult(currentMin, currentMax, comparisons);
}

// ============================================================================
// VERSÕES SEM CONTAGEM
// ============================================================================

pair<int, int> minmax_separate(const vector<int>& arr) {
    if (arr.empty()) {
        throw runtime_error("Array vazio");
    }
    int mn = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < mn) mn = arr[i];
    }
    int mx = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > mx) mx = arr[i];
    }
    return {mn, mx};
}

pair<int, int> minmax_simultaneous(const vector<int>& arr) {
    if (arr.empty()) {
        throw runtime_error("Array vazio");
    }
    const size_t n = arr.size();
    int currentMin, currentMax;
    size_t start = 0;
    if (n % 2 == 1) {
        currentMin = arr[0];
        currentMax = arr[0];
        start = 1;
    } else {
        bool c = arr[0] < arr[1];
        currentMin = c ? arr[0] : arr[1];
        currentMax = c ? arr[1] : arr[0];
        start = 2;
    }
    for (size_t i = start; i + 1 < n; i += 2) {
        bool pairCmp = arr[i] < arr[i + 1];
        int lo = pairCmp ? arr[i] : arr[i + 1];
        int hi = pairCmp ? arr[i + 1] : arr[i];
        currentMin = (lo < currentMin) ? lo : currentMin;
        currentMax = (hi > currentMax) ? hi : currentMax;
    }
    return {currentMin, currentMax};
}

// =========================================================================
// VARIANTES GROK (para comparação de estilo)
// =========================================================================

pair<int, int> minmax_separate_grok(const vector<int>& arr) {
    if (arr.empty()) throw runtime_error("Array vazio");
    int min_val = arr[0], max_val = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }
    return {min_val, max_val};
}

pair<int, int> minmax_simultaneous_grok(const vector<int>& arr) {
    if (arr.empty()) throw runtime_error("Array vazio");
    if (arr.size() < 2) return {arr[0], arr[0]};

    int min_val = arr[0] < arr[1] ? arr[0] : arr[1];
    int max_val = arr[0] < arr[1] ? arr[1] : arr[0];
    for (size_t i = 2; i < arr.size(); i += 2) {
        int a = arr[i];
        int b = (i + 1 < arr.size()) ? arr[i + 1] : a;
        int lo = (a < b) ? a : b;
        int hi = (a < b) ? b : a;
        min_val = (lo < min_val) ? lo : min_val;
        max_val = (hi > max_val) ? hi : max_val;
    }
    return {min_val, max_val};
}

