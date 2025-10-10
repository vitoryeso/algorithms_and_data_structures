#include "hash_open_addressing.h"
#include <stdexcept>

using namespace std;

HashTableOpenAddressing::HashTableOpenAddressing(size_t size)
    : table(size), num_elements(0), table_size(size) {
}

size_t HashTableOpenAddressing::hash(int key) const {
    return (key % table_size + table_size) % table_size;
}

size_t HashTableOpenAddressing::probe(int key, size_t i) const {
    return (hash(key) + i) % table_size;
}

bool HashTableOpenAddressing::insert(int key) {
    for (size_t i = 0; i < table_size; i++) {
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY || table[idx].status == Status::DELETED) {
            table[idx].key = key;
            table[idx].status = Status::OCCUPIED;
            num_elements++;
            return true;
        }
    }
    return false;
}

bool HashTableOpenAddressing::search(int key) const {
    for (size_t i = 0; i < table_size; i++) {
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY) {
            return false;
        }
        if (table[idx].status == Status::OCCUPIED && table[idx].key == key) {
            return true;
        }
    }
    return false;
}

bool HashTableOpenAddressing::remove(int key) {
    for (size_t i = 0; i < table_size; i++) {
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY) {
            return false;
        }
        if (table[idx].status == Status::OCCUPIED && table[idx].key == key) {
            table[idx].status = Status::DELETED;
            num_elements--;
            return true;
        }
    }
    return false;
}

double HashTableOpenAddressing::load_factor() const {
    return static_cast<double>(num_elements) / table_size;
}

void HashTableOpenAddressing::clear() {
    for (auto& entry : table) {
        entry.status = Status::EMPTY;
    }
    num_elements = 0;
}

size_t HashTableOpenAddressing::count_total_probes() const {
    size_t total_probes = 0;
    for (size_t idx = 0; idx < table_size; idx++) {
        if (table[idx].status == Status::OCCUPIED) {
            int key = table[idx].key;
            for (size_t i = 0; i < table_size; i++) {
                size_t probe_idx = probe(key, i);
                total_probes++;
                if (probe_idx == idx) {
                    break;
                }
            }
        }
    }
    return total_probes;
}

size_t HashTableOpenAddressing::max_cluster_size() const {
    size_t max_cluster = 0;
    size_t current_cluster = 0;
    
    for (size_t i = 0; i < table_size; i++) {
        if (table[i].status == Status::OCCUPIED || table[i].status == Status::DELETED) {
            current_cluster++;
            max_cluster = max(max_cluster, current_cluster);
        } else {
            current_cluster = 0;
        }
    }
    
    return max_cluster;
}

