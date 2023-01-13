# CC = clang
# CFLAGS = -Wall -Wextra -Werror -pedantic
# all:
# 	hello
# hello: hello.o
# 	$(CC) $(CFLAGS) -o hello hello.o
# hello.o: hello.c
# 	$(CC) -c $(CFLAGS) hello.c
# clean:
# 	rm -f *.o hello




CC = clang
RM = rm -f

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -Wextra -Werror -pedantic

# the build target executable:
TARGET = hello

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) *.o $(TARGET)