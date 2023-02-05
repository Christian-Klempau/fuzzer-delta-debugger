CXX=g++
CXXFLAGS=-Wall -O3 -DNDEBUG
all: parser
solver: parser.cpp makefile
	$(CXX) $(CXXFLAGS) -o $@ $<
clean:
	rm -f parser makefile 
.PHONY: clean 
