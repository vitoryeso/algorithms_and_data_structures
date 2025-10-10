# Questões do Mestrado - Lista 2

Este diretório contém implementações para as questões 10-15 da lista de exercícios.

## 📁 Estrutura

Cada questão tem seu próprio diretório com:
- **`.h` e `.cpp`**: Arquivos com interfaces prontas (você implementa os corpos)
- **`benchmark_*.cpp`**: Benchmarks completos (já implementados)
- **`test_*.cc`**: Testes unitários com Google Test (já implementados)

## 🎯 Questões

### Questão 10: Min/Max Simultâneo

**Objetivo**: Implementar algoritmo de mínimo e máximo simultâneos (Cormen 9.1) e demonstrar que é mais rápido que abordagem separada.

**Arquivos para implementar**:
- `questao_10_minmax/minmax.cpp`

**Funções a implementar**:
1. `find_minmax_separate()` - Abordagem não-simultânea (2n-2 comparações)
2. `find_minmax_simultaneous()` - Abordagem simultânea (~3n/2 comparações)

**Compilar e rodar**:
```bash
make benchmark_q10
./bin/benchmark_q10 --max-size 10000000 --runs 5 --output-csv reports/q10_results.csv
```

**O que o benchmark mede**:
- Tempo de execução vs tamanho do array
- Número de comparações realizadas
- Comparação das duas abordagens
- Diferentes distribuições (uniforme, ordenado, reverso)

---

### Questão 11: Seleção (Randomized vs Deterministic)

**Objetivo**: Implementar seleção aleatória (9.2) e determinística (9.3) do Cormen e demonstrar vantagens de cada uma.

**Arquivos para implementar**:
- `questao_11_selection/selection.cpp`

**Funções a implementar**:
1. `randomized_select()` - O(n) esperado
2. `deterministic_select()` - O(n) pior caso (median-of-medians)
3. Funções auxiliares: `partition()`, `randomized_partition()`, `median_of_medians()`

**Compilar e rodar**:
```bash
make benchmark_q11
./bin/benchmark_q11 --max-size 1000000 --runs 5 --output-csv reports/q11_results.csv
```

**O que o benchmark mede**:
- Tempo para diferentes posições (i = 1, n/4, n/2, 3n/4, n)
- Casos: aleatório, ordenado, reverso, com duplicatas
- Comparação randomized vs deterministic
- Quando um é melhor que o outro

---

### Questão 12: Mediana Ponderada

**Objetivo**: Implementar mediana ponderada e resolver Problema 9-3(e) do Cormen.

**Arquivos para implementar**:
- `questao_12_weighted_median/weighted_median.cpp`

**Funções a implementar**:
1. `weighted_median()` - Encontra mediana ponderada
2. `optimal_point_weighted()` - Resolve 9-3(e): ponto que minimiza distâncias ponderadas
3. Funções auxiliares

**Compilar e rodar**:
```bash
make benchmark_q12
./bin/benchmark_q12 --max-size 100000 --runs 5 --output-csv reports/q12_results.csv
```

**O que o benchmark mede**:
- Tempo vs tamanho
- Verificação de corretude (compara custo com força bruta)
- Diferentes distribuições de pesos

---

### Questão 13: Estruturas de Busca

**Objetivo**: Implementar 3 estruturas (array ordenado, lista encadeada, AVL) e comparar INSERT e SEARCH.

**Arquivos para implementar**:
- `questao_13_search_structures/sorted_array.cpp`
- `questao_13_search_structures/linked_list.cpp`
- `questao_13_search_structures/avl_tree.cpp`

**Estruturas**:
1. **SortedArray**: INSERT O(n), SEARCH O(log n)
2. **UnorderedLinkedList**: INSERT O(1), SEARCH O(n)
3. **AVLTree**: INSERT O(log n), SEARCH O(log n)

**Compilar e rodar**:
```bash
make benchmark_q13
./bin/benchmark_q13 --max-size 100000 --runs 3 --output-csv reports/q13_results.csv
```

**O que o benchmark mede**:
- Tempo de inserção para n elementos
- Tempo de busca (aleatória e sequencial)
- Diferentes ordens de inserção (aleatória, ordenada, reversa)
- Comparação das 3 estruturas

---

### Questão 14: Hash Tables (Chaining vs Open Addressing)

**Objetivo**: Implementar tabelas hash com encadeamento e endereçamento aberto e comparar vantagens/desvantagens.

**Arquivos para implementar**:
- `questao_14_hash_tables/hash_chaining.cpp`
- `questao_14_hash_tables/hash_open_addressing.cpp`

**Implementações**:
1. **HashTableChaining**: Listas encadeadas para colisões
2. **HashTableOpenAddressing**: Linear probing

**Compilar e rodar**:
```bash
make benchmark_q14
./bin/benchmark_q14 --max-size 100000 --runs 3 --output-csv reports/q14_results.csv
```

