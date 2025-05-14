# Makefile for Sudoku Project (with bin/ and build/ folders)

CC = gcc
CFLAGS = -Wall -O2 -I.
SRC = main.c sudoku_cli.c sudoku_hint.c sudoku_solver.c save_load.c
OBJ_DIR = build
BIN_DIR = bin
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
TARGET = $(BIN_DIR)/sudoku

all: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files into .o files in ./build
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build/ and bin/ folders if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

