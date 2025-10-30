# Compiler settings
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c99 -I firmware/inc

# Output directories
BUILD_DIR := build

# Default target
.PHONY: all clean run

# Default build: scheduler demo
all: $(BUILD_DIR)/scheduler_demo

# Example scheduler demo
$(BUILD_DIR)/scheduler_demo: firmware/src/scheduler.c examples/scheduler/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Example 2: static memory allocator
example2_static:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) firmware/src/mem_pool.c examples/memory/example2_static/main.c -o $(BUILD_DIR)/example2_static
	@echo "Running static memory allocator demo..."
	@./$(BUILD_DIR)/example2_static

# Run the scheduler demo
run: all
	@echo "Running scheduler demo..."
	@./$(BUILD_DIR)/scheduler_demo

# Clean all builds
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

