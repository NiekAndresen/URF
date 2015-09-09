.PHONY:clean

CC=clang
CFLAGS=-Wall -Werror

BIN = testURF.x
MAIN = main.c
SOURCE = URF.c io.c graphURF.c utility.c apsp.c CycleFamsURF.c CFquicksort.c URFInfo.c URFrelation.c URFhandler.c
URFOBJ = $(SOURCE:%.c=%.o)

$(BIN): $(URFOBJ)
	$(CC) $(CFLAGS) -o $@ $(URFOBJ) $(MAIN)

%.o: ./URFcode/%.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~
