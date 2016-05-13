all: clean build style


build: 
	 platformio run

run:
	cd ./ubx-decoder-embedded && platformio run -t upload

clean:
	rm -rf docs
	
doc:
	doxygen

style:
	astyle --style=java src/ubx_decoder.cpp


