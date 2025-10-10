#include "linked_list.h"
#include <stdexcept>

using namespace std;

UnorderedLinkedList::~UnorderedLinkedList() {
    clear();
}

void UnorderedLinkedList::insert(int key) {
    // TODO: IMPLEMENTAR
    //
    // Inserção no início (O(1)):
    // 1. Cria novo nó
    // 2. novo->next = head
    // 3. head = novo
    // 4. Incrementa count
    
    throw runtime_error("UnorderedLinkedList::insert não implementado");
}

bool UnorderedLinkedList::search(int key) const {
    // TODO: IMPLEMENTAR
    //
    // Busca linear:
    // 1. current = head
    // 2. Enquanto current != nullptr:
    //    - Se current->key == key, retorna true
    //    - current = current->next
    // 3. Retorna false
    
    throw runtime_error("UnorderedLinkedList::search não implementado");
}

bool UnorderedLinkedList::remove(int key) {
    // TODO: IMPLEMENTAR
    //
    // Remoção:
    // 1. Se lista vazia, retorna false
    // 2. Se head->key == key:
    //    - temp = head
    //    - head = head->next
    //    - delete temp
    //    - Decrementa count
    //    - Retorna true
    // 3. Caso contrário, percorre lista com dois ponteiros (prev e current)
    // 4. Quando encontrar, ajusta prev->next e libera current
    // 5. Decrementa count e retorna true
    // 6. Se não encontrar, retorna false
    
    throw runtime_error("UnorderedLinkedList::remove não implementado");
}

void UnorderedLinkedList::clear() {
    // TODO: IMPLEMENTAR
    //
    // Libera todos os nós:
    // 1. Enquanto head != nullptr:
    //    - temp = head
    //    - head = head->next
    //    - delete temp
    // 2. count = 0
    
    throw runtime_error("UnorderedLinkedList::clear não implementado");
}

