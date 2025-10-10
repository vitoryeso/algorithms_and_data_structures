#include "hash_chaining.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

HashTableChaining::HashTableChaining(size_t size) 
    : table(size), num_elements(0), table_size(size) {
}

size_t HashTableChaining::hash(int key) const {
    return (key % table_size + table_size) % table_size;
}

void HashTableChaining::insert(int key) {
    size_t idx = hash(key);
    table[idx].push_back(key);
    num_elements++;
}

bool HashTableChaining::search(int key) const {
    size_t idx = hash(key);
    auto it = find(table[idx].begin(), table[idx].end(), key);
    return it != table[idx].end();
}

bool HashTableChaining::remove(int key) {
    size_t idx = hash(key);
    auto it = find(table[idx].begin(), table[idx].end(), key);
    if (it != table[idx].end()) {
        table[idx].erase(it);
        num_elements--;
        return true;
    }
    return false;
}

double HashTableChaining::load_factor() const {
    return static_cast<double>(num_elements) / table_size;
}

void HashTableChaining::clear() {
    for (auto& chain : table) {
        chain.clear();
    }
    num_elements = 0;
}

size_t HashTableChaining::count_collisions() const {
    size_t collisions = 0;
    for (const auto& chain : table) {
        if (chain.size() > 1) {
            collisions++;
        }
    }
    return collisions;
}

size_t HashTableChaining::max_chain_length() const {
    size_t max_len = 0;
    for (const auto& chain : table) {
        max_len = max(max_len, chain.size());
    }
    return max_len;
}

