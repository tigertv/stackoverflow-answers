.PHONY: all clean

CC = gcc
OPTS = -Wall -Wextra -std=c11 -O3
APP = main
SRC = new-main.c parser.c 
OBJ = $(SRC:.c=.o)
# $(wildcard *.c)

all: $(APP)

run: $(APP)
	./$(APP)
$(APP): $(OBJ)
	$(CC) $(OPTS) $^ -o $@ 
%.o : %.c
	$(CC) $(OPTS) $< -c -o $@ 
clean:
	rm -rf *.o $(APP)
