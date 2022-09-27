CC = gcc
LINK = gcc
FLAGS = -Wall -g
EXECUTABLE = testprog
OBJECT_FILES = main.o

testprog: main.o
	$(LINK) $(FLAGS) $^ -o $(EXECUTABLE)

test: testprog
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECT_FILES) $(EXECUTABLE)