#include "graph_utils.h"

void print_arr(bool *vi, int len) {
    for (int i=0; i<len; i++) {
        std::cout << vi[i] << " ";
    }
    std::cout << std::endl;
}

void print_vertices(std::vector<std::string> vec) {
    for (unsigned i=0; i<vec.size(); i++) {
        std::cout << vec[i] << "; ";
    }
    std::cout << std::endl;
}

bool *arr_xor(bool *arr_1, bool *arr_2, unsigned size) {
    bool *out = new bool[size];
    for (unsigned i=0; i<size; i++) {
        out[i] = arr_1[i] xor arr_2[i];
    }
    return out;
}

bool *arr_or(bool *arr_1, bool *arr_2, unsigned size) {
    bool *out = new bool[size];
    for (unsigned i=0; i<size; i++) {
        out[i] = arr_1[i] or arr_2[i];
    }
    return out;
}
