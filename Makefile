SRCS_ALG_CPP = algorithms/cpp/luAlgorithms.cpp
SRCS_DS_CPP = data_structures/cpp/graph_adj_list.cpp \
	data_structures/cpp/grafo_menor_caminho.cpp \
	data_structures/cpp/grafo_navegacao.cpp
SRCS_DS_C = data_structures/c/str_array.c \
	data_structures/c/edge_array.c \
	data_structures/c/worst_graph.c
SRCS_UTILS = utils/test_utils.cpp \
	utils/graph_utils.cpp \
	utils/utils.cpp
SRCS_TESTS = $(wildcard tests/*.cc)

LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lgtest -lgtest_main -lpthread
CXXFLAGS = -std=c++17 -O3 -march=native -funroll-loops -Wall -I. -Ialgorithms/cpp -Idata_structures/cpp -Idata_structures/c -Iutils -I/usr/include/gtest -I/usr/include/gtest/internal -pthread

all: benchmark_matmul benchmark_sorting benchmark_datastructures main_data_structures main_algorithms main_lista test_all

.PHONY: all clean clear bench_matmul bench_sorting bench_datastructures bench_all plots

# Novos benchmarks unificados
benchmark_matmul: ./scripts/benchmark_matmul.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_matmul ./scripts/benchmark_matmul.cpp $(SRCS_ALG_CPP)

benchmark_sorting: ./scripts/benchmark_sorting.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_sorting ./scripts/benchmark_sorting.cpp $(SRCS_ALG_CPP)

benchmark_quicksort: ./scripts/benchmark_quicksort.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_quicksort ./scripts/benchmark_quicksort.cpp $(SRCS_ALG_CPP)

benchmark_datastructures: ./scripts/benchmark_datastructures.cpp ./data_structures/cpp/vector.cpp ./data_structures/cpp/my_vector.cpp ./data_structures/cpp/heap.cpp ./data_structures/cpp/my_queue_linkedlist.cpp ./data_structures/cpp/my_queue_vector.cpp
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_datastructures ./scripts/benchmark_datastructures.cpp ./data_structures/cpp/vector.cpp ./data_structures/cpp/my_vector.cpp ./data_structures/cpp/heap.cpp ./data_structures/cpp/my_queue_linkedlist.cpp ./data_structures/cpp/my_queue_vector.cpp

# Benchmark legado (manter compatibilidade)
benchmark_cpp: ./scripts/benchmark_cpp.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_cpp ./scripts/benchmark_cpp.cpp $(SRCS_ALG_CPP)

main_data_structures: ./scripts/main_data_structures.cpp ./data_structures/cpp/vector.cpp ./data_structures/cpp/heap.cpp ./data_structures/cpp/trees.cpp ./data_structures/cpp/queue_linkedlist.cpp
	mkdir -p ./bin
	g++ -O1 -Wall -o ./bin/main_data_structures ./scripts/main_data_structures.cpp ./data_structures/cpp/vector.cpp ./data_structures/cpp/heap.cpp ./data_structures/cpp/trees.cpp ./data_structures/cpp/queue_linkedlist.cpp

main_algorithms: ./scripts/main_algorithms.cpp $(SRCS_ALG_CPP) $(SRCS_UTILS)
	mkdir -p ./bin
	g++ -O1 -Wall -o ./bin/main_algorithms ./scripts/main_algorithms.cpp $(SRCS_ALG_CPP) $(SRCS_UTILS)

main_lista: ./scripts/main_lista.cpp
	mkdir -p ./bin
	g++ -O1 -Wall -o ./bin/main_lista ./scripts/main_lista.cpp

# ============================================================================
# QUESTÕES DO MESTRADO (Q10-Q15)
# ============================================================================

QUESTAO_COMMON = mestrado/common/questao_utils.cpp

# Questão 10: Min/Max Simultâneo
benchmark_q10: mestrado/questao_10_minmax/minmax.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q10 mestrado/questao_10_minmax/benchmark_minmax.cpp mestrado/questao_10_minmax/minmax.cpp $(QUESTAO_COMMON)

test_q10: mestrado/questao_10_minmax/minmax.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q10 mestrado/questao_10_minmax/test_minmax.cc mestrado/questao_10_minmax/minmax.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Questão 11: Seleção (Randomized vs Deterministic)
benchmark_q11: mestrado/questao_11_selection/selection.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q11 mestrado/questao_11_selection/benchmark_selection.cpp mestrado/questao_11_selection/selection.cpp $(QUESTAO_COMMON)

test_q11: mestrado/questao_11_selection/selection.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q11 mestrado/questao_11_selection/test_selection.cc mestrado/questao_11_selection/selection.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Questão 12: Mediana Ponderada
benchmark_q12: mestrado/questao_12_weighted_median/weighted_median.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q12 mestrado/questao_12_weighted_median/benchmark_weighted_median.cpp mestrado/questao_12_weighted_median/weighted_median.cpp $(QUESTAO_COMMON)

test_q12: mestrado/questao_12_weighted_median/weighted_median.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q12 mestrado/questao_12_weighted_median/test_weighted_median.cc mestrado/questao_12_weighted_median/weighted_median.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Questão 13: Estruturas de Busca
benchmark_q13: mestrado/questao_13_search_structures/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q13 mestrado/questao_13_search_structures/benchmark_search_structures.cpp mestrado/questao_13_search_structures/sorted_array.cpp mestrado/questao_13_search_structures/linked_list.cpp mestrado/questao_13_search_structures/avl_tree.cpp $(QUESTAO_COMMON)

test_q13: mestrado/questao_13_search_structures/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q13 mestrado/questao_13_search_structures/test_search_structures.cc mestrado/questao_13_search_structures/sorted_array.cpp mestrado/questao_13_search_structures/linked_list.cpp mestrado/questao_13_search_structures/avl_tree.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Questão 14: Hash Tables (Chaining vs Open Addressing)
benchmark_q14: mestrado/questao_14_hash_tables/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q14 mestrado/questao_14_hash_tables/benchmark_hash_tables.cpp mestrado/questao_14_hash_tables/hash_chaining.cpp mestrado/questao_14_hash_tables/hash_open_addressing.cpp $(QUESTAO_COMMON)

test_q14: mestrado/questao_14_hash_tables/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q14 mestrado/questao_14_hash_tables/test_hash_tables.cc mestrado/questao_14_hash_tables/hash_chaining.cpp mestrado/questao_14_hash_tables/hash_open_addressing.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Questão 15: Hash Probing (Linear vs Double)
benchmark_q15: mestrado/questao_15_hash_probing/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_q15 mestrado/questao_15_hash_probing/benchmark_hash_probing.cpp mestrado/questao_15_hash_probing/hash_linear_probing.cpp mestrado/questao_15_hash_probing/hash_double_hashing.cpp $(QUESTAO_COMMON)

test_q15: mestrado/questao_15_hash_probing/*.cpp $(QUESTAO_COMMON)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/test_q15 mestrado/questao_15_hash_probing/test_hash_probing.cc mestrado/questao_15_hash_probing/hash_linear_probing.cpp mestrado/questao_15_hash_probing/hash_double_hashing.cpp $(QUESTAO_COMMON) $(LDFLAGS)

# Alvos de conveniência
all_questoes: benchmark_q10 benchmark_q11 benchmark_q12 benchmark_q13 benchmark_q14 benchmark_q15

test_all_questoes: test_q10 test_q11 test_q12 test_q13 test_q14 test_q15

run_all_questoes: all_questoes
	mkdir -p reports mestrado/plots_questoes
	@echo "=========================================="
	@echo "EXECUTANDO TODAS AS QUESTÕES"
	@echo "=========================================="
	./bin/benchmark_q10 --max-size 10000000 --runs 5 --output-csv reports/q10_results.csv --quiet
	./bin/benchmark_q11 --max-size 1000000 --runs 5 --output-csv reports/q11_results.csv --quiet
	./bin/benchmark_q12 --max-size 100000 --runs 5 --output-csv reports/q12_results.csv --quiet
	./bin/benchmark_q13 --max-size 100000 --runs 3 --output-csv reports/q13_results.csv --quiet
	./bin/benchmark_q14 --max-size 100000 --runs 3 --output-csv reports/q14_results.csv --quiet
	./bin/benchmark_q15 --max-size 100000 --runs 3 --output-csv reports/q15_results.csv --quiet
	@echo "=========================================="
	@echo "GERANDO GRÁFICOS"
	@echo "=========================================="
	python3 mestrado/common/plot_questoes.py reports/q1*_results.csv
	@echo "=========================================="
	@echo "CONCLUÍDO! Gráficos em mestrado/plots_questoes/"
	@echo "=========================================="

# ============================================================================

# Alvos auxiliares para execução de benchmarks
bench_matmul:
	mkdir -p reports
	./bin/benchmark_matmul --max-size 256 --runs 3 --output-csv reports/bench_matmul_$(shell date +%Y%m%d_%H%M%S).csv --quiet

bench_sorting:
	mkdir -p reports
	./bin/benchmark_sorting --max-size 10000 --runs 3 --output-csv reports/bench_sorting_$(shell date +%Y%m%d_%H%M%S).csv --quiet

bench_datastructures:
	mkdir -p reports
	./bin/benchmark_datastructures --max-size 10000 --runs 3 --output-csv reports/bench_datastructures_$(shell date +%Y%m%d_%H%M%S).csv --quiet

bench_all: bench_matmul bench_sorting bench_datastructures

# Alvo para gerar gráficos
plots:
	mkdir -p reports
	python scripts/plot_benchmarks.py reports/*.csv --output-dir reports/plots_$(shell date +%Y%m%d_%H%M%S) --log-y
	@echo "Plots gerados com sucesso!"

# Limpeza
clear:
	rm -f ./bin/main_algorithms ./bin/main_data_structures ./bin/benchmark_cpp ./bin/benchmark_matmul ./bin/benchmark_sorting ./bin/benchmark_datastructures ./bin/test_runner ./bin/test_matrix ./bin/main_lista

test_all: $(SRCS_TESTS) $(SRCS_ALG_CPP) $(SRCS_DS_CPP) $(SRCS_DS_C) $(SRCS_UTILS)
	mkdir -p ./bin
	g++ $(CXXFLAGS) $^ -o ./bin/test_runner $(LDFLAGS)
	./bin/test_runner

test_matrix: ./tests/matrix_multiplication_test.cc $(SRCS_ALG_CPP) $(SRCS_DS_CPP) $(SRCS_DS_C) $(SRCS_UTILS)
	mkdir -p ./bin
	g++ $(CXXFLAGS) $^ -o ./bin/test_matrix $(LDFLAGS)
	./bin/test_matrix
