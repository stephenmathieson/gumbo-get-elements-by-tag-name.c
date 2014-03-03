
CC ?= gcc
SRC = $(wildcard src/*.c)
DEPS = $(wildcard deps/*/*.c)
OBJS = $(SRC:.c=.o)
OBJS += $(DEPS:.c=.o)
CFLAGS = -std=c99 -Ideps -Isrc
CFLAGS += -Wall -Wno-unused-function

test: test.o $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)
	./$@

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f test test.o $(OBJS)

.PHONY: clean test
