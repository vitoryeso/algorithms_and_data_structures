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
\begin{align}
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
\end{align}
$$


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

