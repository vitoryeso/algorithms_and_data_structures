# Sistema Unificado de Benchmarks

Este documento descreve o novo sistema unificado de benchmarks para algoritmos e estruturas de dados, que substitui os scripts antigos por uma interface mais robusta e consistente.

## 🚀 Novos Binários

### 1. `benchmark_matmul`
Benchmark para algoritmos de multiplicação de matrizes.

**Algoritmos suportados:**
- `naive` - Multiplicação ingênua O(n³)
- `strassen` - Algoritmo de Strassen O(n^2.81)
- `blocked` - Multiplicação por blocos com cache
- `blocked_opt` - Versão otimizada com cache L3

**Exemplo de uso:**
```bash
# Benchmark básico
./bin/benchmark_matmul --max-size 256 --runs 5

# Benchmark específico com saída CSV
./bin/benchmark_matmul --algorithms naive,strassen --sizes 64 128 256 \
                      --output-csv results/matmul.csv --quiet

# Valores grandes para testar overflow
./bin/benchmark_matmul --min-val -1000 --max-val 1000 --cutoff 128
```

### 2. `benchmark_sorting`
Benchmark para algoritmos de ordenação.

**Algoritmos suportados:**
- `selection_sort` - Selection Sort O(n²)
- `insertion_sort` - Insertion Sort O(n²)
- `quick_sort` - Quick Sort O(n log n)
- `merge_sort` - Merge Sort O(n log n)
- `counting_sort` - Counting Sort O(n+k)
- `bucket_sort` - Bucket Sort O(n+k)
- `radix_sort` - Radix Sort O(n*d)

**Padrões de entrada:**
- `ordered` - Vetor já ordenado
- `reverse` - Vetor em ordem reversa
- `random` - Vetor aleatório
- `nearly_sorted` - Quase ordenado

**Exemplo de uso:**
```bash
# Benchmark com diferentes padrões de entrada
./bin/benchmark_sorting --algorithms quick_sort,merge_sort \
                       --patterns ordered,random --max-size 10000

# Todos os algoritmos em vetor aleatório
./bin/benchmark_sorting --patterns random --output-csv results/sorting.csv
```

### 3. `benchmark_datastructures`
Benchmark para estruturas de dados.

**Estruturas suportadas:**
- `vector` - Vetor customizado
- `heap` - Heap binário
- `queue` - Fila encadeada

**Operações suportadas:**
- **Vector:** `push_back`, `access`, `insert_middle`
- **Heap:** `insert`, `extract_max`, `build_heap`
- **Queue:** `enqueue`, `dequeue`, `mixed`

**Exemplo de uso:**
```bash
# Benchmark de estruturas básicas
./bin/benchmark_datastructures --data-structures vector,heap,queue \
                              --operations push_back,insert,enqueue \
                              --max-size 50000
```

## 📊 Sistema de Saída Unificado

### Formatos Suportados

**CSV (Long Format):**
```csv
size,algorithm,run_index,seed,time_s,cutoff,block_size,min_val,max_val,cpu_model,cpu_cores,compiler_version,os_info,timestamp,hostname
64,naive,0,42,0.001234,64,64,-10,10,Unknown,8,GCC 11.3.0,Linux,2024-01-01 12:00:00,localhost
```

**JSON (Structured):**
```json
{
  "metadata": {
    "cpu_model": "Unknown",
    "cpu_cores": 8,
    "compiler_version": "GCC 11.3.0",
    "os_info": "Linux",
    "timestamp": "2024-01-01 12:00:00",
    "hostname": "localhost"
  },
  "results": [
    {
      "size": 64,
      "algorithm": "naive",
      "run_index": 0,
      "seed": 42,
      "time_s": 0.001234,
      "cutoff": 64,
      "block_size": 64,
      "min_val": -10,
      "max_val": 10
    }
  ]
}
```

## 🎨 Sistema de Visualização Unificado

### `plot_benchmarks.py`
Script Python unificado para gerar gráficos comparativos.

**Características:**
- Auto-detecção do tipo de benchmark
- Curvas teóricas automáticas
- Suporte a múltiplos arquivos
- Anotações e legendas inteligentes

**Exemplos:**
```bash
# Plotar resultados de multiplicação de matrizes
python3 scripts/plot_benchmarks.py results/matmul_*.csv --output matmul_plot.png

# Plotar algoritmos de ordenação
python3 scripts/plot_benchmarks.py results/sorting_*.csv --output sorting_plot.png

# Combinar múltiplos tipos
python3 scripts/plot_benchmarks.py results/*.csv --output-dir plots/
```

## 🏗️ Makefile Atualizado

