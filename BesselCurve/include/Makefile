all: 
mymath.o: mymath.cpp config.hpp
	g++ -c $^ -std=c++11 -Wall -W
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.gch" -delete