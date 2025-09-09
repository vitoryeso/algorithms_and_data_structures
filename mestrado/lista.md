### **1. (Exercício 2.1-3 do livro do Cormen) Escreva um pseudocódigo para a busca linear e mostre, usando invariância de laço,  que o seu algoritmo está correto.**

 Algoritmo simples para a busca linear:

```python
int key;
vector V[int][MAX] // MAX pode ser qualquer valor inteiro positivo dentro dos limites de memória da máquina
unsigned i(0);
for i=0 até MAX:
    if V[i] == key: // na vdd pode ser qualquer OP com saída booleana envolvendo V[i] e key
        return i;
return -1;
```

As buscas procuram elementos em estruturas de dados. Dado que uma estrutura não foi checada para verificar a existencia dos elementos chave, ela será checada até que o algoritmo de busca termine.
A invariante de laço nesse exemplo é que **sempre, o próximo elemento de V a ser checado, ainda não foi checado desde o passo anterior.** Isso vale para cada elemento de V a ser percorrido, do início ao fim da busca.

### **2. Implemente o algoritmo de ordenação por inserção e crie uma cópia  anotada dele que mede o número de operações no modelo da Random Access  Machine (RAM, seção 2.2 livro do Cormen). **

### **Usando entradas de tamanho  crescente, mostre em um gráfico quando o tempo de execução no modelo RAM diverge de medições feitas em uma máquina real.**

código do insertion_sort

```cpp
void insertion_sort(vector<int>& V) {
        if (V.size() == 1) return;
        int i, j, left;
        for (i=1; i<V.size(); i++) {
                for (j=i; j>0; j--) {
                	    // swap(V[j], V[j - 1])
                        if (V[j-1] > V[j]) {
                                left = V[j - 1];
                                V[j - 1] = V[j];
                                V[j] = left;
                        }
                        else break;
                }
        }
}
```

código anotado

```cpp
void insertion_sort(vector<int>& V) {
        if (V.size() == 1) return;								 // c1 * 1
        int i, j, left;										    // c2 * 1
        for (i=1; i<V.size(); i++) {						     // c3 * n
                for (j=i; j>0; j--) {                              // c4 * sum(1, n-1, ti)
                	    // swap(V[j], V[j - 1])                   
                        if (V[j-1] > V[j]) {                       // c5 * sum(1, n-1, ti)
                                left = V[j - 1];				// c6 * sum(1, n-1, ti - 1)
                                V[j - 1] = V[j];				// c7 * sum(1, n-1, ti - 1)
                                V[j] = left;					// c8 * sum(1, n-1, ti - 1)
                        }
                        else break;								// c9 * sum(1, n-1, 1)
                }
        }
}
```

Análise de _running time_ _(steps)_

$$
\begin{align*}
(I) T(n) = c_1 + c_2 + nc_3 + c_4\sum_{i=1}^{n-1}t_i + c_5\sum_{i=1}^{n-1}t_i + c_6\sum_{i=1}^{n-1}(t_i - 1) + c_7\sum_{i=1}^{n-1}(t_i - 1) + c_8\sum_{i=1}^{n-1}(t_i - 1) + c_9\sum_{i=1}^{n-1}1 \\
\\
(II) T(n)= c_1 + c_2 + nc_3 + (c_4 + c_5)\sum_{i=1}^{n-1}t_i + (c_6 + c_7 + c_8)\sum_{i=1}^{n-1}(t_i - 1) + c_9(n - 1)
\end{align*}
$$


Análise para Melhor caso:
$$
\begin{align*}
t_i = 1\\
T(n)= c_1 + c_2 + nc_3 + (c_4 + c_5)\sum_{i=1}^{n-1}1 + (c_6 + c_7 + c_8)\sum_{i=1}^{n-1}(1 - 1) + c_9(n - 1)\\
T(n)= c_1 + c_2 + nc_3 + (c_4 + c_5)(n - 1) + c_9(n - 1) \\
T(n) = c_1 + c_2 + nc_3 + nc_4 - c_4 + nc_5 - c_5 + nc_9 - c_9\\
T(n) = n(c_3 + c_4 + c_5 + c_9) + c_{22}\\
c_{22} = c_1 + c_2 - c_4 - c_5 - c_9\\
T(n) \approx{O(n)}
\end{align*}
$$
Análise para o Pior caso:
$$
t_i = i\\
\sum_{i=1}^{n-1}i = \frac{(n-1)n}{2} = \frac{n^2 - n}{2}\\
\sum_{i=1}^{n-1}(i - 1) = \sum_{i=1}^{n-1}i - \sum_{i=1}^{n-1}1 = \frac{(n-1)n}{2} - (n - 1) = \frac{n^2 - 3n + 2}{2}\\
\\
T(n) = c_1 + c_2 + nc_3 + (c_4 + c_5)\sum_{i=1}^{n-1}i + (c_6 + c_7 + c_8)\sum_{i=1}^{n-1}(i - 1) + c_9(n - 1)\\
T(n) = c_1 + c_2 + nc_3 + (c_4 + c_5)\frac{n^2 - n}{2} + (c_6 + c_7 + c_8)\frac{n^2 - 3n + 2}{2} + c_9(n - 1)\\
T(n) = c_1 + c_2 + c_3n + \left(\frac{c_4+c_5}{2}\right)n^2 - \left(\frac{c_4+c_5}{2}\right)n + \left(\frac{c_6+c_7+c_8}{2}\right)n^2 - \left(\frac{3(c_6+c_7+c_8)}{2}\right)n + (c_6+c_7+c_8) + c_9n - c_9\\
\\
T(n) = \left(\frac{c_4+c_5+c_6+c_7+c_8}{2}\right)n^2 + \left(c_3 + c_9 - \frac{c_4+c_5}{2} - \frac{3(c_6+c_7+c_8)}{2}\right)n + (c_1 + c_2 + c_6+c_7+c_8 - c_9)\\
\\
T(n) = an^2 + bn + c\\
\\
Portanto, T(n) \approx O(n^2)\\
\\
\text{Onde os coeficientes a, b, e c são constantes:}\\
a = \frac{c_4+c_5+c_6+c_7+c_8}{2}\\
b = c_3 + c_9 - \frac{c_4+c_5 + 3(c_6+c_7+c_8)}{2}\\
c = c_1 + c_2 + c_6 + c_7 + c_8 - c_9
$$

