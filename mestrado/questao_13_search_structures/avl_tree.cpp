#include "avl_tree.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

// ============================================================================
// FUNÇÕES AUXILIARES PRIVADAS
// ============================================================================

int AVLTree::get_height(Node* node) const {
    // TODO: IMPLEMENTAR
    // Retorna node ? node->height : 0
    throw runtime_error("AVLTree::get_height não implementado");
}

void AVLTree::update_height(Node* node) {
    // TODO: IMPLEMENTAR
    // node->height = 1 + max(get_height(node->left), get_height(node->right))
    throw runtime_error("AVLTree::update_height não implementado");
}

int AVLTree::balance_factor(Node* node) const {
    // TODO: IMPLEMENTAR
    // Retorna get_height(node->left) - get_height(node->right)
    throw runtime_error("AVLTree::balance_factor não implementado");
}

AVLTree::Node* AVLTree::rotate_right(Node* y) {
    // TODO: IMPLEMENTAR
    //
    // Rotação direita:
    // 1. x = y->left
    // 2. T2 = x->right
    // 3. x->right = y
    // 4. y->left = T2
    // 5. Atualiza alturas de y e x
    // 6. Retorna x (nova raiz da subárvore)
    
    throw runtime_error("AVLTree::rotate_right não implementado");
}

AVLTree::Node* AVLTree::rotate_left(Node* x) {
    // TODO: IMPLEMENTAR
    //
    // Rotação esquerda (espelho de rotate_right)
    
    throw runtime_error("AVLTree::rotate_left não implementado");
}

AVLTree::Node* AVLTree::balance(Node* node) {
    // TODO: IMPLEMENTAR
    //
    // Balanceamento:
    // 1. Atualiza altura do nó
    // 2. Calcula fator de balanceamento
    // 3. Se bf > 1 (desbalanceado à esquerda):
    //    - Se filho esquerdo tem bf < 0: rotação Left-Right
    //    - Caso contrário: rotação Right
    // 4. Se bf < -1 (desbalanceado à direita):
    //    - Se filho direito tem bf > 0: rotação Right-Left
    //    - Caso contrário: rotação Left
    // 5. Retorna nó (potencialmente nova raiz)
    
    throw runtime_error("AVLTree::balance não implementado");
}

AVLTree::Node* AVLTree::insert_helper(Node* node, int key, bool& inserted) {
    // TODO: IMPLEMENTAR
    //
    // Inserção BST padrão + balanceamento:
    // 1. Se node == nullptr:
    //    - inserted = true
    //    - Retorna new Node(key)
    // 2. Se key < node->key:
    //    - node->left = insert_helper(node->left, key, inserted)
    // 3. Else if key > node->key:
    //    - node->right = insert_helper(node->right, key, inserted)
    // 4. Else: chave duplicada, não insere (inserted = false)
    // 5. Retorna balance(node)
    
    throw runtime_error("AVLTree::insert_helper não implementado");
}

bool AVLTree::search_helper(Node* node, int key) const {
    // TODO: IMPLEMENTAR
    //
    // Busca BST padrão:
    // 1. Se node == nullptr: retorna false
    // 2. Se key == node->key: retorna true
    // 3. Se key < node->key: busca na esquerda
    // 4. Caso contrário: busca na direita
    
    throw runtime_error("AVLTree::search_helper não implementado");
}

AVLTree::Node* AVLTree::remove_helper(Node* node, int key, bool& removed) {
    // TODO: IMPLEMENTAR
    //
    // Remoção BST padrão + balanceamento:
    // 1. Se node == nullptr: retorna nullptr
    // 2. Se key < node->key: node->left = remove_helper(...)
    // 3. Else if key > node->key: node->right = remove_helper(...)
    // 4. Else (encontrou):
    //    - removed = true
    //    - Caso 1: nó folha ou com um filho
    //    - Caso 2: nó com dois filhos (substitui por sucessor/predecessor)
    // 5. Se node não foi deletado, retorna balance(node)
    
    throw runtime_error("AVLTree::remove_helper não implementado");
}

AVLTree::Node* AVLTree::find_min(Node* node) const {
    // TODO: IMPLEMENTAR
    // Percorre sempre para a esquerda até encontrar nullptr
    throw runtime_error("AVLTree::find_min não implementado");
}

void AVLTree::clear_helper(Node* node) {
    // TODO: IMPLEMENTAR
    //
    // Percorrimento pós-ordem:
    // 1. Se node == nullptr: retorna
    // 2. clear_helper(node->left)
    // 3. clear_helper(node->right)
    // 4. delete node
    
    throw runtime_error("AVLTree::clear_helper não implementado");
}

// ============================================================================
// INTERFACE PÚBLICA
// ============================================================================

AVLTree::~AVLTree() {
    clear();
}

void AVLTree::insert(int key) {
    // TODO: IMPLEMENTAR
    // bool inserted = false;
    // root = insert_helper(root, key, inserted);
    // if (inserted) count++;
    
    throw runtime_error("AVLTree::insert não implementado");
}

bool AVLTree::search(int key) const {
    // TODO: IMPLEMENTAR
    // return search_helper(root, key);
    
    throw runtime_error("AVLTree::search não implementado");
}

bool AVLTree::remove(int key) {
    // TODO: IMPLEMENTAR
    // bool removed = false;
    // root = remove_helper(root, key, removed);
    // if (removed) count--;
    // return removed;
    
    throw runtime_error("AVLTree::remove não implementado");
}

void AVLTree::clear() {
    // TODO: IMPLEMENTAR
    // clear_helper(root);
    // root = nullptr;
    // count = 0;
    
    throw runtime_error("AVLTree::clear não implementado");
}

