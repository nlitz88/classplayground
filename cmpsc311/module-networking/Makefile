

CC = gcc
LINK = gcc
FLAGS = -Wall
OBBJECT_FILES = client.o server.o
EXECUTABLES = client server

all : $(EXECUTABLES)

# Rule for building client executable.
client : client.o
	$(LINK) $(FLAGS) $^ -o $@

server : server.o
	$(LINK) $(FLAGS) $^ -o $@

# Pattern rule for object files.
%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OBBJECT_FILES) $(EXECUTABLES)

testclient : client
	./client

testserver : server
	./server