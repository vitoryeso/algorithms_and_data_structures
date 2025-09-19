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
# Compilar benchmarks
make benchmark_matmul

# Executar benchmark
./bin/benchmark_matmul --algorithms naive,strassen --sizes 64 128 256 --runs 5

# Gerar gráficos
python scripts/plot_benchmarks.py results/*.csv --output analysis.png
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

