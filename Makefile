FLAGS = -g3 -pedantic -std=c++11 # -Wall -Wextra
LSH_EXE = lsh
CUBE_EXE = cube
CLUSTER_EXE = cluster
COMMON_HEADERS = header/Dataset.h header/HashTable.h  header/InputParser.h header/vector_operations.h header/Point.h header/neighbour_search.h header/Hypercube.h
COMMON_CPP = src/Dataset.cpp src/HashTable.cpp src/vector_operations.cpp src/neighbour_search.cpp src/Hypercube.cpp


all: LSH CUBE CLUSTER


LSH: src_main/lsh_main.cpp $(COMMON_HEADERS) $(COMMON_CPP)
	g++ $(FLAGS) -o $(LSH_EXE) src_main/lsh_main.cpp $(COMMON_CPP)


CUBE: src_main/hcube_main.cpp $(COMMON_HEADERS) $(COMMON_CPP)
	g++ $(FLAGS) -o $(CUBE_EXE) src_main/hcube_main.cpp $(COMMON_CPP)


CLUSTER: src_main/cluster_main.cpp $(COMMON_HEADERS)  $(COMMON_CPP) header/clustering.h src/clustering.cpp
	g++ $(FLAGS) -o $(CLUSTER_EXE) src_main/cluster_main.cpp $(COMMON_CPP) src/clustering.cpp


clean:
	rm -f *.o $(LSH_EXE) $(CUBE_EXE) $(CLUSTER_EXE)
