all:
	gcc -I src/include -L src/lib -o test drawing.c -lmingw32 -lSDL2main -lSDL2