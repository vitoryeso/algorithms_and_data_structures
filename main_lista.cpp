#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

// Assinatura que você deve implementar
void insertion_sort(vector<int>& V) {
	if (V.size() == 1) return;
	int i, j, left;
	for (i=1; i<V.size(); i++) {
		for (j=i; j>0; j--) {
			if (V[j-1] > V[j]) {
				left = V[j - 1];
				// swap(V[j], V[j - 1])
				V[j - 1] = V[j];
				V[j] = left;
			}
			else break;
		}
	}
}

// Assinatura para implementar o merge_sort
void merge_sort(vector<int>& V) {
    if (V.size() == 1) return;
    int mid = V.size() / 2;
    vector<int> left(V.begin(), V.begin() + mid);
    vector<int> right(V.begin() + mid, V.end());
    merge_sort(left);
    merge_sort(right);
    merge(left, right, V);
}

static vector<int> read_vector_from_brackets_line() {
    // Lê uma linha no formato: [10 3 5 2]
    string line;
    getline(cin, line);
    while (line.size() == 0) getline(cin, line);
    if (line.front() != '[' || line.back() != ']') {
        cerr << "Formato incorreto. Use: [10 3 5 2]" << endl;
        return {};
    }
    // remove colchetes
    line.erase(line.begin());
    line.pop_back();

    istringstream iss(line);
    vector<int> v{istream_iterator<int>{iss}, istream_iterator<int>{}};
    return v;
}

static void print_vector(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i + 1 < v.size()) cout << " ";
    }
    cout << "]\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Digite o vetor no formato [a b c d]:\n";
    vector<int> v = read_vector_from_brackets_line();

    cout << "Vetor original: ";
    print_vector(v);

    insertion_sort(v);

    cout << "Vetor ordenado:  ";
    print_vector(v);

    return 0;
}


