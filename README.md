# Algorithms and Data Structures
exercises of book Introduction to Algorithms by Thomas Cormen and homeworks from DCA0208 - UFRN

## 🚀 Sistema Unificado de Benchmarks

Este projeto inclui um **sistema completo de benchmarks** para algoritmos e estruturas de dados:

### Benchmarks Disponíveis
- ✅ **benchmark_matmul** - Multiplicação de matrizes (naive, strassen, blocked, blocked_opt)
- ✅ **benchmark_sorting** - Algoritmos de ordenação com padrões de entrada variados
- ✅ **benchmark_datastructures** - Estruturas de dados (vector, heap, queue)

### Recursos do Sistema
- 🎯 **CLI Robusta** - Interface consistente entre todos os benchmarks
- 📊 **Schema Padronizado** - CSV/JSON com metadados de ambiente
- 📈 **Visualização Inteligente** - Plots automáticos com curvas teóricas
- 🔧 **Automação Completa** - Scripts para execução e análise
- 🧪 **Testes de Correção** - Validação paramétrica entre algoritmos

### Uso Rápido
```bash
# Compilar todos os benchmarks
make all

# === BENCHMARK DE MULTIPLICAÇÃO DE MATRIZES ===
# Executar benchmark
./bin/benchmark_matmul --algorithms naive,strassen,blocked,blocked_opt --max-size 256 --runs 3 --output-csv results/matmul_results.csv

# === BENCHMARK DE ALGORITMOS DE ORDENAÇÃO ===
# Executar benchmark  
./bin/benchmark_sorting --algorithms selection_sort,insertion_sort,quick_sort,merge_sort --patterns ordered,reverse,random,nearly_sorted --max-size 10000 --runs 3 --output-csv results/sorting_results.csv

# === BENCHMARK DE ESTRUTURAS DE DADOS ===
# Executar benchmark (apenas STL para tamanhos grandes)
./bin/benchmark_datastructures --data-structures vector,heap,queue --max-size 50000 --runs 3 --output-csv results/datastructures_results.csv

# Ou testar implementações customizadas (tamanhos menores)
./bin/benchmark_datastructures --data-structures vector,heap,queue,my_vector,my_heap,my_queue_linkedlist,my_queue_vector,my_stack --max-size 10000 --runs 3 --output-csv results/datastructures_custom.csv

# === GERAR GRÁFICOS ===
# Gerar todos os plots automaticamente
python scripts/plot_benchmarks.py results/*.csv --output-dir plots_$(date +%Y%m%d_%H%M%S) --log-y

# Ou executar via make (roda benchmarks + plots)
make bench_all && make plots
```

### Documentação Completa
- 📖 `BENCHMARKS_README.md` - Guia completo do sistema
- 📋 `SCRIPTS_ANALYSIS.md` - Análise de scripts obsoletos
- 📁 `deprecated/` - Scripts antigos (mantidos temporariamente)

---

## 🧪 Tests
Debian-based distributions
```bash
sudo apt-get install libgtest-dev
make test_all
```

Arch-based distributions
```bash
sudo pacman -S gtest
make test_all
```

---

**Nota:** Muitos scripts antigos foram movidos para `deprecated/` como parte da unificação do sistema de benchmarks. Consulte `deprecated/README.md` para detalhes da migração.

