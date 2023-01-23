CC = clang

FORMAT = clang-format

CFLAGS = -Wall -pedantic -Werror -Wextra 

all: memory

memory: memory.c
	$(CC) $(CFLAGS) -o memory memory.c
clean:
	rm -f memory *.o
format:
	$(FORMAT)
%.fmt: %.c
	$(FORMAT) -i $<
	touch $@