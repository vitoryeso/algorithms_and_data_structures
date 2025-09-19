# 📁 Scripts Deprecated - Sistema Unificado de Benchmarks

## ⚠️ **ATENÇÃO: Estes arquivos são obsoletos!**

Estes scripts foram **movidos para esta pasta** porque foram **substituídos** pelo novo **sistema unificado de benchmarks**.

## 📋 **Arquivos Movidos**

### 🚫 **Scripts de Benchmark (Python) - SUBSTITUÍDOS**

#### 1. `benchmark_matmul.py` ❌
- **Substituído por:** `scripts/benchmark_matmul.cpp`
- **Razões:**
  - Compilação dinâmica ineficiente
  - CLI limitada
  - Não suporta múltiplos algoritmos
  - Sem metadados de ambiente

#### 2. `benchmark_update.py` ❌
- **Substituído por:** `scripts/benchmark_matmul.cpp`
- **Razões:**
  - Mesmo problema que benchmark_matmul.py
  - Funcionalidades já integradas no sistema unificado

#### 3. `docker_benchmark.py` ❌
- **Substituído por:** `scripts/run_benchmarks.sh`
- **Razões:**
  - Funcionalidade integrada no script shell
  - CLI menos robusta
  - Não usa novo schema de dados

### 🚫 **Scripts de Plotting - SUBSTITUÍDOS**

#### 4. `plot_benchmark.py` ❌
- **Substituído por:** `scripts/plot_benchmarks.py`
- **Razões:**
  - Detecção manual de tipo de benchmark
  - Suporte limitado a múltiplos arquivos

#### 5. `plot_comparison.py` ❌
- **Substituído por:** `scripts/plot_benchmarks.py`
- **Razões:**
  - Muito específico (hardcoded)
  - Não extensível

#### 6. `plot_insertion_results.py` ❌
- **Substituído por:** `scripts/plot_benchmarks.py`
- **Razões:**
  - Específico demais
  - Dados hardcoded

#### 7. `plot_merge_results.py` ❌
- **Substituído por:** `scripts/plot_benchmarks.py`
- **Razões:**
  - Mesmo problema dos outros scripts específicos

### 🤔 **Scripts com OpenCV - AVALIAÇÃO PENDENTE**

#### 8. `benchmark_visualizer/benchmark.cpp` ❓
- **Status:** Mantido temporariamente
- **Razões para manter:**
  - Interface visual interativa (OpenCV)
  - Pode ser útil para debugging específico
- **Recomendação:** Avaliar uso em 1 mês

## 🎯 **Como Migrar**

### **Para Benchmark de Multiplicação de Matrizes:**

```bash
# ❌ ANTIGO (deprecated)
python3 scripts/benchmark_matmul.py --sizes 64 128 256

# ✅ NOVO (recomendado)
./bin/benchmark_matmul --algorithms naive,strassen --sizes 64 128 256 --runs 5
```

### **Para Plotting de Resultados:**

```bash
# ❌ ANTIGO (deprecated)
python3 scripts/plot_benchmark.py results.csv

# ✅ NOVO (recomendado)
python scripts/plot_benchmarks.py results/*.csv --output analysis.png
```

### **Para Execução em Docker:**

```bash
# ❌ ANTIGO (deprecated)
python3 scripts/docker_benchmark.py --test-case small

# ✅ NOVO (recomendado)
BENCHMARK_TYPE=matmul ./scripts/run_benchmarks.sh
```

## 📚 **Sistema Unificado - Documentação**

Para informações completas sobre o novo sistema, consulte:

- `BENCHMARKS_README.md` - Documentação completa
- `SCRIPTS_ANALYSIS.md` - Análise detalhada da migração
- `scripts/run_benchmarks.sh --help` - Ajuda dos scripts

## 🗑️ **Próximos Passos**

1. **Testar** o sistema unificado completamente
2. **Atualizar** documentação principal
3. **Remover** definitivamente estes arquivos em 3 meses
4. **Manter** apenas `benchmark_visualizer` se for útil

## 📞 **Suporte**

Se você precisa usar algum destes scripts:

1. **Verifique** se a funcionalidade existe no sistema unificado
2. **Migre** para os novos scripts
3. **Abra uma issue** se algo estiver faltando

---

**Data de depreciação:** Outubro 2025
**Data de remoção prevista:** Janeiro 2026
