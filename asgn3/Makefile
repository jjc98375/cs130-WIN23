CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic

all: queue.o

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c
clean:
	rm -f *.o

format: 
	clang-format -i *.c *.h
