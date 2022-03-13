

OBJECT_FILES = main.o
CC = gcc
LINK = gcc
FLAGS = -Wall -g
EXECUTABLE = testprog

testprog : main.o
	$(LINK) $(FLAGS) $^ -o $(EXECUTABLE)

test : testprog
	./$(EXECUTABLE)

clean :
	rm -rf $(OBJECT_FILES) $(EXECUTABLE)