#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <iterator>
#include <vector>
#include "utils.h"
#include "luAlgorithms.h"

using namespace std;

enum SEARCH_CMD {
    BINARY=1
};

enum SORT_CMD {
    INSERTION = 1,
    SELECTION,
    MERGE,
    MERGE_INSERTION,
    QUICK,
    QUICK_RAND,
    QUICK_MED3,
    COUNTING,
	BUCKET,
    RADIX
};

void print_options() {
    cerr << "OPCOES:\n";
    cerr << "-1: sair\n";
    cerr << "1: criar um vetor\n";
    cerr << "2: selecionar um vetor\n";
    cerr << "3: digitar os valores de um vetor criado\n";
    cerr << "4: imprimir um vetor selecionado\n";
    cerr << "5: imprimir todos os vetores criados\n";
    cerr << "6: ordena um vetor selecionado\n";
    cerr << "7: procura um valor no vetor selecionado\n";
}

void print_sort_options() {
    cerr << "OPCOES DE ORDENACAO:\n";
    cerr << "1: insertion\n";
    cerr << "2: selection\n";
    cerr << "3: merge\n";
    cerr << "4: merge com insertion\n";
    cerr << "5: quick\n";
    cerr << "6: quick aleatorio\n";
    cerr << "7: quick mediana de 3 elementos\n";
    cerr << "8: counting sort\n";
	cerr << "9: bucket sort\n";
    cerr << "10: radix sort\n";
}

void print_search_options() {
    cerr << "OPCOES DE BUSCA\n";
    cerr << "1: binary\n";
}

void print_selected_vector(vector<vector<int>>& vectors, int32_t selectedVector) {
    if(selectedVector == -1) {
        cerr << "nenhum vetor selecionado.\n";    
    }
    else {
        int prov = vectors[selectedVector].size();
        if(prov > 0) {
            cout << "vetor " << selectedVector << ": [";
            for(unsigned i=0; i<prov - 1; i++) {
                cout << vectors[selectedVector][i] << " ";
            }
            cout << vectors[selectedVector][prov - 1] << "]\n";
        }
        else cout << "vetor " << selectedVector << ": []\n";
    }
}

void print_all_vectors(vector<vector<int>>& vectors) {
    for(unsigned i=0; i<vectors.size(); i++) print_selected_vector(vectors, i);
}

void add_vector(vector<vector<int>>& vectors) {
    vector<int> prov;
    vectors.push_back(prov);
}

void type_values(vector<vector<int>>& vectors, int32_t selectedVector) {
    if(selectedVector == -1) {
        cerr << "nenhum vetor selecionado.\n";
    }
    else {
        string vetor;
        cin.ignore();
        while(1) {
            vetor = "";
            cout << "digite o vetor como no exemplo-> vetor 2: [10 23 12 100]\nvetor " << selectedVector << ": ";
            getline(cin, vetor); 
            if(vetor[0] != '[' || vetor[vetor.length() - 1] != ']') {
                cout << "formato incorreto.\n";
                continue;
            }
            else {
                istringstream prov(vetor);
                vector<string> splitted_vector( (istream_iterator<string>{prov}), istream_iterator<string>() );
                splitted_vector[0].erase(0, 1);
                splitted_vector[splitted_vector.size() - 1].pop_back();
                for(unsigned i=0; i<splitted_vector.size(); i++) {
                    vectors[selectedVector].push_back(stoi(splitted_vector[i]));
                }
                cerr << "o vetor foi adicionado.\n";
                print_selected_vector(vectors, selectedVector);
                break;
            }
        }
    }     
}

int32_t select_vector(vector<vector<int>>& vectors) {
    print_all_vectors(vectors);
    cerr << "digite o numero do vetor que deseja selecionar: ";
    int32_t prov;
    cin >> prov;
    if(prov > vectors.size() || prov < 1) {
        cerr << "numero invalido.\n";
        return -1; 
    }
    else {
        cout << "vetor " << prov << " selecionado!\n";
        return prov - 1;
    }
}

void sort_vector(vector<vector<int>>& vectors, int32_t selectedVector, int32_t SORT_CMD) {
    if(selectedVector != -1) {
        cout << "metodo " << SORT_CMD << " selecionado!" << endl;
        switch(SORT_CMD) {
            case INSERTION:
                insertion_sort(vectors[selectedVector]);
                break;

            case SELECTION:
                selection_sort(vectors[selectedVector]);
                break;

            case MERGE:
                merge_sort(vectors[selectedVector]);
                break; 

            case MERGE_INSERTION:
                merge_insertion_sort(vectors[selectedVector]);
                break;

            case QUICK:
                quick_sort(vectors[selectedVector], "last");
                break;

            case QUICK_RAND:
                quick_sort(vectors[selectedVector], "random");
                break;

            case QUICK_MED3:
                quick_sort(vectors[selectedVector], "median3");
                break;
        
            case COUNTING:
                counting_sort(vectors[selectedVector]);
                break;
			
			case BUCKET:
				bucket_sort(vectors[selectedVector]);
				break;

            case RADIX:
                radix_sort(vectors[selectedVector]);
                break;

            default:
                cerr << "metodo invalido\n";
                break;
        }
        print_selected_vector(vectors, selectedVector);
    }
    else cerr << "vetor invalido.\n";
}

void search_value(vector<vector<int>>& vectors, int32_t selectedVector, int value, int32_t SEARCH_CMD) {
    int prov;
    switch(SEARCH_CMD) {
        case BINARY:
            prov = binary_search(vectors[selectedVector], value);
            if(prov != -1) {
                cout << "o valor foi encontrado na posicao " << prov << endl;
            }
            else cerr << "o valor nao foi encontrado\n";
            break;
        default:
            cerr << "metodo invalido.\n";
            break;
    }
}
