CXXFLAGS=-std=c++11 $(shell root-config --cflags)
LIBS=$(shell root-config --libs)

run : uboone_example_driver
			@echo "Finished Compiling..."
			@echo "To run: ./uboone_example_driver"

uboone_example_driver : uboone_example_driver.o library_access.o

	g++ -o $@ $^ ${LIBS}

%.o : %.cc
	g++ ${CXXFLAGS} -o $@ -c $^ 
