#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "luAlgorithms.h"

using namespace std;

void selection_sort(vector<int>& V){
    for(unsigned i=0; i<V.size() - 1; i++) {
        unsigned min = i;
        for(unsigned j=i + 1; j<V.size(); j++) {
            if(V[j] < V[min]) { 
                min = j;
            }
        }
        swap(V[i], V[min]);
    }
}

void insertion_sort(vector<int>& V){
    for(unsigned i=1; i<V.size(); i++) {
        int key, j;
        key = V[i];
        j = i - 1;
        while(V[j] > key && j>=0) {
            V[j + 1] = V[j];
            j--;
        }
        if(key != V[j + 1]) V[j + 1] = key;
    }
}

void merge(vector<int>& V, vector<int>& V1, vector<int>& V2){
    unsigned n1, n2, i, j;
    n1 = V1.size();
    n2 = V2.size();
    i = j = 0;
    while(i<n1 && j<n2){
        if(V1[i] <= V2[j]) {
            V.push_back(V1[i]);
            i++;
        }
        else {
            V.push_back(V2[j]);
            j++;
        }
    }
    if(n1 == i) {
        for(unsigned j2=j; j2<n2; j2++) {
            V.push_back(V2[j2]);
        }
    }
    else {
        for(unsigned i2=i; i2<n1; i2++) {
            V.push_back(V1[i2]);
        }
    }
}

void merge_sort(vector<int>& V){
    unsigned n = V.size();
    if (n <= 1) return; 
    vector<int> V1, V2;
    for(unsigned i=0; i<n; i++) {
        if(i < n/2) V1.push_back(V[i]);
        else V2.push_back(V[i]);
    }
    merge_sort(V1);
    merge_sort(V2);

    V.clear();
    merge(V, V1, V2);
}

void quick_sort(vector<int>& V, const string pivot_choice) {
        if(pivot_choice == "random") randomized_quick_sort(V, 0, V.size() - 1);
        else if(pivot_choice == "median3") median3_quick_sort(V, 0, V.size() - 1);
        else if(pivot_choice == "last") quick_sort(V, 0, V.size() - 1);
        else randomized_quick_sort(V, 0, V.size() - 1);
}

void quick_sort(vector<int>& V, const unsigned p, const unsigned r) {
    if(r > p) {
        const unsigned q = partition(V, p, r);
        quick_sort(V, p, q - 1);
        quick_sort(V, q + 1, r);
    }
}

const unsigned partition(vector<int>& V, const unsigned p, const unsigned r) {
    // pivot = V[r]
    int i = p - 1;
    for(unsigned j=p; j<r; j++) {
        if(V[j] < V[r]) {
            swap(V[i + 1], V[j]);
            i++;
        }
    }
    swap(V[i + 1], V[r]);
    return i + 1;
}

void randomized_quick_sort(vector<int>& V, const unsigned p, const unsigned r) {
    if(r > p) {
        unsigned q = randomized_partition(V, p, r);
        randomized_quick_sort(V, p, q - 1);
        randomized_quick_sort(V, q + 1, r);
    }
}

const unsigned randomized_partition(vector<int>& V, const unsigned p, const unsigned r) {
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(p, r); 
   
    // swap between a random position and the last element
    swap(V[dist(rng)], V[r]);
    return partition(V, p, r);
}

void median3_quick_sort(vector<int>& V, const unsigned p, const unsigned r) {
    if(r > p) {
        unsigned q = median3_partition(V, p, r);
        median3_quick_sort(V, p, q - 1);
        median3_quick_sort(V, q + 1, r);
    }
}

const unsigned median3_partition(vector<int>& V, const unsigned p, const unsigned r) {
    int provId = (r - p)/2;
    if(V[p] > V[provId]) { 
        if(V[p] < V[r]) swap(V[p], V[r]);
        else if(V[provId] > V[r]) swap(V[provId], V[r]);
    }
    return partition(V, p, r);
}
           
void merge_insertion_sort(vector<int>& V, unsigned k) {
    if(V.size() <= k) {
        insertion_sort(V);
    }
    else {
        vector<int> V1, V2;
        unsigned n = V.size();
        for(unsigned i=0; i<n; i++) {
            if(i < n/2) V1.push_back(V[i]);
            else V2.push_back(V[i]);
        }
        merge_insertion_sort(V1);
        merge_insertion_sort(V2);

        V.clear();
        merge(V, V1, V2);
    }
}

const int binary_search(vector<int>& V, const int value) {
    return binary_search(V, 0, V.size() - 1, value); 
}

const int binary_search(vector<int>& V, const unsigned p, const unsigned r, const int value) {
    unsigned q = (r - p) /2;
    if(V[q] == value) return q;
    else if(V[q] > value) {
        binary_search(V, q + 1, r, value);
    }
    else if(V[q] < value) {
        binary_search(V, p, q - 1, value);
    }
    return -1;
}

void counting_sort(vector<int>& V, const int k) {
    vector<int> prov(k + 1, 0), prov2(V);
    for(unsigned i=0; i<V.size(); i++) prov[V[i]]++;
    for(unsigned i=1; i<prov.size(); i++) prov[i] += prov[i - 1];
    for(unsigned i=V.size(); i>0; i--) {
        V[prov[prov2[i - 1]] - 1] = prov2[i - 1];
        prov[prov2[i - 1]]--;
    }
}
