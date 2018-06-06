default_target: all
DIR=BesselCurve
EXEC=BesselCurve
all:
	make -C BesselCurve/
	make -C DataReading/
run:
	@echo "make run_obj, make run_off, make run_ply, make run_bessel"
run_obj:
	make -C DataReading/ run_obj

run_off:
	make -C DataReading/ run_off

run_ply:
	make -C DataReading/ run_ply
run_bessel:
	make -C BesselCurve/ run_bessel
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.out" -delete
	find . -type f -name "*.gch" -delete