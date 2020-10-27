#include <iostream>
#include <string>
#include <limits>
#include "./algorithms/cpp/luAlgorithms.h"
#include "./utils/utils.h"

using namespace std;

enum CMD {
    EXIT = -1,
    CREATE_VECTOR = 1,
    SELECT_VECTOR,
    TYPE_VALUES,
    SHOW_SELECTED_VECTOR,
    SHOW_ALL_VECTORS,
    SORT_VECTOR,
    SEARCH_VALUE
};

int main(){
    int32_t CMD, selectedVector(-1), metodo_ordenacao, metodo_busca;
    int value;
    vector<vector<int>> vectors;
    bool run = true;
    while(run) {
        print_options();
        cerr << "digite o comando: ";
        cin >> CMD;
        switch(CMD) {
            case EXIT:
                cerr << "tchau !\n";
                run = false;
                break;
                
            case CREATE_VECTOR:
                add_vector(vectors);
                break;

            case SELECT_VECTOR:
                selectedVector = select_vector(vectors);
                break;

            case TYPE_VALUES:
                type_values(vectors, selectedVector);
                break;

            case SHOW_SELECTED_VECTOR:
                print_selected_vector(vectors, selectedVector);
                break;

            case SHOW_ALL_VECTORS:
                print_all_vectors(vectors);
                break;

            case SORT_VECTOR:
                print_sort_options();
                cerr << "digite o metodo: ";
                cin >> metodo_ordenacao;
                sort_vector(vectors, selectedVector, metodo_ordenacao);
                break;

            case SEARCH_VALUE:
                print_search_options();
                cerr << "digite o valor: ";
                cin >> value;
                cerr << "digite o metodo de busca: ";
                cin >> metodo_busca;
                search_value(vectors, selectedVector, value, metodo_busca); 
                break;
            
            default:
                cerr << "comando ERRADO\n";
                print_options();
                break;
        }
    }
    return 0;
}
