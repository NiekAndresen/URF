.PHONY:clean

CC=clang
CFLAGS=-Wall -Werror

BIN = testURF.x
SOURCE = main.c URF.c io.c graphURF.c utility.c apsp.c CycleFamsURF.c CFquicksort.c URFInfo.c URFrelation.c URFhandler.c
OBJ = $(SOURCE:.c=.o)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~
