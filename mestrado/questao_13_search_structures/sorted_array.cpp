#include "sorted_array.h"
#include <algorithm>

using namespace std;

void SortedArray::insert(int key) {
    // TODO: IMPLEMENTAR
    //
    // Opção 1 (mais simples):
    // 1. data.push_back(key)
    // 2. Encontra posição correta percorrendo de trás para frente
    // 3. Desloca elementos maiores
    //
    // Opção 2 (mais eficiente):
    // 1. Usa lower_bound para encontrar posição
    // 2. Insere nessa posição: data.insert(data.begin() + pos, key)
    
    throw runtime_error("SortedArray::insert não implementado");
}

bool SortedArray::search(int key) const {
    // TODO: IMPLEMENTAR
    //
    // Use busca binária (binary_search ou lower_bound da STL, ou implemente)
    //
    // Com STL:
    // return binary_search(data.begin(), data.end(), key);
    
    throw runtime_error("SortedArray::search não implementado");
}

bool SortedArray::remove(int key) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo:
    // 1. Usa lower_bound para encontrar posição do elemento
    // 2. Verifica se elemento está presente
    // 3. Se sim, remove com data.erase(it)
    // 4. Retorna true se removeu, false caso contrário
    
    throw runtime_error("SortedArray::remove não implementado");
}

