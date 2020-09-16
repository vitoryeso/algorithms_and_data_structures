#include <iostream>
#include <vector>

using namespace std;

void insertionSort(vector<unsigned>& A);
void printVec(vector<unsigned>& A);
vector<unsigned> binarySum(vector<unsigned>& A, vector<unsigned>& B);
unsigned linearSearch(vector<unsigned>& A, unsigned value);

int main(void) {
    vector<unsigned> A {1, 1, 1};
    vector<unsigned> B {0, 0, 1};
    printVec(A);
    printVec(B);  

    vector<unsigned> C = binarySum(A, B);
    printVec(C);

    return 0;
}

void printVec(vector<unsigned>& A) {
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
