# Compile tests
CC=gcc
CCFLAGS=-Wall -c

all:
	$(CC) $(CCFLAGS) child_list/child_list_node.c
	$(CC) $(CCFLAGS) child_list/child_list.c
	$(CC) $(CCFLAGS) list/list_node.c
	$(CC) $(CCFLAGS) list/list.c
	$(CC) $(CCFLAGS) queue/queue_node.c
	$(CC) $(CCFLAGS) queue/queue.c
	$(CC) $(CCFLAGS) trie/trie_utils.c
	$(CC) $(CCFLAGS) trie/trie_node.c
	$(CC) $(CCFLAGS) trie/trie.c
	$(CC) $(CCFLAGS) stack/stack_node.c
	$(CC) $(CCFLAGS) stack/stack.c
	$(CC) $(CCFLAGS) pair/pair.c
	$(CC) $(CCFLAGS) parse_tree/parse_tree.c
	$(CC) $(CCFLAGS) parse_tree/utils.c
	$(CC) $(CCFLAGS) sanitizer/sanitizer.c
	$(CC) $(CCFLAGS) main.c
	$(CC) -o main main.o list.o list_node.o child_list.o child_list_node.o queue_node.o queue.o trie_utils.o trie_node.o trie.o sanitizer.o parse_tree.o utils.o stack_node.o stack.o pair.o

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o
	rm -rf main

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
