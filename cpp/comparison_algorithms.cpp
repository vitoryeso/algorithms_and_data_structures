#include <iostream>
#include <vector>

using namespace std;

void print_vector(vector<int>& V);

void selection_sort(vector<int>& V);
void insertion_sort(vector<int>& V);
void merge(vector<int>& V, vector<int>& V1, vector<int>& V2);
void merge_sort(vector<int>& V);

int main(){
    vector<int> V {1,4,3,5,3,5,6,6,74,7,5,-3};
    cout << "arranjo V = ";
    print_vector(V);
    cout << endl;

    cout << "arranjo V ordenado com o selection_sort = ";
    selection_sort(V);
    print_vector(V);
    cout << endl;


    vector<int> V2 {1,4,3,5,3,5,6,6,74,7,5,-3};
    cout << "arranjo V2 = ";
    print_vector(V2);
    cout << endl;
    
    cout << "arranjo V2 ordenado com o insertion_sort = ";
    insertion_sort(V2);
    print_vector(V2);
    cout << endl;
    

    vector<int> V3 {1,4,3,5,3,5,6,6,74,7,5,-3};
    cout << "arranjo V3 = ";
    print_vector(V3);
    cout << endl;
    
    cout << "arranjo V3 ordenado com o merge_sort = ";
    merge_sort(V3);
    print_vector(V3);
    cout << endl;

    return 0;
}

void print_vector(vector<int>& V) {
    cout << "[";
    for(unsigned i=0; i<V.size(); i++) {
        cout << " " << V[i] << " ";
    }
    cout << "]";
}

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

