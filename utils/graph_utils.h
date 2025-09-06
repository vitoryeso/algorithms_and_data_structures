#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <iostream>
#include <vector>
#include <string>

void print_arr(bool *vi, int len);
void print_vertices(std::vector<std::string> vec);
bool *arr_xor(bool *arr_1, bool *arr_2, unsigned size);
bool *arr_or(bool *arr_1, bool *arr_2, unsigned size);

#endif
