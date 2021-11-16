

CC = gcc
LINK = gcc
FLAGS = -Wall

# Rule for building client executable.
client : client.o
	$(LINK) $(FLAGS) $^ -o $@

# Pattern rule for object files.
%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f client.o

testclient : client
	./client