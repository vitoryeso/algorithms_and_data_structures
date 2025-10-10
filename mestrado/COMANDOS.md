# 📋 Comandos para Execução das Questões

## ☕ Enquanto você toma seu café...

Criei toda a infraestrutura! Você só precisa implementar os corpos das funções nos arquivos `.cpp`.

---

## 🚀 Comandos Principais

### Compilar Uma Questão Específica
```bash
make benchmark_q10    # Questão 10: Min/Max Simultâneo
make benchmark_q11    # Questão 11: Seleção
make benchmark_q12    # Questão 12: Mediana Ponderada
make benchmark_q13    # Questão 13: Estruturas de Busca
make benchmark_q14    # Questão 14: Hash Tables
make benchmark_q15    # Questão 15: Hash Probing
```

### Compilar Todas as Questões
```bash
make all_questoes
```

---

## 🧪 Testes Unitários

### Compilar e Rodar Testes
```bash
# Uma questão específica
make test_q10
./bin/test_q10

# Ou em uma linha
make test_q10 && ./bin/test_q10
```

### Compilar Todos os Testes
```bash
make test_all_questoes
```

### Rodar Todos os Testes
```bash
./bin/test_q10
./bin/test_q11
./bin/test_q12
./bin/test_q13
./bin/test_q14
./bin/test_q15
```

---

## 📊 Executar Benchmarks

### Uma Questão Específica

#### Questão 10 (Min/Max Simultâneo)
```bash
./bin/benchmark_q10 --max-size 10000000 --runs 5 --output-csv reports/q10_results.csv
```

#### Questão 11 (Seleção)
```bash
./bin/benchmark_q11 --max-size 1000000 --runs 5 --output-csv reports/q11_results.csv
```

#### Questão 12 (Mediana Ponderada)
```bash
./bin/benchmark_q12 --max-size 100000 --runs 5 --output-csv reports/q12_results.csv
```

#### Questão 13 (Estruturas de Busca)
```bash
./bin/benchmark_q13 --max-size 100000 --runs 3 --output-csv reports/q13_results.csv
```

#### Questão 14 (Hash Tables)
```bash
./bin/benchmark_q14 --max-size 100000 --runs 3 --output-csv reports/q14_results.csv
```

#### Questão 15 (Hash Probing)
```bash
./bin/benchmark_q15 --max-size 100000 --runs 3 --output-csv reports/q15_results.csv
```

### Todas as Questões de Uma Vez
```bash
make run_all_questoes
```
Este comando:
1. Compila todos os benchmarks
2. Roda cada um com parâmetros otimizados
3. Gera gráficos automaticamente
4. Salva tudo em `reports/` e `mestrado/plots_questoes/`

---

## 📈 Gerar Gráficos

### Manualmente
```bash
python3 mestrado/common/plot_questoes.py reports/q10_results.csv
```

### Para Todos os Resultados
```bash
python3 mestrado/common/plot_questoes.py reports/q1*_results.csv
```

Gráficos são salvos em: `mestrado/plots_questoes/`

---

## 🔧 Workflow Recomendado

### Para Cada Questão:

1. **Implementar** os corpos das funções no `.cpp`
   ```bash
   # Por exemplo, para Q10:
   vim mestrado/questao_10_minmax/minmax.cpp
   ```

2. **Testar** se compila
   ```bash
   make benchmark_q10
   ```

3. **Rodar testes unitários**
   ```bash
   make test_q10 && ./bin/test_q10
   ```

4. **Ajustar** até todos os testes passarem

5. **Executar benchmark**
   ```bash
   ./bin/benchmark_q10 --max-size 1000000 --runs 3 --output-csv reports/q10_test.csv
   ```

6. **Gerar gráficos**
   ```bash
   python3 mestrado/common/plot_questoes.py reports/q10_test.csv
   ```

7. **Verificar** resultados em `mestrado/plots_questoes/`

8. **Repetir** para próxima questão

---

## 🛠️ Comandos Úteis

### Ver Ajuda de Um Benchmark
```bash
./bin/benchmark_q10 --help
```

### Modo Silencioso (menos output)
```bash
./bin/benchmark_q10 --max-size 100000 --runs 3 --output-csv reports/q10.csv --quiet
```

### Teste Rápido (tamanhos pequenos)
```bash
./bin/benchmark_q10 --max-size 10000 --runs 1 --output-csv reports/q10_quick.csv
```

### Limpar Binários
```bash
make clean
```

### Limpar Tudo (binários + reports)
```bash
make clear
```

---

