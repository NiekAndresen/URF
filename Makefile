.PHONY:clean

CC=clang
CFLAGS=-Wall -Werror

BIN = testURF.x
SOURCE = main.c URF.c io.c graphURF.c utility.c apsp.c relevantCyclesURF.c
OBJECTS = $(SOURCE:.c=.o)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(SOURCE)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~
