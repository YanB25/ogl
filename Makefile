default_target: all
all: helloworld cubic
helloworld:
	cd helloworld/ && make
	cd ../
cubic:
	cd cubic/ && make 
	cd ../
run_helloworld:
	cd helloworld/ && make run
	cd ../
run_cubic:
	cd cubic/ && make run
	cd ../