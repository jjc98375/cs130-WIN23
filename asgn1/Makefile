# CC = clang
# CFLAGS = -Wall -Wextra -Werror -pedantic
# all:
# 	memory.o
# memory.o: memory.c
# 	$(CC) -c $(CFLAGS) memory.c
# clean:
# 	rm -f *.o memory



CC = clang
RM = rm -f

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -Wextra -Werror -pedantic

# the build target executable:
TARGET = memory

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) *.o $(TARGET)
