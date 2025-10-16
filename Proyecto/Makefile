# Define the C compiler
CC = gcc

# Define the executable name
TARGET = main

# Define the source files
SRCS = main.c

# Rule to build the executable
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)