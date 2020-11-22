#ifndef _LUALGORITHMS_H_
#define _LUALGORITHMS_H_

#include <string>
#include <vector>

using namespace std;

void selection_sort(vector<int>& V);

void insertion_sort(vector<int>& V);

void merge(vector<int>& V, int p, int q, int r);
void merge_sort(vector<int>& V, int p, int r);
void merge_sort(vector<int>& V);

void quick_sort(vector<int>& V, const string pivot_choice);
void quick_sort(vector<int>& V);

void quick_sort(vector<int>& V, int p, int r);
void randomized_quick_sort(vector<int>& V, const unsigned p, const unsigned r);
void median3_quick_sort(vector<int>& V, const unsigned p, const unsigned r);

int partition(vector<int>& V, int p, int r);
const unsigned randomized_partition(vector<int>& V, const unsigned p, const unsigned r);
const unsigned median3_partition(vector<int>& V, const unsigned p, const unsigned r);

void counting_sort(vector<int>& V, const int k);
void counting_sort(vector<int>& V);
vector<int> counting_sort_pairs(vector<pair<int, int>>& V, const int k);

void bucket_sort(vector<int>& V, const int k, const unsigned n_buckets);
void bucket_sort(vector<int>& V);

vector<pair<int, int>> get_pairs(vector<int>& V, const unsigned base, const unsigned d);
void radix_sort(vector<int>& V, const int base, const unsigned d);
void radix_sort(vector<int>& V);

const int binary_search(vector<int>& V, const int value);
const int binary_search(vector<int>& V, const unsigned p, const unsigned r, const int value);

#endif
