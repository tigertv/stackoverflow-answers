.PHONY: all clean

CC = gcc
OPTS = -Wall -Wextra -std=c11 -O3
APP = main
SRC = new-main.c parser.c 
# $(wildcard *.c)

all: $(APP)

run: $(APP)
	./$(APP)

$(APP): $(SRC)
	$(CC) $(OPTS) $^ -o $@ 
clean:
	rm -rf $(APP)
