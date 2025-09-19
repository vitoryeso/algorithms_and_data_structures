# 📋 Análise de Scripts Obsoletos - Sistema Unificado de Benchmarks

## 🎯 **Status Atual**

O sistema unificado de benchmarks foi implementado com sucesso, tornando vários scripts antigos **obsoletos** ou **necessitando refatoração**.

## 📁 **Scripts Analisados**

### 🚫 **Scripts de Benchmark (Python) - CANDIDATOS À REMOÇÃO**

#### 1. `scripts/benchmark_matmul.py` ❌ **REMOVER**
- **Status:** Completamente substituído pelo `scripts/benchmark_matmul.cpp`
- **Razões:**
  - Funcionalidade duplicada
  - Compila código C++ dinamicamente (ineficiente)
  - CLI limitada comparada ao binário C++
  - Não suporta múltiplos algoritmos simultaneamente
  - Não gera metadados de ambiente

#### 2. `scripts/benchmark_update.py` ❌ **REMOVER**
- **Status:** Versão aprimorada do `benchmark_matmul.py`
- **Razões:**
  - Ainda usa compilação dinâmica (problema de performance)
  - CLI menos robusta que o binário C++
  - Não integrado ao sistema unificado
  - Funcionalidades já cobertas pelo `benchmark_matmul.cpp`

#### 3. `scripts/docker_benchmark.py` ❌ **REMOVER**
- **Status:** Script específico para Docker
- **Razões:**
  - Funcionalidade integrada ao `run_benchmarks.sh`
  - CLI limitada
  - Não suporta o novo schema de dados
  - Scripts shell são mais apropriados para automação Docker

### 🚫 **Scripts de Plotting - CANDIDATOS À REMOÇÃO**

#### 4. `scripts/plot_benchmark.py` ❌ **REMOVER**
- **Status:** Substituído pelo `scripts/plot_benchmarks.py`
- **Razões:**
  - Detecção manual de tipo de benchmark
  - Suporte limitado a múltiplos arquivos
  - Não detecta automaticamente algoritmos
  - CLI menos intuitiva

#### 5. `scripts/plot_comparison.py` ❌ **REMOVER**
- **Status:** Script específico para sorting (insertion vs merge)
- **Razões:**
  - Funcionalidade específica demais
  - Dados hardcoded (insertion_sort_detailed_results.csv)
  - Não extensível para outros tipos de benchmark
  - Substituído pelo sistema unificado de plotting

#### 6. `scripts/plot_insertion_results.py` ❌ **REMOVER**
- **Status:** Plot específico para insertion sort
- **Razões:**
  - Muito específico (hardcoded)
  - Não extensível
  - Funcionalidade coberta pelo sistema unificado

#### 7. `scripts/plot_merge_results.py` ❌ **REMOVER**
- **Status:** Plot específico para merge sort
- **Razões:**
  - Mesmo problema que `plot_insertion_results.py`
  - Muito específico
  - Não integrado ao sistema unificado

### 🤔 **Scripts de Benchmark (C++) - MANTER COM REFATORAÇÃO**

#### 8. `scripts/benchmark_cpp.cpp` ⚠️ **REFATORAR**
- **Status:** Benchmark legado de multiplicação de matrizes
- **Recomendação:** Manter como backup/compatibilidade
- **Razões para manter:**
  - Pode ter funcionalidades específicas não cobertas
  - Algumas pessoas podem estar usando
  - Baixo custo de manutenção

#### 9. `benchmark_visualizer/benchmark.cpp` ⚠️ **REFATORAR/AVALIAR**
- **Status:** Script com OpenCV para visualização interativa
- **Recomendação:** Avaliar utilidade
- **Razões:**
  - Usa OpenCV (dependência pesada)
  - Interface interativa diferente do foco atual
  - Pode ser útil para debugging específico

### ✅ **Scripts Mantidos - SISTEMA UNIFICADO**

#### 10. `scripts/benchmark_matmul.cpp` ✅ **MANTER**
- **Status:** Parte do sistema unificado
- **Razões:** Core do sistema de benchmarks

#### 11. `scripts/benchmark_sorting.cpp` ✅ **MANTER**
- **Status:** Parte do sistema unificado
- **Razões:** Core do sistema de benchmarks

#### 12. `scripts/benchmark_datastructures.cpp` ✅ **MANTER**
- **Status:** Parte do sistema unificado
- **Razões:** Core do sistema de benchmarks

#### 13. `scripts/plot_benchmarks.py` ✅ **MANTER**
- **Status:** Sistema unificado de plotting
- **Razões:** Core do sistema de visualização

#### 14. `scripts/run_benchmarks.sh` ✅ **MANTER**
- **Status:** Atualizado para o sistema unificado
- **Razões:** Interface principal para automação

## 🗑️ **Plano de Limpeza Recomendado**

### **FASE 1: Remoção Imediata (Scripts Completamente Obsoletos)**

```bash
# Scripts Python de benchmark - REMOVER
rm scripts/benchmark_matmul.py
rm scripts/benchmark_update.py
rm scripts/docker_benchmark.py

# Scripts de plotting específicos - REMOVER
rm scripts/plot_benchmark.py
rm scripts/plot_comparison.py
rm scripts/plot_insertion_results.py
rm scripts/plot_merge_results.py
```

### **FASE 2: Avaliação (Scripts com Possível Utilidade)**

```bash
# Avaliar se ainda é usado
# Se sim: mover para pasta deprecated/
# Se não: remover
mv benchmark_visualizer/benchmark.cpp deprecated/
```

### **FASE 3: Compatibilidade (Scripts Legado)**

```bash
# Manter como backup, mas documentar como deprecated
echo "DEPRECATED: Use ./bin/benchmark_matmul instead" > scripts/benchmark_cpp.cpp.README
```

## 📊 **Impacto da Limpeza**

### **Benefícios:**
- ✅ **Redução de complexidade:** Menos arquivos para manter
- ✅ **Clareza:** Sistema unificado mais fácil de entender
- ✅ **Manutenibilidade:** Menos código duplicado
- ✅ **Performance:** Binários C++ mais eficientes que scripts Python

### **Riscos:**
- ⚠️ **Quebra de compatibilidade:** Alguns usuários podem estar usando scripts antigos
- ⚠️ **Perda de funcionalidades:** Alguns scripts podem ter features únicas

### **Mitigação:**
- 📝 **Documentação:** README.md atualizado com guia de migração
- 🔄 **Período de transição:** Avisos de depreciação
- 💾 **Backup:** Arquivos movidos para `deprecated/` antes da remoção

## 🎯 **Recomendação Final**

**Executar FASE 1 imediatamente:**
- Remover scripts Python de benchmark completamente obsoletos
- Remover scripts de plotting específicos

**Avaliar FASE 2 em 1 mês:**
- Verificar uso do benchmark_visualizer
- Decidir sobre benchmark_cpp.cpp

**Implementar FASE 3:**
- Adicionar avisos de depreciação
- Atualizar documentação

**Resultado esperado:** Redução de ~60% nos arquivos de script, com sistema muito mais limpo e manutenível.
