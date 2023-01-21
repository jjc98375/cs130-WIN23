# CC = clang

# # compiler flags:
# #  -Wall turns on most, but not all, compiler warnings
# CFLAGS  = -Wall -Wextra -Werror -pedantic

# # the build target executable:
# TARGET = memory

# all: $(TARGET)

# $(TARGET): $(TARGET)
# 	$(CC) -o $@ $^

# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o $@ $^

# clean:
# 	rm -rf $(TARGET) *.o


# all:
# 	$(CC) $(CFLAGS) memory.c -o memory.o








EXECBIN = memory
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)
FORMATS = $(SOURCES:%.c=%.fmt)

CC = clang
FORMAT = clang-format
CFLAGS = -Wall -Wpedantic -Werror -Wextra

.PHONY: all clean format

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f $(EXECBIN) $(OBJECTS)

format: $(FORMATS)

%.fmt: %.c
	$(FORMAT) -i $<
	touch $@