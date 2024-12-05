CC = g++
OUT = revgol
LIB = -lz3

all:
	$(CC) revgol.cpp -o $(OUT) $(LIB)

gol:
	gcc gol.c -o gol

visual:
	gcc visual.c -o visual

purge:
	rm -f $(OUT) gol visual
