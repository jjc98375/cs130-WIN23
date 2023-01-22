CC 		= clang
CFLAGS 	= -Wall -Werror -Wextra -pedantic

all: memory

memory: memory.o
	$(CC) $(CFLAGS) -o memory memory.o

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c

clean:
	rm -f *.o memory
	
format: 
	clang-format -i *.c