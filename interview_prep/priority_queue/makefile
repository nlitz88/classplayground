CC = gcc
LINK = gcc
FLAGS = -g -Wall
EXECUTABLE = testprog
TEST_EXECUTABLE = unittests
OBJECT_FILES = main.o test.o

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

prog: main.o
	$(LINK) $(FLAGS) $^ -o $(EXECUTABLE)

unittest: test.o heap.o
	$(LINK) $(FLAGS) $^ -o $(TEST_EXECUTABLE)

test: unittest
	./$(TEST_EXECUTABLE)

run: prog
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECT_FILES) $(EXECUTABLE) $(TEST_EXECUTABLE)