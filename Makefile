default_target: all
all: star/star.out
HEADERS = star/include/mymath.hpp common/shader.hpp star/include/config.hpp
star/star.out: star/star.cpp ${HEADERS}
	cd star/ && make star.out
	cd ../
run:
	cd star/ && ./star.out
	cd ../
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.out" -delete
	find . -type f -name "*.gch" -delete