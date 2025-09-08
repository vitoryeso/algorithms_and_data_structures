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
        for (i=1; i<V.size(); i++) {						     // c3 * (n - 1)
                for (j=i; j>0; j--) {                              // c4 * (n - 1) * (n - i)
                	    // swap(V[j], V[j - 1])                   
                        if (V[j-1] > V[j]) {                       // c5 * (n - 1) * (n - 1 - i)
                                left = V[j - 1];
                                V[j - 1] = V[j];
                                V[j] = left;
                        }
                        else break;								// c6 * (n - 1) * i - 1
                }
        }
}
```

Análise para o melhor e pior caso

```
o melhor caso é o array ordenado, e o pior caso é o array inversamente ordenado
```

Análise para o caso médio

```
```

Experimento para o melhor e pior caso

```
```

Experimento para o caso médio

```
```

Comparação de todos os casos no mesmo plot com escala

```
```



### **3. Mostre numericamente com suas implementações dos algoritmos de  insertion-sort e merge-sort como se comporta o desempenho de cada  algoritmo utilizando entradas de tamanho crescente, considerando  entradas de pior caso, melhor caso e caso médio.**

### **Análise, para cada tipo de entrada, se existe algum ponto a partir do qual um algoritmo passa a ser mais rápido que o outro.**

```
```

