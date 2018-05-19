default_target: all
ROOT=..
INCLUDE=ROOT/external/glfw-3.1.2/include/
LINK= -lGL -lGLEW
CC_FLAGS=`pkg-config --cflags glfw3`
GLFW_LINK=`pkg-config --static --libs glfw3`
OBJ=../common/build/shader.o

SHADER=../common/build/shader.o
EXEC=star
all: ${EXEC}.out

run:
	./${EXEC}.out

${EXEC}.o: ${EXEC}.cpp include/mymath.hpp include/config.hpp
	g++ -c $^ -I ${ROOT} ${LINK} 
${EXEC}.out: ${EXEC}.o ../common/build/shader.o include/mymath.o
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK}
${SHADER}: ../common/shader.cpp ../common/shader.hpp
	cd ../common/ && make build/shader.o
	cd ../star/
include/mymath.o: include/mymath.cpp include/config.hpp include/mymath.hpp
	cd include/ && make mymath.o
	cd ../
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.gch" -delete