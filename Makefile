benchmark: ./benchmark_visualizer/benchmark.cpp
	g++ -O1 -Wall -o ./bin/benchmark_visualizer ./benchmark_visualizer/benchmark.cpp ./algorithms/cpp/luAlgorithms.cpp

benchmark_cpp: ./benchmark_cpp.cpp
	g++ -std=c++17 -O3 -march=native -Wall -o ./bin/benchmark_cpp ./benchmark_cpp.cpp ./algorithms/cpp/luAlgorithms.cpp

main_data_structures: ./main_data_structures.cpp
	g++ -O1 -Wall -o ./bin/main_data_structures ./main_data_structures.cpp ./data_structures/cpp/vector.cpp ./data_structures/cpp/heap.cpp ./data_structures/cpp/trees.cpp ./data_structures/cpp/queue_linkedlist.cpp ./data_structures/cpp/stack.h

main_algorithms: ./main_algorithms.cpp
	g++ -O1 -Wall -o ./bin/main_algorithms ./main_algorithms.cpp ./algorithms/cpp/luAlgorithms.cpp ./utils/utils.cpp

main_lista: ./main_lista.cpp
	g++ -O1 -Wall -o ./bin/main_lista ./main_lista.cpp

clear:
	rm -f ./bin/main_algorithms ./bin/main_data_structures ./bin/benchmark_visualizer ./bin/benchmark_cpp

test_all: ./tests/*.cc 
	g++ -pthread ./tests/*.cc ./data_structures/c/*.c ./utils/*.cpp ./algorithms/cpp/luAlgorithms.cpp -lgtest -lgtest_main -o ./bin/tests && ./bin/tests

test_matrix: ./tests/matrix_multiplication_test.cc
	g++ -pthread ./tests/matrix_multiplication_test.cc ./algorithms/cpp/luAlgorithms.cpp -lgtest -lgtest_main -o ./bin/test_matrix && ./bin/test_matrix