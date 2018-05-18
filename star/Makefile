default_target: all
ROOT=..
INCLUDE=ROOT/external/glfw-3.1.2/include/
LINK= -lGL -lGLEW
CC_FLAGS=`pkg-config --cflags glfw3`
GLFW_LINK=`pkg-config --static --libs glfw3`
OBJ=../common/build/shader.o

SHADER=../common/build/shader.o
EXEC=star
all: ${EXEC}

run:
	./${EXEC}

${EXEC}.o: ${EXEC}.cpp include/mymath.hpp include/config.hpp
	g++ -c $^ -I ${ROOT} ${LINK} 
${EXEC}: ${EXEC}.o ../common/build/shader.o include/mymath.o
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK}
${SHADER}: ../common/shader.cpp
	cd ../commom/ && make shader.o
	cd ../learning/
include/mymath.o: include/mymath.cpp include/config.hpp
	cd include/ && make mymath.o
	cd ../
clean:
	rm *.o && rm ${EXEC}