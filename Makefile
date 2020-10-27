main_data_structures: ./main_data_structures.cpp
	g++ -O1 -Wall -o ./bin/main_data_structures ./main_data_structures.cpp ./data_structures/cpp/vector.cpp

main_algorithms: ./main_algorithms.cpp
	g++ -O1 -Wall -o ./bin/main_algorithms ./main_algorithms.cpp ./algorithms/cpp/luAlgorithms.cpp ./utils/utils.cpp

clear:
	rm ./bin/main_algorithms ./bin/main_data_structures
