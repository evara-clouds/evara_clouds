CC = gcc
CFLAGS = -Wall -I firmware/inc

SRC = firmware/src/scheduler.c examples/scheduler/main.c
OUT = build/scheduler_demo

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(OUT)

clean:
	rm -rf build