### Novos Alvos
```bash
# Compilar benchmarks
make benchmark_matmul
make benchmark_sorting
make benchmark_datastructures
make all  # Compila todos

# Executar benchmarks rapidamente
make bench_matmul      # Tamanho pequeno, saída timestamped
make bench_sorting     # Tamanho médio
make bench_datastructures
make bench_all         # Todos os benchmarks

# Gerar gráficos
make plots             # Gera plots de todos os resultados em results/
```

## 🚀 Script de Execução Unificado

### `run_benchmarks.sh`
Script shell atualizado com suporte completo aos novos binários.

**Variáveis de ambiente:**
```bash
export RUNS=5                          # Execuções por teste
export MATMUL_MAX_SIZE=512             # Tamanho máximo para matmul
export SORTING_MAX_SIZE=50000          # Tamanho máximo para sorting
export DS_MAX_SIZE=100000              # Tamanho máximo para DS
export BENCHMARK_TYPE=all              # matmul, sorting, datastructures, all
export GENERATE_PLOTS=true            # Gerar plots automaticamente
```

**Exemplos:**
```bash
# Benchmark completo
./scripts/run_benchmarks.sh

# Apenas multiplicação de matrizes
BENCHMARK_TYPE=matmul ./scripts/run_benchmarks.sh

# Com plots automáticos
GENERATE_PLOTS=true ./scripts/run_benchmarks.sh
```

## 🔄 Migração dos Scripts Antigos

### Scripts Deprecados
Os seguintes scripts foram substituídos pelos novos binários:

| Script Antigo | Novo Comando |
|---------------|--------------|
| `benchmark_cpp.cpp` | `./bin/benchmark_matmul` |
| `benchmark_matmul.py` | `./bin/benchmark_matmul` |
| `benchmark_update.py` | `./bin/benchmark_matmul` |
| `docker_benchmark.py` | `./bin/benchmark_matmul` |

### Equivalências de Comando

**Antigo:**
```bash
python3 scripts/benchmark_matmul.py --sizes 64 128 256 --runs 5
```

**Novo:**
```bash
./bin/benchmark_matmul --sizes 64 128 256 --runs 5
```

**Antigo:**
```bash
python3 scripts/docker_benchmark.py --test-case small --max-size 1024
```

**Novo:**
```bash
./bin/benchmark_matmul --max-size 1024 --min-val -10 --max-val 10
```

## ✅ Testes de Correção

### `benchmark_correctness_test.cc`
Testes GTest paramétricos que validam a correção entre algoritmos:

- **Matrizes:** Todos os algoritmos produzem o mesmo resultado
- **Ordenação:** Todos os algoritmos produzem vetores ordenados
- **Estruturas:** Operações básicas funcionam corretamente

**Executar testes:**
```bash
make test_all
./bin/test_runner --gtest_filter="*Correctness*"
```

## 📈 Melhorias do Sistema Unificado

### ✅ Benefícios Implementados
- **CLI consistente** entre todos os benchmarks
- **Schema unificado** para CSV/JSON
- **Metadados de ambiente** automáticos
- **Auto-detecção** de tipo de benchmark nos plots
- **Curvas teóricas** automáticas
- **Testes de correção** abrangentes
- **Suporte a Docker** mantido
- **Documentação** completa

### 🔄 Compatibilidade
- Scripts antigos ainda funcionam
- Dados antigos podem ser convertidos
- Interface familiar mantida onde possível

### 🎯 Casos de Uso
1. **Desenvolvimento:** Testes rápidos de performance
2. **Comparação:** Benchmarks lado a lado de algoritmos
3. **Análise:** Plots detalhados com curvas teóricas
4. **CI/CD:** Execução automatizada em containers
5. **Pesquisa:** Dados estruturados para análise

## 🐛 Troubleshooting

### Problemas Comuns

**Erro de compilação:**
```bash
# Verificar dependências
make clean && make all

# Verificar includes
find . -name "*.h" -exec grep -l "Matrix" {} \;
```

**Plots não geram:**
```bash
# Instalar matplotlib
pip3 install matplotlib numpy pandas

# Verificar dados
head -5 results/*.csv
```

**Benchmarks lentos:**
```bash
# Usar --quiet para menos output
# Reduzir --runs para testes rápidos
# Usar tamanhos menores para desenvolvimento
```

### Suporte
Para questões sobre o novo sistema, consulte:
1. Este documento (`BENCHMARKS_README.md`)
2. Código fonte dos binários em `scripts/`
3. Testes em `tests/benchmark_correctness_test.cc`
4. Exemplos no `run_benchmarks.sh`
