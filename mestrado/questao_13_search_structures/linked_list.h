#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// ============================================================================
// QUESTÃO 13: Lista Encadeada Não Ordenada
// ============================================================================

/**
 * Estrutura de dados: Lista Encadeada Não Ordenada
 * 
 * Lista simplesmente encadeada, sem manter ordem.
 * 
 * Complexidades:
 * - INSERT: O(1) - insere no início
 * - SEARCH: O(n) - precisa percorrer toda lista
 * - DELETE: O(n) - precisa encontrar elemento
 */
class UnorderedLinkedList {
private:
    struct Node {
        int key;
        Node* next;
        
        Node(int k) : key(k), next(nullptr) {}
    };
    
    Node* head;
    size_t count;
    
public:
    UnorderedLinkedList() : head(nullptr), count(0) {}
    
    /**
     * Destrutor - libera toda memória.
     */
    ~UnorderedLinkedList();
    
    /**
     * Insere elemento no início da lista.
     * 
     * Complexidade: O(1)
     * 
     * @param key Valor a inserir
     * 
     * TODO: IMPLEMENTAR
     */
    void insert(int key);
    
    /**
     * Busca elemento na lista.
     * 
     * Percorre lista linearmente.
     * 
     * Complexidade: O(n)
     * 
     * @param key Valor a buscar
     * @return true se encontrado, false caso contrário
     * 
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove primeira ocorrência de elemento.
     * 
     * Algoritmo:
     * 1. Busca elemento percorrendo lista
     * 2. Se encontrado, ajusta ponteiros e libera nó
     * 
     * Complexidade: O(n)
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
    bool empty() const { return head == nullptr; }
    
    /**
     * Limpa todos os elementos.
     * 
     * TODO: IMPLEMENTAR
     */
    void clear();
};

#endif // LINKED_LIST_H

