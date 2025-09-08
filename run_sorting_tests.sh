#!/bin/bash

echo "Compilando testes de ordenação..."
g++ -pthread ./tests/sorting_algorithms_test.cc ./algorithms/cpp/luAlgorithms.cpp -lgtest -lgtest_main -o ./bin/sorting_tests

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    echo "Executando testes de ordenação..."
    echo "=========================================="
    
    # Executar com flags do Google Test:
    # --gtest_continue_on_failure: continua executando mesmo se algum teste falhar
    # --gtest_print_time=1: mostra tempo de execução
    # --gtest_color=yes: usa cores na saída
    ./bin/sorting_tests --gtest_continue_on_failure --gtest_print_time=1 --gtest_color=yes
else
    echo "Erro na compilação!"
    exit 1
fi
