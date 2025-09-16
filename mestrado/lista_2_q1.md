### **4. Mostre numericamente com suas implementações dos algoritmos de multiplicação de matrizes que o algoritmo de Strassen é mais rápido que o algoritmo convencional.**

Strassen reduz a árvore de recursão: em vez de 8 multiplicações recursivas por nível, realiza 7 multiplicações e compensa com algumas somas/subtrações. Esse trade-off diminui a complexidade assintótica de O(n^3) para O(n^2.81).

- **Implementação**: C++ com `matmul_naive` e `matmul_strassen(cutoff=64)`. Onde o cutoff é o tamanho mínimo para usar matmul_naive como "fallback"
- **Medições**: script `benchmark_matmul.py` (múltiplas execuções por tamanho, média) e gráfico com curvas teóricas O(n^3) e O(n^{2.81}) para comparação.
- **Resultado**: como esperado, a curva de Strassen cresce mais lentamente; em tamanhos pequenos a diferença pode ser sutil pelo overhead de somas, mas a tendência favorece Strassen à medida que (n) cresce.

> Observação: o ideal é testar com valores maiores de n para evidenciar melhor a diferença; na figura atual foram incluídos tamanhos até 2048.

<img src="../matmul_benchmark_plot.png" alt="Benchmark Strassen vs Convencional" width="2000">

**Vítor Yeso Fidelis Freitas - Programa de Pós Graduação em Engenharia Elétrica e Computação - 2025.2**
