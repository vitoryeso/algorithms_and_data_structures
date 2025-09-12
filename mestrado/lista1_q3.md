## Questão 3 — Merge Sort (resposta proposta)

### Ideia geral
O merge sort divide o vetor em duas metades, ordena recursivamente cada metade e intercala as duas metades ordenadas em um único vetor ordenado. O custo por nível é linear no tamanho do vetor (intercalação), e há aproximadamente log2(n) níveis de recursão.

### Diagrama Mermaid — Divisão e Merge (único, horizontal)
```mermaid
flowchart LR
  %% Divisão
  A["V[0..7]"] --> M["Split"]
  M --> B["V[0..3]"]
  M --> C["V[4..7]"]
  B --> M2["Split"]
  
  M2 --> D["V[0..1]"]
  M2 --> E["V[2..3]"]
  
  C --> M3["Split"]
  M3 --> F["V[4..5]"]
  M3 --> G["V[6..7]"]

  %% Bases após camada de tamanho 2
  D --> P1["n < 2?"]
  E --> P2["n < 2?"]
  F --> P3["n < 2?"]
  G --> P4["n < 2?"]
  
  %% 
  P1 --> D_["V_sorted[0..1]"]
  P2 --> E_["V_sorted[2..3]"]
  P3 --> F_["V_sorted[4..5]"]
  P4 --> G_["V_sorted[6..7]"]

  %% Etapa de merges
  D_ --> M03["merge"]
  E_ --> M03
  M03 --> E__["V[0..3]"]

  F_ --> M47["merge"] 
  G_ --> M47
  M47 --> F__["V[4..7]"]
  
  E__ --> M4["merge"]
  F__ --> M4

  M4 --> M07["V[0..7]"]
```

### Intercalação (merge) e estabilidade
Considere duas metades ordenadas `V1` e `V2`. Mantemos três índices: `i` (em `V1`), `j` (em `V2`) e `k` (posição de escrita em `V`). A cada passo escrevemos o menor entre `V1[i]` e `V2[j]` em `V[k]` e avançamos o(s) índice(s) correspondente(s). A condição `V1[i] <= V2[j]` garante estabilidade: em empates, o elemento da esquerda sai primeiro.

Trecho representativo:

```cpp
while (i < n1 && j < n2) {
    if (V1[i] <= V2[j]) { V[k++] = V1[i++]; }
    else { V[k++] = V2[j++]; }
}
// Copia o restante do lado não esgotado
```

### Passo a passo numérico Merge

#### Exemplo A — tamanho ímpar
Suponha `p=0, r=6, q=2`, com metades já ordenadas:

- `V1 = [1, 4, 9]` (n1=3), `V2 = [2, 3, 10, 11]` (n2=4)

```mermaid
flowchart TB
    %% Arrays iniciais no lado esquerdo
    n1["V1 = [1, 4, 9] indice i"]
    n2["V2 = [2, 3, 10, 11] indice j"]
    
    %% Fluxo principal
    s0["s0"] -->|"1<=2 => V[0]=1"| s1["(i=1,j=0,k=1)"]
    s1 -->|"4<=2? não => V[1]=2"| s2["(i=1,j=1,k=2)"]
    s2 -->|"4<=3? não => V[2]=3"| s3["(i=1,j=2,k=3)"]
    s3 -->|"4<=10 => V[3]=4"| s4["(i=2,j=2,k=4)"]
    s4 -->|"9<=10 => V[4]=9"| s5["(i=3,j=2,k=5)"]
    s5 -->|"i==n1 => copia V2: V[5]=10, V[6]=11"| s6["(fim)"]
    
    %% Estados do array V alinhados com o fluxo
    s0 -.-> n3["V = [*, *, *, *, *, *, *] (k=0)"]
    s1 -.-> n4["V = [1, *, *, *, *, *, *] (k=1)"]
    s2 -.-> n5["V = [1, 2, *, *, *, *, *] (k=2)"]
    s3 -.-> n6["V = [1, 2, 3, *, *, *, *] (k=3)"]
    s4 -.-> n7["V = [1, 2, 3, 4, *, *, *] (k=4)"]
    s5 -.-> n8["V = [1, 2, 3, 4, 9, *, *] (k=5)"]
    s6 -.-> n9["V = [1, 2, 3, 4, 9, 10, 11] (k=6)"]


```

