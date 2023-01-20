# CC = clang
# RM = rm -f

# # compiler flags:
# #  -Wall turns on most, but not all, compiler warnings
# CFLAGS  = -Wall -Wextra -Werror -pedantic

# # the build target executable:
# TARGET = memory

# all: $(TARGET)

# $(TARGET): $(TARGET).c
# 	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# clean:
# 	$(RM) *.o $(TARGET)

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

%.0: %.c
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f $(EXECBIN) $(OBJECTS)

format: $(FORMATS)

%.fmt: %.c
	$(FORMAT) -i $<
	touch $@