CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic
all: memory
memory: memory.o
	$(CC) $(CFLAGS) -o memory memory.o
memory.o: memory.c
	$(CC) -c $(CFLAGS) memory.c
clean:
	rm -f *.o memory