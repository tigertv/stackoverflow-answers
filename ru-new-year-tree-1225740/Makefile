.PHONY: all clean run

CXX = g++
OPTS =  -Wall -Wextra
APP = main
SRC = $(wildcard *.cpp)

all: main 

$(APP): $(SRC)
	$(CXX) $(OPTS) $^ -o $@ 
run: $(APP)
	./$(APP)
clean:
	rm -rf $(APP)
