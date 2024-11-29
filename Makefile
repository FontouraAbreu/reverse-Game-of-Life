CC = g++
OUT = revgol
LIB = -lz3
all:
	$(CC) revgol.cpp -o $(OUT) $(LIB)

debug:
	$(CC) revgol.cpp -o $(OUT) -DDEBUG $(LIB)

gol:
	gcc gol.c -o gol

purge:
	rm -f $(OUT) gol