#### Exemplo B — tamanho par
Suponha `p=0, r=7, q=3`:

- `V1 = [1, 3a, 5a, 7a]`, `V2 = [3b, 5b, 7b, 9]` (sufixos apenas para rastrear a origem)

```mermaid
flowchart TB
    %% Arrays iniciais no lado esquerdo
    n1["V1 = [1, 3a, 5a, 7a] indice i"]
    n2["V2 = [3b, 5b, 7b, 9] indice j"]
    
    %% Fluxo principal
    s0["s0"] -->|"1<=3b => V[0]=1"| s1["(i=1,j=0,k=1)"]
    s1 -->|"3a<=3b => V[1]=3a"| s2["(i=2,j=0,k=2)"]
    s2 -->|"5a<=3b? não => V[2]=3b"| s3["(i=2,j=1,k=3)"]
    s3 -->|"5a<=5b => V[3]=5a"| s4["(i=3,j=1,k=4)"]
    s4 -->|"7a<=5b? não => V[4]=5b"| s5["(i=3,j=2,k=5)"]
    s5 -->|"7a<=7b => V[5]=7a"| s6["(i=4,j=2,k=6)"]
    s6 -->|"i==n1 => copia V2: V[6]=7b, V[7]=9"| s7["(fim)"]
    
    %% Estados do array V alinhados com o fluxo
    s0 -.-> n3["V = [_, *, *, *, *, *, *, *]</br> (k=0)"]
    s1 -.-> n4["V = [1, *, *, *, *, *, *, *]</br> (k=1)"]
    s2 -.-> n5["V = [1, 3a, *, *, *, *, *, *]</br> (k=2)"]
    s3 -.-> n6["V = [1, 3a, 3b, *, *, *, *, *]</br> (k=3)"]
    s4 -.-> n7["V = [1, 3a, 3b, 5a, *, *, *, *]</br> (k=4)"]
    s5 -.-> n8["V = [1, 3a, 3b, 5a, 5b, *, *, *]</br> (k=5)"]
    s6 -.-> n9["V = [1, 3a, 3b, 5a, 5b, 7a, *, *]</br> (k=6)"]
    s7 -.-> n10["V = [1, 3a, 3b, 5a, 5b, 7a, 7b, 9]</br> (k=7)"]
```

### Complexidade
- Tempo por nível: Θ(n) (intercalação percorre todos os elementos das metades)
- Níveis da recursão: ≈ log₂(n)
- Resultado: `T(n) = Θ(n log n)` para melhor, médio e pior casos
- Espaço adicional: Θ(n) para os vetores temporários (ou Θ(n) no total, dependendo da implementação)

### Observações práticas e comparação breve com insertion sort
- Em vetores muito pequenos ou quase ordenados, o insertion sort costuma ser mais rápido devido a constantes menores e melhor localidade de referência.
- Para entradas aleatórias ou invertidas e tamanhos moderados/grandes, o merge sort domina por garantir `Θ(n log n)`.

### Paralelização do merge (breve)
O laço acima faz uma varredura sequencial (dependência de dados passo a passo). Para paralelizar, usa-se uma partição por diagonais (“merge path”): escolhe-se posições alvo `k` em `V` e encontra-se, por busca binária, pares `(i, j)` tais que `i + j = k` e `V1[i-1] <= V2[j]` e `V2[j-1] <= V1[i]`. Cada thread mescla um segmento independente. Isso mantém complexidade total `O(n)` com sobrecusto de `O(t log n)` para `t` partições, e é eficaz em GPUs/CPUs multinúcleo quando `n` é grande.

