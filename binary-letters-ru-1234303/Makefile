.PHONY: all clean

APP = main

CXX = g++
OPTS = -std=c++17 -Wall -Wextra
SRC = $(wildcard *.cpp)

all: $(APP)

run: $(APP)
	./$(APP)

$(APP): $(SRC)
	$(CXX) $(OPTS) $^ -o $@ 
clean:
	rm -rf $(APP)