**O que o benchmark mede**:
- INSERT, SEARCH, DELETE vs fator de carga α
- Performance para α = 0.1, 0.25, 0.5, 0.75, 0.9, 0.95
- Número de colisões
- Clustering (para open addressing)
- Comparação das duas abordagens

---

### Questão 15: Hash Probing (Linear vs Double)

**Objetivo**: Comparar linear probing e double hashing, analisando clustering e performance.

**Arquivos para implementar**:
- `questao_15_hash_probing/hash_linear_probing.cpp`
- `questao_15_hash_probing/hash_double_hashing.cpp`

**Implementações**:
1. **HashTableLinearProbing**: h(k,i) = (h(k) + i) mod m
2. **HashTableDoubleHashing**: h(k,i) = (h₁(k) + i·h₂(k)) mod m

**Compilar e rodar**:
```bash
make benchmark_q15
./bin/benchmark_q15 --max-size 100000 --runs 3 --output-csv reports/q15_results.csv
```

**O que o benchmark mede**:
- Número de probes por operação
- Clustering primário (maior em linear probing)
- Performance vs fator de carga
- Comparação das duas técnicas
- Quando double hashing compensa overhead

---

## 🚀 Workflow

### 1. Implementar
Você só precisa implementar os corpos das funções nos arquivos `.cpp`. As interfaces já estão prontas nos `.h`.

### 2. Testar
```bash
make test_q10  # Compila testes
./bin/test_q10  # Roda testes unitários
```

### 3. Fazer Benchmark
```bash
make benchmark_q10
./bin/benchmark_q10 --max-size 1000000 --runs 5 --output-csv reports/q10_results.csv
```

### 4. Gerar Gráficos
```bash
python3 mestrado/common/plot_questoes.py reports/q10_results.csv
```
Gráficos serão salvos em `mestrado/plots_questoes/`.

### 5. Rodar Todas as Questões
```bash
make run_all_questoes
```

---

## 📊 Parâmetros dos Benchmarks

Todos os benchmarks aceitam:
- `--max-size <n>`: Tamanho máximo dos dados
- `--min-size <n>`: Tamanho mínimo (padrão: 1000)
- `--runs <n>`: Número de execuções por teste (padrão: 3)
- `--output-csv <arquivo>`: Arquivo CSV de saída
- `--quiet`: Modo silencioso
- `--help`: Mostra ajuda

---

## 📈 Saída dos Benchmarks

Cada benchmark gera CSV com:
- `size`: Tamanho do input
- `algorithm`: Algoritmo testado
- `data_type`: Tipo de dados (random, sorted, reverse, etc.)
- `run_index`: Número da execução
- `seed`: Seed usada
- `time_s`: Tempo em segundos
- `comparisons`: Número de comparações (quando aplicável)
- Metadata do sistema (CPU, OS, compiler)

---

## 🔧 Utilitários Comuns

`mestrado/common/questao_utils.h` fornece:
- Geração de dados (aleatórios, ordenados, com duplicatas)
- Timer de alta precisão
- Cálculo de estatísticas (média, mediana, desvio padrão)
- Salvamento de resultados em CSV
- Utilitários de CLI

Você pode usar essas funções nos seus algoritmos se precisar.

---

## 📝 Dicas de Implementação

### Questão 10 (Min/Max)
- Abordagem separada: percorre array 2 vezes
- Abordagem simultânea: processa elementos em pares, 3 comparações por par

### Questão 11 (Seleção)
- Randomized: mais rápido na prática, implementação mais simples
- Deterministic: garantia O(n) pior caso, mas constantes maiores
- Median-of-medians: divida em grupos de 5, ordene cada grupo

### Questão 12 (Mediana Ponderada)
- Baseie-se no algoritmo de seleção
- Acumule pesos ao particionar
- Para 9-3(e): a resposta é simplesmente a mediana ponderada!

### Questão 13 (Estruturas de Busca)
- SortedArray: use `lower_bound` da STL para busca binária
- LinkedList: implementação clássica de lista encadeada
- AVL: cuidado com as rotações (4 casos: LL, RR, LR, RL)

### Questão 14 (Hash Chaining vs Open Addressing)
- Chaining: simples, cada posição é uma lista
- Open Addressing: use DELETED marker, não EMPTY ao remover

### Questão 15 (Linear vs Double Hashing)
- Linear probing: incrementa de 1 em 1
- Double hashing: h₂(k) deve ser relativamente primo com m
- Meça clustering: double hashing deve ter menos

---

## 🧪 Testes

Testes unitários verificam:
- Corretude dos algoritmos
- Casos extremos (arrays vazios, tamanho 1, etc.)
- Propriedades das estruturas de dados
- Invariantes (ex: AVL mantém altura balanceada)

---

## 📖 Referências

- Cormen et al., "Introduction to Algorithms", 4ª Ed.
  - Cap. 9: Medians and Order Statistics
  - Cap. 11: Hash Tables
  - Cap. 12-13: Binary Search Trees

---

**Boa sorte com as implementações!** ☕

Qualquer dúvida, consulte os comentários nos arquivos `.h` ou os exemplos de uso nos benchmarks.

