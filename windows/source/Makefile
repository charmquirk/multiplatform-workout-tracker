# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./include
LDLIBS = -lraylib -lgdi32 -lwinmm

# Target executable (with .exe extension for Windows)
TARGET = multiplatform-workout-tracker.exe

# Directory structure
BUILD_DIR = ../build
SRC_DIR = .
INCLUDE_DIR = ./include
LIBS_DIR = ./libs

# Automatically find all files
SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_FILES = $(wildcard $(LIBS_DIR)/*.a) $(wildcard $(LIBS_DIR)/*.lib)

# Extract library names from library files (for -l flags)
LIB_NAMES = $(patsubst lib%,%,$(basename $(notdir $(LIB_FILES))))

# Add -l flags for each library
LDLIBS += $(addprefix -l,$(LIB_NAMES))

# Object files (will be placed in build directory)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# Link object files to create executable in build directory
$(BUILD_DIR)/$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile source files to object files in build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build files (Windows rm command)
clean:
	if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"

# Run the application from build directory
run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

# Debug info - show what files were found
debug:
	@echo "Source files found: $(SRCS)"
	@echo "Library files found: $(LIB_FILES)"
	@echo "Library names: $(LIB_NAMES)"
	@echo "Object files to create: $(OBJS)"

.PHONY: all clean run debug
