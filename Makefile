# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=gnu99 -g

# Source files
SRC = mmu.c utils.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = mmu

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)
