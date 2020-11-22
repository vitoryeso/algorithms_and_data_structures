#include <opencv2/opencv.hpp>
#include "../algorithms/cpp/luAlgorithms.h"
#include <vector>
#include <chrono>

/* calculate the time of the function, using a random V[N] as input */
int get_time(void (*f)(std::vector<int>&), int N);

/* calculate the time of the function, using a random V[N] and a random value to search as input */
int get_time_search(void (*f)(std::vector<int>&, const int value), int N);

void printVec(std::vector<int> V);

int main(void) {
  char key;
  int i(10);
  cin >> key;
  while((int)key != 27) {
    cout << "N: " << i << endl;
    cout << " INSERTION time(microseconds): " << get_time(insertion_sort, i) << endl;;
    cout << " MERGE time(microseconds): " << get_time(merge_sort, i) << endl;;
    cout << " QUICK time(microseconds): " << get_time(quick_sort, i) << endl;;
    cin >> key;
    i *= 10;
  }

  return 0;
}

void printVec(std::vector<int> V) {
  cout << "V = [";
  for(int i=0; i<(int)V.size(); i++) {
    if(i == (int)V.size() - 1) cout << V[i] << "]" << endl;
    else cout << V[i] << " ";
  }
}

int get_time(void (*f)(std::vector<int>&), int N) {
  vector<int> V(N); 
  // Generate Random values 
  auto func = []() -> int { return rand() % 10000; }; 
  // Fill up the vector 
  generate(V.begin(), V.end(), func); 

  auto start = std::chrono::high_resolution_clock::now();
  f(V);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  return duration.count();
}
