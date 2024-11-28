CC = g++
OUT = gol
LIB = -lz3
all:
	$(CC) main.cpp -o $(OUT) $(LIB)

ex:
	gcc exgol.c -o exgol

purge:
	rm $(OUT)
