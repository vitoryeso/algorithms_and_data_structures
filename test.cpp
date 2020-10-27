#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

void insertionSort(vector<unsigned>& A);
void printVec(vector<int>& A);
vector<unsigned> binarySum(vector<unsigned>& A, vector<unsigned>& B);
unsigned linearSearch(vector<unsigned>& A, unsigned value);
void selectionSort(vector<unsigned>& A);
vector<unsigned> merge(vector<unsigned>& A, vector<unsigned>& B);

/* quick sort */
int partition(vector<int>& V, int p, int r); /* vai pegar um pivo e dividir o vetor em 2. menores e maiores q o pivo */
void quickSort(vector<int>& V, int p, int r);

/* binary search
 * obs: only use this if the vector V is sorted */
int binarySearch(vector<int>& V, int p, int r, int value);

int main(void) {
    vector<int> A {1, 10, 14};
    vector<int> B {4, 8, 12, 1123, 32, 93, 27, 44, 19, 20, 10};
    printVec(A);
    printVec(B);  

    quickSort(B, 0, B.size());
    printVec(B);

    cout << "position of 10 in A: " << binarySearch(A, 0, A.size(), 10) << endl;
    cout << "position of 10 in B: " << binarySearch(B, 0, B.size(), 12) << endl;

    return 0;
}

int binarySearch(vector<int>& V, int p, int r, int value) {
  int q = floor((r - p)/2) + p;
  cout << "q: " << q << endl;
  cout << "V[q]: " << V[q] << endl;
  if(V[q] == value) return q;
  else if(V[q] > value){ 
    return binarySearch(V, p, q-1, value);
  }
  else {
    return binarySearch(V, q, r, value);
  }
  return -1;
}


int partition(vector<int>& V, int p, int r) {
  /* considerando o pivo como o ultimo elemento */
  int i= p-1; /* posicao do pivo */
  for(int j=p; j<r - 1; j++) {
    if(V[r - 1] > V[j]) swap(V[++i], V[j]);
  }
  swap(V[r - 1], V[++i]);
  return i;
}

void quickSort(vector<int>& V, int p, int r) {
  if(r-p > 1) {
    int q = partition(V, p, r); 
    quickSort(V, p, q);
    quickSort(V, q+1, r);
  }
}

vector<unsigned> merge(vector<unsigned>& A, vector<unsigned>& B) {
    unsigned M(A.size()), D(B.size());
    vector<unsigned> C;

    unsigned i(0), j(0);
    while(i<M && j<D) {
        if(A[i] <= B[j]) C.push_back(A[i++]);
        else C.push_back(B[j++]);
    }
    if(M == i) {
        for(unsigned n=j;n<D;n++) C.push_back(B[n]);
    }
    else for(unsigned n=i;n<M;n++) C.push_back(A[n]);

    return C;
}

void selectionSort(vector<unsigned>& A) {
    unsigned menor(9999999);
    unsigned index;
    for(unsigned i=0; i<A.size() - 1; i++) {
        for(unsigned j=1; j<A.size(); j++) {
            if(A[j] < menor){
                menor = A[j];
                index = j;
            }              }
        if(A[index] < A[i]) swap(A[index], A[i]);
    }
}

void printVec(vector<int>& A) {
    cout << "[";
    for(unsigned i=0; i<A.size(); i++) {
        if(i == A.size() - 1) {
            cout << A[i] << "];" << endl;
        }
        else {
            cout << A[i] << " ";
        }
    }
}

vector<unsigned> binarySum(vector<unsigned>& A, vector<unsigned>& B) {
    vector<unsigned> C;
    unsigned carry(0);
    unsigned prov;
    for(unsigned i=0; i<A.size(); i++) {
        prov = carry + A[i] + B[i];
        switch(prov) {
            case 0:
               C.push_back(0); 
               carry = 0;
               break;
            case 1:
               C.push_back(1);
               carry = 0;
               break;
            case 2:
               C.push_back(0);
               carry = 1;
               break;
            case 3:
               C.push_back(1);
               carry = 1;
               break;
        }
    } 
    if(carry > 0) C.push_back(carry);
    return C;
}

unsigned linearSearch(vector<unsigned>& A, unsigned value) {
    for(unsigned i=0; i<A.size(); i++) {
        if(A[i] == value) return i;
    }
    return 9999;
}

void insertionSort(vector<unsigned>& A) {
    int j, prov_i;
    for(unsigned i=1; i<A.size(); i++) {
        j = i - 1;
        prov_i = i;
        while((A[prov_i] > A[j]) && j>=0) {
            swap(A[prov_i], A[j]);
            j--;
            prov_i--;
        }
    }
}
