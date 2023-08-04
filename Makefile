# compiler and flags
CC := g++
CFLAGS := -Wall -Wextra -std=c++11

# source files
SRC_FILES := $(wildcard src/*.cpp) main.cpp

# target executable
TARGET := main

# rules
$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# cleaning
clean:
	rm main