### **Experimento:** 

Desempenho do Insertion Sort (100 ≤ n ≤ 20k) para vetores ordenado, aleatório e inverso. As linhas tracejadas f(n)=n e g(n)=n² confirmam: melhor caso ~O(n) e pior caso ~O(n²).

<img src="insertion_sort_detailed_performance.png" alt="Experimento Insertion Sort" width="2000">



### **3. Mostre numericamente com suas implementações dos algoritmos de  insertion-sort e merge-sort como se comporta o desempenho de cada  algoritmo utilizando entradas de tamanho crescente, considerando  entradas de pior caso, melhor caso e caso médio.**

### **Análise, para cada tipo de entrada, se existe algum ponto a partir do qual um algoritmo passa a ser mais rápido que o outro.**



### Merge Sort

```cpp
void merge(vector<int>& V, int p,int q,int r) {
  int n1 = q - p + 1;
  int n2 = r - q;
  int V1[n1], V2[n2];
  for(int i=0; i<n1; i++)
    V1[i] = V[p + i];

  for(int i=0; i<n2; i++)
    V2[i] = V[i + q + 1];

  int i = 0, j = 0;
  int v_idx = p;

  while(i<n1 && j<n2){
      if(V1[i] <= V2[j]) {
          V[v_idx] = V1[i];
          i++;
      }
      else {
          V[v_idx] = V2[j];
          j++;
      }
      v_idx++;
  }
  if(n1 == i) {
      for(int j2=j; j2<n2; j2++) {
          V[v_idx] = V2[j2];
          v_idx++;
      }
  }
  else {
      for(int i2=i; i2<n1; i2++) {
          V[v_idx] = V1[i2];
          v_idx++;
      }
  }
}

void merge_sort(vector<int>& V, int p, int r) {
  if(p>=r){
    return;//returns recursively
  }

  int m = (p+r-1)/2;
  merge_sort(V ,p,m);
  merge_sort(V ,m+1,r);
  merge(V ,p,m,r);
}
```



------

## 3.1 Estrutura da recursão

O `merge_sort` divide sempre o vetor em duas partes quase iguais:
$$
T(n) = 2T\!\left(\frac{n}{2}\right) + \Theta(n)
$$

- chamadas recursivas para as duas metades.
  $$
  2T(n/2)
  $$

- custo do `merge`, que sempre percorre **todos os elementos** de cada metade, independente da ordem.
  $$
  \Theta(n)
  $$
  

------

## 3.2 Exemplo numérico

Entrada:
$$
V = [5, 2, 4, 7, 1, 3, 2, 6]
$$


Divisões:
$$
\begin{aligned} [5,2,4,7,1,3,2,6]  &\to [5,2,4,7] \;\;\; [1,3,2,6] \\ &\to [5,2] \; [4,7] \;\;\; [1,3] \; [2,6] \\ &\to [5]\;[2]\;[4]\;[7]\;\;\;[1]\;[3]\;[2]\;[6] \end{aligned}
$$


Em cada nível, o `merge` percorre **todos os elementos** das duas metades:

- Merge `[5]` e `[2]`, `[4]` e `[7]`,  `[1]` e `[3]`, `[2]` e `[6]` : custo:
  $$
  \Theta(2)
  $$
  

- Merge `[2,5]` e `[4,7]`,  `[1,3]` e `[2,6]`: custo:
  $$
  \Theta(4)
  $$
  

- Merge `[2,4,5,7]` e `[1,2,3,6]`: custo:
  $$
  \Theta(8)
  $$
  

Somando:
$$
2+2+2+2+4+4+8 = 24 = \Theta(8 \log 8)
$$


------

## 3.3 Melhor, pior e caso médio

- **Melhor caso (vetor já ordenado)**
   O `merge` ainda copia todos os elementos de ambos os vetores temporários.
  $$
  \Theta(n)
  $$

- **Pior caso (vetor ordenado invertido)**
   O `merge` também percorre todos os elementos de ambos os vetores temporários.
  $$
  \Theta(n)
  $$

- **Caso médio (vetor aleatório, como o exemplo acima)**
   O `merge` faz comparações variadas, mas o laço `while(i<n1 && j<n2)` garante percorrer todos os elementos.
  $$
  \Theta(n)
  $$

------

## 3.4 Análise de Tempo de Execução

O número de níveis da recursão é
$$
\log_2 n
$$
O custo por nível é
$$
\Theta(n)
$$
Portanto:
$$
T(n) = \Theta(n \log n) \quad \text{(melhor, pior e caso médio)}
$$


### Comparação Experimental: Merge Sorte e Insertion Sort

n = 100, ... 20k

Para o vetor ordenado ou próximo de ordenado, o insertion sort é melhor

Para o ovetor inversamente ordenado e aleatório, o merge sort é melhor

<img src="sorting_algorithms_comparison.png" alt="Experimento Insertion Sort" width="4767">



**Vítor Yeso Fidelis Freitas - Programa de Pós Graduação em Engenharia Elétrica e Computação - 2025.2**
