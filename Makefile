# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Target executable
TARGET = phpcc

# Source files
SRCS = main.c lexer.c parser.c semantic.c symbol.c codegen.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean