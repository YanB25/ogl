default_target: all
DIR=BesselCurve
EXEC=BesselCurve
all: $(DIR)/$(EXEC).out
HEADERS = common/shader.hpp $(DIR)/include/config.hpp
$(DIR)/$(EXEC).out: $(DIR)/$(EXEC).cpp ${HEADERS}
	@make -C $(DIR)/
run:
	@make run -C $(DIR)/
clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.out" -delete
	find . -type f -name "*.gch" -delete