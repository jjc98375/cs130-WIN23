# CC = clang
# CFLAGS = -Wall -Wextra -Werror -pedantic
# all:
# 	memory
# memory: memory.o
# 	$(CC) $(CFLAGS) -o memory memory.o
# memory.o: memory.c
# 	$(CC) -c $(CFLAGS) memory.c
# clean:
# 	rm -f *.o memory




CC = clang
RM = rm -f

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -Wextra -Werror -pedantic

# the build target executable:
TARGET = memory

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) *.o $(TARGET)