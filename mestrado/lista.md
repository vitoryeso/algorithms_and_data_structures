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

```
```

código anotado

```
```

Análise para o melhor e pior caso

```
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

