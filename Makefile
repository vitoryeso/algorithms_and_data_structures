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
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -I. -Ialgorithms/cpp -Idata_structures/cpp -Idata_structures/c -Iutils -I/usr/include/gtest -I/usr/include/gtest/internal -pthread

all: benchmark_matmul benchmark_sorting benchmark_datastructures main_data_structures main_algorithms main_lista test_all

.PHONY: all clean clear bench_matmul bench_sorting bench_datastructures bench_all plots

# Novos benchmarks unificados
benchmark_matmul: ./scripts/benchmark_matmul.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_matmul ./scripts/benchmark_matmul.cpp $(SRCS_ALG_CPP)

benchmark_sorting: ./scripts/benchmark_sorting.cpp $(SRCS_ALG_CPP)
	mkdir -p ./bin
	g++ $(CXXFLAGS) -o ./bin/benchmark_sorting ./scripts/benchmark_sorting.cpp $(SRCS_ALG_CPP)

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
