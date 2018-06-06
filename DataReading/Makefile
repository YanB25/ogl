default_target: all
ROOT=..
INCLUDE=ROOT/external/glfw-3.1.2/include/
LINK= -lGL -lGLEW
CC_FLAGS=`pkg-config --cflags glfw3`
GLFW_LINK=`pkg-config --static --libs glfw3`
STD_FLAGS=-std=c++11
OBJ=../common/build/shader.o

SHADER=../common/build/shader.o
EXEC_OBJ=dataReading_OBJ
EXEC_PLY=dataReading_PLY
EXEC_OFF=dataReading_OFF
EXEC=dataReading
all: ${EXEC_OBJ}.out ${EXEC_OFF}.out ${EXEC_PLY}.out

run_obj:
	./${EXEC_OBJ}.out
run_off:
	./${EXEC_OFF}.out
run_ply:
	./${EXEC_PLY}.out

${EXEC_OBJ}.o: ${EXEC}.cpp include/config.hpp include/controllPoint.h
	g++ -c $< -I ${ROOT} ${LINK} $(STD_FLAGS) -D OBJ -o $@
${EXEC_OBJ}.out: ${EXEC_OBJ}.o ../common/build/shader.o 
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK} $(STD_FLAGS)

${EXEC_PLY}.o: ${EXEC}.cpp include/config.hpp include/controllPoint.h
	g++ -c $< -I ${ROOT} ${LINK} $(STD_FLAGS) -D PLY -o $@
${EXEC_PLY}.out: ${EXEC_PLY}.o ../common/build/shader.o 
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK} $(STD_FLAGS)

${EXEC_OFF}.o: ${EXEC}.cpp include/config.hpp include/controllPoint.h
	g++ -c $< -I ${ROOT} ${LINK} $(STD_FLAGS) -D OFF -o $@
${EXEC_OFF}.out: ${EXEC_OFF}.o ../common/build/shader.o 
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK} $(STD_FLAGS)

${SHADER}: ../common/shader.cpp ../common/shader.hpp
	cd ../common/ && make build/shader.o
	cd ../star/
include/mymath.o: include/mymath.cpp include/config.hpp include/mymath.hpp
	cd include/ && make mymath.o
	cd ../
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.gch" -delete