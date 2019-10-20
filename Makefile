CXXFLAGS = -g -Wall -Wfatal-errors -std=c++17

ALL = main

all: $(ALL)

main: main.cpp Makefile
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

clean:
	$(RM) $(ALL) *.o

test: all
	bash test.sh
