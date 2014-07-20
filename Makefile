CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wextra -pedantic
OBJECTS = bit_functions.o confuzz.o election.o hamming_table.o votesim.o
RM = rm

votesim : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

bit_functions.o : bit_functions.h hamming_table.h
confuzz.o : 
election.o: bit_functions.h confuzz.h election.h
hamming_table.o :
votesim.o : bit_functions.h confuzz.h election.h

.PHONY : clean
clean :
	$(RM) $(OBJECTS) votesim
