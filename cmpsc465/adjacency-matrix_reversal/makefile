CC = gcc
LINK = gcc
FLAGS = -g -Wall
EXECUTABLE = testprog
OBJECT_FILES = main.o

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

testprog: main.o
	$(LINK) $(FLAGS) $^ -o $(EXECUTABLE)

test: testprog
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJECT_FILES) $(EXECUTABLE)