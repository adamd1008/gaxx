CXX=g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=

MY_SOURCES=$(wildcard *.cpp)
MY_TESTS=$(MY_SOURCES:.cpp=)

# TODO GoogleTest
GTEST_SOURCES=$(wildcard gtest_*.cpp)
GTEST_TESTS=$(GTEST_SOURCES:.cpp=)

all: $(MY_TESTS) $(GTEST_TESTS)

.cpp:
	@echo "** Compiling executable '$@'"
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *~ $(MY_TESTS) $(GTEST_TESTS)
