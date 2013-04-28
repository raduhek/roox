# Compile tests
CC=gcc
CCFLAGS=-Wall -c

all:
	$(CC) $(CCFLAGS) list_node.c
	$(CC) -o main list.c list_node.o

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o
	rm -rf b

debug: CCFLAGS += -DDEBUG -g
debug: all

help:
	@echo "Available make options:"
	@echo ""
	@echo "make -> build executable"
	@echo "make debug -> build executable with debug option"
	@echo "make clean -> remove executable and object files"
	@echo "make clean_o -> remove object files"
	@echo "make help -> display this text"
