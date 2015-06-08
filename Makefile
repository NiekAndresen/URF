.PHONY:clean

CC=clang
CFLAGS=-Wall -Werror

BIN = niek
SOURCE = URF.c io.c graph.c utility.c apsp.c
OBJECTS = $(SOURCE:.c=.o)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(SOURCE)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~
