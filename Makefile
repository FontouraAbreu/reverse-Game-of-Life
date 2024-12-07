CC = g++
OUT = revgol
LIB = -lz3

all: src/revgol.cpp
	$(CC) src/revgol.cpp -o $(OUT) $(LIB)

verbose: src/revgol.cpp
	$(CC) src/revgol.cpp -DVERBOSE -o $(OUT) $(LIB)

gol: auxiliar/gol.c
	gcc auxiliar/gol.c -o gol

visual: auxiliar/visual.c
	gcc auxiliar/visual.c -o visual

rand: auxiliar/randTable.c
	gcc auxiliar/randTable.c -o rand

purge:
	rm -f $(OUT) gol visual rand