## 📂 Estrutura de Arquivos Criada

```
mestrado/
├── common/
│   ├── questao_utils.h           ✅ Pronto
│   ├── questao_utils.cpp         ✅ Pronto
│   └── plot_questoes.py          ⏳ A criar
│
├── questao_10_minmax/
│   ├── minmax.h                  ✅ Pronto
│   ├── minmax.cpp                🔴 VOCÊ IMPLEMENTA
│   ├── benchmark_minmax.cpp      ⏳ A criar
│   └── test_minmax.cc            ⏳ A criar
│
├── questao_11_selection/
│   ├── selection.h               ✅ Pronto
│   ├── selection.cpp             🔴 VOCÊ IMPLEMENTA
│   ├── benchmark_selection.cpp   ⏳ A criar
│   └── test_selection.cc         ⏳ A criar
│
├── questao_12_weighted_median/
│   ├── weighted_median.h         ✅ Pronto
│   ├── weighted_median.cpp       🔴 VOCÊ IMPLEMENTA
│   ├── benchmark_*.cpp           ⏳ A criar
│   └── test_*.cc                 ⏳ A criar
│
├── questao_13_search_structures/
│   ├── sorted_array.h/.cpp       ✅/🔴 Pronto/Você implementa
│   ├── linked_list.h/.cpp        ✅/🔴 Pronto/Você implementa
│   ├── avl_tree.h/.cpp           ✅/🔴 Pronto/Você implementa
│   ├── benchmark_*.cpp           ⏳ A criar
│   └── test_*.cc                 ⏳ A criar
│
├── questao_14_hash_tables/
│   ├── hash_chaining.h/.cpp      ✅/🔴 Pronto/Você implementa
│   ├── hash_open_addressing.h/.cpp ✅/🔴 Pronto/Você implementa
│   ├── benchmark_*.cpp           ⏳ A criar
│   └── test_*.cc                 ⏳ A criar
│
├── questao_15_hash_probing/
│   ├── hash_linear_probing.h/.cpp    ✅/🔴 Pronto/Você implementa
│   ├── hash_double_hashing.h/.cpp    ✅/🔴 Pronto/Você implementa
│   ├── benchmark_*.cpp           ⏳ A criar
│   └── test_*.cc                 ⏳ A criar
│
├── QUESTOES_README.md            ✅ Pronto
└── COMANDOS.md                   ✅ Este arquivo
```

**Legenda**:
- ✅ Pronto: Interface completa, você só implementa corpos
- 🔴 VOCÊ IMPLEMENTA: Seu trabalho
- ⏳ A criar: Vou criar benchmarks e testes ainda

---

## 📝 Próximos Passos

1. ✅ **Interfaces `.h` criadas** - Todas prontas!
2. ✅ **Esqueletos `.cpp` criados** - Com TODOs e dicas
3. ⏳ **Criar benchmarks** - Vou fazer enquanto você implementa
4. ⏳ **Criar testes unitários** - Vou fazer também
5. ⏳ **Script de plotting** - Vou criar
6. ✅ **Makefile atualizado** - Pronto!
7. 🔴 **Você implementa** as funções nos `.cpp`

---

## 💡 Dicas

- Comece pela **Questão 10** (mais simples)
- Leia os comentários nos `.h` - têm algoritmos detalhados
- Use os TODOs nos `.cpp` como guia
- Testes vão te ajudar a validar corretude
- Benchmarks vão gerar os gráficos automaticamente

---

## 🆘 Em Caso de Erro

### Erro de Compilação
```bash
# Verifica se arquivo está correto
cat mestrado/questao_10_minmax/minmax.cpp

# Compila com warnings detalhados
g++ -std=c++17 -Wall -Wextra -c mestrado/questao_10_minmax/minmax.cpp
```

### Teste Falha
- Leia a mensagem de erro do Google Test
- Verifica lógica da implementação
- Adiciona prints para debug
- Roda com gdb se necessário

### Benchmark Não Roda
```bash
# Verifica se binário existe
ls -la ./bin/benchmark_q10

# Testa com tamanho pequeno primeiro
./bin/benchmark_q10 --max-size 100 --runs 1
```

---

## 📚 Documentação

- **README detalhado**: `mestrado/QUESTOES_README.md`
- **Interfaces completas**: Arquivos `.h` de cada questão
- **Referências**: Cormen 4ª Ed., capítulos citados nos `.h`

---

**Bom café e boa implementação!** ☕🚀

Quando voltar, é só começar a implementar os `.cpp`. Tudo o resto está pronto!

