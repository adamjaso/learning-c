SHELL := /bin/bash

CC := gcc
CFLAGS := -Wall -pthread

all:
	mkdir -p bin
	make $(shell ls src)

%: CWD := ../../
%:
	make -f $(CWD)Makefile -C src/$@ $@.a BIN=$(CWD)bin/

%.a: %.c
	$(CC) $(CFLAGS) -o $(BIN)$@ $^
