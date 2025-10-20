# Criador de Grafos DAG Aleatórios com Ordenação Topológica

Este programa cria grafos acíclicos direcionados (DAGs) com estrutura forward (como redes neurais) e realiza ordenação topológica usando duas abordagens diferentes.

## Como Funciona

### Estrutura do Grafo
- **Camadas bem definidas**: Cada camada conecta apenas para a próxima (forward)
- **Conexões aleatórias**: 50-80% dos neurônios de uma camada conectam para a próxima
- **Skip connections**: Algumas conexões aleatórias adicionais (garantindo aciclicidade)
- **Pesos aleatórios**: Entre 1-10 para cada aresta

### Algoritmos de Ordenação Topológica
1. **DFS (Depth-First Search)**: Explora profundamente antes de voltar
2. **Kahn (BFS)**: Remove vértices com grau de entrada zero

## Como Usar

### Compilação
```bash
make create_dag
# ou
g++ -std=c++17 -O3 -march=native -funroll-loops -Wall -I. -Ialgorithms/cpp -Idata_structures/cpp -Idata_structures/c -Iutils create_random_dag.cpp -o bin/create_dag
```

### Execução
```bash
./bin/create_dag
```

### Entrada do Usuário
1. **Número de camadas**: Incluindo entrada e saída (mínimo 2)
2. **Neurônios por camada**: Quantidade de vértices em cada camada

### Exemplo de Uso

```
Digite o número de camadas (incluindo entrada e saída): 3
Digite o número de neurônios em cada camada:
Camada 0 (neurônios): 2
Camada 1 (neurônios): 3
Camada 2 (neurônios): 2
```

Isso cria um grafo com:
- **Camada 0 (Entrada)**: 2 neurônios (c0_n0, c0_n1)
- **Camada 1 (Oculta)**: 3 neurônios (c1_n0, c1_n1, c1_n2)
- **Camada 2 (Saída)**: 2 neurônios (c2_n0, c2_n1)

## Saída do Programa

### 1. Visualização do Grafo
```
=== GRAFO DAG ===
c0_n0 -> c1_n0(3) c1_n1(6)
c0_n1 -> c1_n0(7)
c1_n0 -> c2_n0(7)
c1_n1 -> c2_n0(1)
c1_n2 -> c2_n0(7)
c2_n0 -> (sem conexões)
c2_n1 -> (sem conexões)
```

### 2. Estatísticas
```
📊 Estatísticas:
   Total de vértices: 7
   Total de arestas: 6
   Grau médio: 0.86
```

### 3. Export GraphViz (opcional)
```
🔍 Deseja exportar o grafo em formato GraphViz? (s/n): s
✅ Grafo exportado para grafo_dag.dot
💡 Para visualizar: dot -Tpng grafo_dag.dot -o grafo.png && xdg-open grafo.png
```

### 4. Ordenação Topológica
```
=== ORDENAÇÃO TOPOLÓGICA ===
Usando DFS:
c2_n1 -> c1_n2 -> c0_n1 -> c0_n0 -> c1_n1 -> c1_n0 -> c2_n0

Usando Kahn (BFS):
c0_n0 -> c0_n1 -> c1_n2 -> c2_n1 -> c1_n1 -> c1_n0 -> c2_n0
```

## Implementação Técnica

### Estruturas Usadas
- **vector<string> vertices**: Lista de rótulos dos vértices
- **vector<vector<pair<int, int>>> arestas**: Lista de adjacências (índice_destino, peso)

### Funcionalidades Principais
- `criarDAGForward()`: Gera o grafo com estrutura forward
- `ordenacaoTopologicaDFS()`: Ordenação usando DFS
- `ordenacaoTopologicaKahn()`: Ordenação usando algoritmo de Kahn
- `imprimirGrafo()`: Visualização do grafo
- `exportarGraphViz()`: Exporta grafo em formato .dot para visualização

### Garantias
- **Aciclicidade**: Estrutura forward + conexões apenas para índices maiores
- **Conectividade**: Pelo menos uma ordem topológica válida
- **Aleatoriedade**: Conexões e pesos são determinados aleatoriamente

## Exemplos de Uso Educacional

### Rede Neural Simples
- Entrada: 4 neurônios
- Oculta: 6 neurônios
- Saída: 2 neurônios

### Grafo de Dependências
- Camada 0: Tarefas iniciais
- Camada 1: Processamento intermediário
- Camada 2: Resultados finais

## Visualização do Grafo

O programa inclui suporte para exportar grafos em formato GraphViz (.dot):

### Instalação do GraphViz
```bash
# Ubuntu/Debian
sudo apt-get install graphviz

# macOS
brew install graphviz

# Windows
# Baixar de: https://graphviz.org/download/
```

### Visualização
```bash
# Gerar imagem PNG
dot -Tpng grafo_dag.dot -o grafo.png

# Abrir imagem
xdg-open grafo.png  # Linux
open grafo.png      # macOS
start grafo.png     # Windows
```

### Exemplo de Arquivo .dot Gerado
```dot
digraph DAG {
    rankdir=LR;
    node [shape=circle, style=filled, fillcolor=lightblue];

    // Camada 0
    { rank=same; c0_n0; c0_n1; }
    // Camada 1
    { rank=same; c1_n0; c1_n1; c1_n2; }
    // Camada 2
    { rank=same; c2_n0; c2_n1; }

    c0_n0 -> c1_n0 [label="8"];
    c0_n1 -> c1_n0 [label="5"];
    c0_n1 -> c1_n1 [label="7"];
    ...
}
```

## Extensões Possíveis

1. **Pesos customizáveis**: Distribuição diferente para pesos
2. **Estruturas alternativas**: Redes residuais, conexões densas
3. **Detecção de ciclos**: Verificação adicional de aciclicidade
4. **Múltiplas topologias**: Diferentes padrões de conectividade

## Baseado nas Implementações Existentes

Este programa utiliza e estende as classes de grafos já implementadas em:
- `data_structures/cpp/graph_adj_list.cpp`
- `data_structures/cpp/grafo_navegacao.cpp`
- `data_structures/cpp/grafo_menor_caminho.cpp`
