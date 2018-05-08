default_target: all
ROOT=..
INCLUDE=ROOT/external/glfw-3.1.2/include/
LINK= -lGL -lGLEW
CC_FLAGS=`pkg-config --cflags glfw3`
GLFW_LINK=`pkg-config --static --libs glfw3`
OBJ=../common/build/shader.o

SHADER=../common/build/shader.o
all: helloworld

run:
	./helloworld

helloworld.o: helloworld.cpp
	g++ -c $^ -I ${ROOT} ${LINK} 
helloworld: helloworld.o ../common/build/shader.o
	g++ ${CC_FLAGS} -o $@ $^ -I {ROOT} ${LINK} ${GLFW_LINK}
${SHADER}: ../common/shader.cpp
	cd ../commom/ && make shader.o
	cd ../cubic/
clean:
	rm *.o && rm helloworld