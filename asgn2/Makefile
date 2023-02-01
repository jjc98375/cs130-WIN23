# CC=clang
# CFLAGS=-Wall -Werror -Wextra -pedantic
# all: httpserver
# httpserver: httpserver.o
# 	$(CC) $(CFLAGS) -o httpserver httpserver.o asgn2_helper_funcs.a
# httpserver.o: httpserver.c
# 	$(CC) $(CFLAGS) -c httpserver.c
# clean:
# 	rm -f *.o httpserver

CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic
EXEC = httpserver
OBJS = httpserver.o module.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS) asgn2_helper_funcs.a

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format: 
	clang-format -i *.c *.h