#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <string>
#include "../data_structures/cpp/trees.h"

using namespace std;

void print_options();
void print_sort_options();
void print_search_options();
void print_selected_vector(vector<vector<int>>& vectors, int32_t selectedVector);
void print_all_vectors(vector<vector<int>>& vectors);

void add_vector(vector<vector<int>>& vectors);
void type_values(vector<vector<int>>& vectors, int32_t selectedVector);
int32_t select_vector(vector<vector<int>>& vectors);

void sort_vector(vector<vector<int>>& vectors, int32_t selectedVector, int32_t SORT_CMD);
void search_value(vector<vector<int>>& vectors, int32_t selectedVector, int value, int32_t SEARCH_CMD);

 

#endif
