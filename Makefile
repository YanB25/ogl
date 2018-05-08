default_target: all
all: helloworld cubic
helloworld:
	cd helloworld/ && make
	cd ../
cubic:
	cd cubic/ && make 
	cd ../