CC = gcc
LINK = gcc

OBJECT_FILES = main.o
FLAGS = -Wall
BIN = exam2
INCLUDE_DIRS = ./

all: $(BIN)

$(BIN): $(OBJECT_FILES)
	$(LINK) $(FLAGS) $^ -o $@

# Apparently, make has predefined pattern rules (the newer version of suffixes) to automatically handle generating
# common file types. For instance, for my rule above to create the "exam2" executable, I list my prerequisites as all
# of the object files in OBJECT_FILES. However, I don't specify a means by which to generate/create those dependencies.
# Therefore, I must either make a rule for each one (Ouch), or define a pattern rule.

# Make has a pattern rule built in that does this. For fun, I add it here explicitly. The key here to making a 
# pattern rule is this wildcard character %. This can take any value.
%.o : %.c
	$(CC) $(FLAGS) -I $(INCLUDE_DIRS) -c $< -o $@


clean:
	rm -rf $(OBJECT_FILES)

test: $(BIN)
	./$(BIN)