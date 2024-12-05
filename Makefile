CC = g++
OUT = revgol
LIB = -lz3

all:
	$(CC) revgol.cpp -o $(OUT) $(LIB)
	$(CC) gol.cpp -o gol

dimInclude:
	$(CC) revgol.cpp -o $(OUT) -DDIMS $(LIB)

gol:
	gcc gol.c -o gol

visual:
	gcc visual.c -o visual

purge:
	rm -f $(OUT) gol visual
