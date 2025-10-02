# Google Test (gtest) - Guia de Flags Úteis para InovAI Labs

## 📋 Visão Geral
Este documento apresenta as flags mais úteis do Google Test para desenvolvimento e testes automatizados, especialmente relevantes para projetos de IA e algoritmos.

## 🚀 Flags Essenciais para Desenvolvimento

### 1. **Controle de Execução de Testes**

```bash
# Executar apenas testes específicos
--gtest_filter="TestSuite.TestName"
--gtest_filter="TestSuite.*"                    # Todos os testes de uma suite
--gtest_filter="*TestName*"                     # Testes que contenham "TestName"

# Exemplos práticos:
./bin/tests --gtest_filter="SortingAlgorithmsTest.SelectionSort"
./bin/tests --gtest_filter="SortingAlgorithmsTest.*"
./bin/tests --gtest_filter="*Sort*"
```

### 2. **Controle de Falhas e Continuidade**

```bash
# Continuar executando mesmo com falhas
--gtest_break_on_failure=false

# Executar testes desabilitados
--gtest_also_run_disabled_tests

# Repetir testes N vezes
--gtest_repeat=3

# Embaralhar ordem dos testes
--gtest_shuffle
```

### 3. **Saída e Relatórios**

```bash
# Mostrar tempo de execução
--gtest_print_time=1

# Cores na saída (yes/no/auto)
--gtest_color=yes

# Apenas mostrar falhas
--gtest_brief=1

# Gerar relatório XML
--gtest_output=xml:test_results.xml

# Gerar relatório JSON
--gtest_output=json:test_results.json
```

### 4. **Debugging e Desenvolvimento**

```bash
# Listar todos os testes disponíveis
--gtest_list_tests

# Executar em modo verbose
--gtest_verbose

# Definir seed para testes aleatórios
--gtest_random_seed=42
```

## 🎯 Configurações Recomendadas por Cenário

### **Desenvolvimento Local**
```bash
./bin/tests --gtest_color=yes --gtest_print_time=1 --gtest_break_on_failure=false
```

### **CI/CD Pipeline**
```bash
./bin/tests --gtest_output=xml:results.xml --gtest_print_time=1 --gtest_color=no
```

### **Debug de Testes Específicos**
```bash
./bin/tests --gtest_filter="ProblematicTest.*" --gtest_break_on_failure=true --gtest_verbose
```

### **Testes de Performance**
```bash
./bin/tests --gtest_filter="*Performance*" --gtest_print_time=1 --gtest_repeat=5
```
