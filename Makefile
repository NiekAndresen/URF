# Niek Andresen 2015 - Bachelor Thesis

.PHONY:clean

CC=clang
CFLAGS=-Wall -Werror

URFCODEDIR = ./URFcode

# The following files are all (going to be) in the same directory as the Makefile
# BIN: name of the executable
# MAIN: file with main function
# SOURCE: other .c-files of your program
# Change these three lines:
BIN = testURF.x
MAIN = main.c
SOURCE =

OBJ = $(SOURCE:%.c=%.o) 
URFSRC = URF.c io.c graphURF.c utility.c apsp.c CycleFamsURF.c CFquicksort.c URFInfo.c URFrelation.c URFhandler.c
URFOBJ = $(URFSRC:%.c=%.o)
URFOBJDIR = $(URFSRC:%.c=$(URFCODEDIR)/%.o)

$(BIN): $(URFOBJDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(URFOBJ) $(MAIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~
