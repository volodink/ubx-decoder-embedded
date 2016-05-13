all: clean build style

build: 
	platformio run

run:
	platformio run -t upload

clean:
	rm -rf docs
	platformio run -t clean
	
doc:
	doxygen

style:
	astyle --style=java src/ubx_decoder.cpp


