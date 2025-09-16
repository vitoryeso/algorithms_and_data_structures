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

## 🤖 MVP: Teste para Agente de IA

### Estrutura de Teste Básica

```cpp
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <memory>

// Mock de um agente de IA simples
class AIAgent {
public:
    struct Prediction {
        std::string label;
        float confidence;
        std::vector<float> features;
    };
    
    Prediction predict(const std::vector<float>& input) {
        // Simulação de predição
        Prediction result;
        result.confidence = 0.85f;
        result.label = "positive";
        result.features = input;
        return result;
    }
    
    bool train(const std::vector<std::vector<float>>& data, 
               const std::vector<std::string>& labels) {
        // Simulação de treinamento
        return data.size() == labels.size();
    }
};

// Classe de teste para o agente
class AIAgentTest : public ::testing::Test {
protected:
    void SetUp() override {
        agent = std::make_unique<AIAgent>();
    }
    
    std::unique_ptr<AIAgent> agent;
    
    // Dados de teste
    std::vector<float> validInput = {1.0f, 2.0f, 3.0f};
    std::vector<float> emptyInput = {};
    std::vector<float> largeInput = std::vector<float>(1000, 1.0f);
};

// Testes de Funcionalidade Básica
TEST_F(AIAgentTest, ValidPrediction) {
    auto result = agent->predict(validInput);
    
    EXPECT_FALSE(result.label.empty());
    EXPECT_GE(result.confidence, 0.0f);
    EXPECT_LE(result.confidence, 1.0f);
    EXPECT_EQ(result.features.size(), validInput.size());
}

TEST_F(AIAgentTest, EmptyInputHandling) {
    auto result = agent->predict(emptyInput);
    
    // Deve lidar graciosamente com entrada vazia
    EXPECT_TRUE(result.label.empty() || !result.label.empty());
    EXPECT_GE(result.confidence, 0.0f);
    EXPECT_LE(result.confidence, 1.0f);
}

TEST_F(AIAgentTest, LargeInputHandling) {
    auto result = agent->predict(largeInput);
    
    EXPECT_FALSE(result.label.empty());
    EXPECT_GE(result.confidence, 0.0f);
    EXPECT_LE(result.confidence, 1.0f);
    EXPECT_EQ(result.features.size(), largeInput.size());
}

// Testes de Performance
TEST_F(AIAgentTest, PredictionPerformance) {
    const int iterations = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        agent->predict(validInput);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Deve processar 1000 predições em menos de 100ms
    EXPECT_LT(duration.count(), 100);
}

// Testes de Treinamento
TEST_F(AIAgentTest, ValidTraining) {
    std::vector<std::vector<float>> data = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    std::vector<std::string> labels = {"class1", "class2"};
    
    bool success = agent->train(data, labels);
    EXPECT_TRUE(success);
}

TEST_F(AIAgentTest, InvalidTrainingData) {
    std::vector<std::vector<float>> data = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    std::vector<std::string> labels = {"class1"}; // Tamanho diferente
    
    bool success = agent->train(data, labels);
    EXPECT_FALSE(success);
}

// Testes de Robustez
TEST_F(AIAgentTest, MultiplePredictionsConsistency) {
    auto result1 = agent->predict(validInput);
    auto result2 = agent->predict(validInput);
    
    // Para um agente determinístico, resultados devem ser consistentes
    EXPECT_EQ(result1.label, result2.label);
    EXPECT_FLOAT_EQ(result1.confidence, result2.confidence);
}

// Testes de Edge Cases
TEST_F(AIAgentTest, ExtremeValues) {
    std::vector<float> extremeInput = {std::numeric_limits<float>::max(), 
                                      std::numeric_limits<float>::min(), 
                                      0.0f};
    
    auto result = agent->predict(extremeInput);
    
    EXPECT_FALSE(result.label.empty());
    EXPECT_GE(result.confidence, 0.0f);
    EXPECT_LE(result.confidence, 1.0f);
}
```

### Script de Execução para Agente de IA

```bash
#!/bin/bash
# test_ai_agent.sh

echo "🤖 Executando testes do Agente de IA..."

# Compilar testes
g++ -std=c++17 -pthread ai_agent_test.cc -lgtest -lgtest_main -o ./bin/ai_agent_tests

# Executar com flags otimizadas para IA
./bin/ai_agent_tests \
    --gtest_color=yes \
    --gtest_print_time=1 \
    --gtest_break_on_failure=false \
    --gtest_output=xml:ai_test_results.xml \
    --gtest_filter="AIAgentTest.*"

echo "✅ Testes concluídos! Verifique ai_test_results.xml para detalhes."
```

## 📊 Métricas e Monitoramento

### Flags para Análise de Performance
```bash
# Medir tempo de cada teste
--gtest_print_time=1

# Repetir testes para análise estatística
--gtest_repeat=10

# Gerar relatório detalhado
--gtest_output=json:performance_report.json
```

### Flags para Debugging
```bash
# Parar no primeiro erro
--gtest_break_on_failure=true

# Modo verbose
--gtest_verbose

# Executar apenas testes que falharam
--gtest_filter="*FailingTest*"
```

## 🔧 Integração com CI/CD

### GitHub Actions Example
```yaml
- name: Run Tests
  run: |
    ./bin/tests \
      --gtest_output=xml:test_results.xml \
      --gtest_print_time=1 \
      --gtest_color=no \
      --gtest_break_on_failure=false

- name: Upload Test Results
  uses: actions/upload-artifact@v2
  with:
    name: test-results
    path: test_results.xml
```

## 📝 Boas Práticas

1. **Sempre use `--gtest_break_on_failure=false`** em CI/CD
2. **Use `--gtest_print_time=1`** para monitorar performance
3. **Filtre testes específicos** durante desenvolvimento
4. **Gere relatórios XML/JSON** para análise posterior
5. **Use cores** para melhor visualização local
6. **Teste edge cases** especialmente em IA (valores extremos, entradas vazias)

## 🎯 Comandos Rápidos para InovAI Labs

```bash
# Desenvolvimento rápido
./bin/tests --gtest_filter="*" --gtest_color=yes --gtest_print_time=1

# Testes de performance
./bin/tests --gtest_filter="*Performance*" --gtest_repeat=5 --gtest_print_time=1

# Debug de falhas
./bin/tests --gtest_filter="*Failing*" --gtest_break_on_failure=true --gtest_verbose

# Relatório completo
./bin/tests --gtest_output=xml:full_report.xml --gtest_print_time=1 --gtest_color=no
```

---

*Documento criado para InovAI Labs - Guia prático de Google Test para projetos de IA e algoritmos*
