# Define some variables for use throughout Makefile.
CC = gcc				# variable to set which compiler and linker to use.
LINK = gcc

# Here's a rule (aka production) that just requires another rule.
all : sample

sample : main.o
	$(LINK) main.o -o $@
# Notice how we use bash syntax to actually get the value of CC. This is because
# the RULE defines the actual commands that linux needs to run, and therefore
# we are defining a command line command to run!
# Also note, $@ is a built in variable for the target of the rule.

# Here's a rule to make the dependency of the target sample above.
main.o : main.c
	$(CC) -c -Wall main.c -o main.o

# Could've also more generally just defined a suffix. Here's a generalized suffix rule.
.SUFFIXES: .c .o
# Then here's the rule to actually build those files with those suffixes.
.c.o :
	$(CC) -c -Wall -o $@ $<


# Now, here's a rule to clean up.
clean :
	rm -f sample main.o
# Note that a useful addition might be to make a variable containing all object
# file names such that you wouldn't have to update all instances of listing the
# object files.

# The prequisite for running unittests is the unittest executable being linked.
# If it's not linked/doesn't exist, the make will run the commands under the unittest rule to generate the executable.
runtests : unittest
	./unittest

unittest : main.o test.o
	$(CC) $^ -o $@

