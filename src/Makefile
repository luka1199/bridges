CXX = g++ -Wall -pedantic -std=c++11
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))
TEST_BINARIES = $(basename $(wildcard *Test.cpp))
HEADERS = $(wildcard *.h)
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))
LIBRARIES = -lncurses

.PRECIOUS: %.o
.SUFFIXES:
.PHONY: all compile test checkstyke

all: compile test checkstyle

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

test: $(TEST_BINARIES)
#	for T in $(TEST_BINARIES); do valgrind --leak-check=full ./$$T; done
	for T in $(TEST_BINARIES); do ./$$T.exe; done

checkstyle:
	python3 ./cpplint.py --repository=. *.h *.cpp

clean:
	rm -f *.o
	rm -f $(MAIN_BINARIES)
	rm -f $(TEST_BINARIES)

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@.exe $^ $(LIBRARIES)

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@.exe $^ $(LIBRARIES) -lgtest -lgtest_main -lpthread

%.o: %.cpp $(HEADERS)
	$(CXX) -c $<
