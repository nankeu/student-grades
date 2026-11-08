# Makefile for Unix (Linux / macOS)
#   make         - build the program
#   make run     - build and run
#   make clean   - delete compiled files

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
TARGET   = students
SRCS     = $(wildcard src/*.cpp)
OBJS     = $(SRCS:.cpp=.o)
HEADERS  = $(wildcard include/*.h)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
