#ifndef AVL_TREE_H
#define AVL_TREE_H

// ============================================================================
// QUESTÃO 13: Árvore AVL (Árvore de Busca Binária Balanceada)
// ============================================================================

/**
 * Estrutura de dados: Árvore AVL
 * 
 * Árvore de busca binária auto-balanceada.
 * Mantém propriedade: |altura(esquerda) - altura(direita)| ≤ 1
 * 
 * Complexidades (todas operações):
 * - INSERT: O(log n)
 * - SEARCH: O(log n)
 * - DELETE: O(log n)
 * 
 * Nota: Você pode implementar Red-Black Tree ao invés de AVL se preferir.
 */
class AVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;  // Altura do nó
        
        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };
    
    Node* root;
    size_t count;
    
    // ========================================================================
    // FUNÇÕES AUXILIARES PRIVADAS
    // ========================================================================
    
    /**
     * Retorna altura de um nó (0 se nullptr).
     * TODO: IMPLEMENTAR
     */
    int get_height(Node* node) const;
    
    /**
     * Atualiza altura de um nó.
     * TODO: IMPLEMENTAR
     */
    void update_height(Node* node);
    
    /**
     * Calcula fator de balanceamento (altura_esquerda - altura_direita).
     * TODO: IMPLEMENTAR
     */
    int balance_factor(Node* node) const;
    
    /**
     * Rotação simples à direita.
     * 
     *       y                x
     *      / \              / \
     *     x   C    =>      A   y
     *    / \                  / \
     *   A   B                B   C
     * 
     * TODO: IMPLEMENTAR
     */
    Node* rotate_right(Node* y);
    
    /**
     * Rotação simples à esquerda.
     * 
     *     x                  y
     *    / \                / \
     *   A   y      =>      x   C
     *      / \            / \
     *     B   C          A   B
     * 
     * TODO: IMPLEMENTAR
     */
    Node* rotate_left(Node* x);
    
    /**
     * Balanceia um nó após inserção/remoção.
     * 
     * Verifica fator de balanceamento e aplica rotações apropriadas:
     * - Left-Left: rotação direita
     * - Right-Right: rotação esquerda
     * - Left-Right: rotação esquerda-direita
     * - Right-Left: rotação direita-esquerda
     * 
     * TODO: IMPLEMENTAR
     */
    Node* balance(Node* node);
    
    /**
     * Insere recursivamente.
     * TODO: IMPLEMENTAR
     */
    Node* insert_helper(Node* node, int key, bool& inserted);
    
    /**
     * Busca recursivamente.
     * TODO: IMPLEMENTAR
     */
    bool search_helper(Node* node, int key) const;
    
    /**
     * Remove recursivamente.
     * TODO: IMPLEMENTAR
     */
    Node* remove_helper(Node* node, int key, bool& removed);
    
    /**
     * Encontra nó com valor mínimo em subárvore.
     * TODO: IMPLEMENTAR
     */
    Node* find_min(Node* node) const;
    
    /**
     * Libera memória recursivamente.
     * TODO: IMPLEMENTAR
     */
    void clear_helper(Node* node);
    
public:
    AVLTree() : root(nullptr), count(0) {}
    
    /**
     * Destrutor.
     */
    ~AVLTree();
    
    /**
     * Insere elemento na árvore.
     * 
     * Mantém propriedade AVL através de rotações.
     * 
     * Complexidade: O(log n)
     * 
     * @param key Valor a inserir
     * 
     * TODO: IMPLEMENTAR
     */
    void insert(int key);
    
    /**
     * Busca elemento na árvore.
     * 
     * Busca binária padrão.
     * 
     * Complexidade: O(log n)
     * 
     * @param key Valor a buscar
     * @return true se encontrado, false caso contrário
     * 
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove elemento da árvore.
     * 
     * Mantém propriedade AVL através de rotações.
     * 
     * Complexidade: O(log n)
     * 
     * @param key Valor a remover
     * @return true se removido, false se não encontrado
     * 
     * TODO: IMPLEMENTAR
     */
    bool remove(int key);
    
    /**
     * Retorna número de elementos.
     */
    size_t size() const { return count; }
    
    /**
     * Verifica se está vazia.
     */
    bool empty() const { return root == nullptr; }
    
    /**
     * Limpa todos os elementos.
     * 
     * TODO: IMPLEMENTAR
     */
    void clear();
    
    /**
     * Retorna altura da árvore.
     */
    int height() const { return get_height(root); }
};

#endif // AVL_TREE_H

