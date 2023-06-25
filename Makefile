#!/bin/bash

# Author: Sihan Ma
# Date: 05.24.2023
# This is the Makefile for Homework 6B that compiles the memory
# management system.
# Modified provided starter code.

# The compiler: gcc for C programs
CC = gcc

# Compiler flags:
# -Wall for debugger warnings
# -std=c11 for updated standards
CARGS = -Wall -std=c11

# The name of the program that we are producing.
all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

# object files
bench.o: bench.c mem.h

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CARGS) -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CARGS) -c freemem.c

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CARGS) -c mem_utils.c

## make debug version
debug: CARGS += -g -D DEBUG
debug: bench

noassert: CARGS += -D NDEBUG
noassert: bench

## Utility targetscd
test: debug
	./bench
	./bench 5
	./bench 50000 10
	./bench 2 10 100
	./bench 18 5 100 16
	./bench 3 100 100 500 2000 1
	./bench 4 50 50 500 2000 1
	./bench 1 50 50 500 2000 1

# remove built files and backups
clean:
	rm -rf bench *.o *~