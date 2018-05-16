default_target: all
all: helloworld cubic
helloworld: helloworld/helloworld.cpp
	cd helloworld/ && make
	cd ../
cubic: cubic/cubic.cpp
	cd cubic/ && make 
	cd ../
run_helloworld:
	cd helloworld/ && make run
	cd ../
run_cubic:
	cd cubic/ && make run
	cd ../