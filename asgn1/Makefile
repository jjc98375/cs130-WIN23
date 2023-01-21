CFLAGS = -Wall -Werror -Wextra -pedantic
all: memory
memory: memory.o
		clang memory.o -o memory
memory.o: memory.c
		clang -c $(CFLAGS) memory.c
clean:
		rm -f *.o memory
format: 
		clang-format -i *.c