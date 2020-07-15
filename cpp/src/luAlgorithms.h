#ifndef _LUALGORITHMS_H_
#define _LUALGORITHMS_H_

#include <string>
#include <vector>

using namespace std;

void selection_sort(vector<int>& V);

void insertion_sort(vector<int>& V);

void merge(vector<int>& V, vector<int>& V1, vector<int>& V2);
void merge_sort(vector<int>& V);
void merge_insertion_sort(vector<int>& V, unsigned k=4);

void quick_sort(vector<int>& V, const string pivot_choice);

void quick_sort(vector<int>& V, const unsigned p, const unsigned r);
void randomized_quick_sort(vector<int>& V, const unsigned p, const unsigned r);
void median3_quick_sort(vector<int>& V, const unsigned p, const unsigned r);

const unsigned partition(vector<int>& V, const unsigned p, const unsigned r);
const unsigned randomized_partition(vector<int>& V, const unsigned p, const unsigned r);
const unsigned median3_partition(vector<int>& V, const unsigned p, const unsigned r);

void counting_sort(vector<int>& V, const int k);

void bucket_sort(vector<int>& V, const unsigned n_buckets, const int k);

const int binary_search(vector<int>& V, const int value);
const int binary_search(vector<int>& V, const unsigned p, const unsigned r, const int value);

#endif
