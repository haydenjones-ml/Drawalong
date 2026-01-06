all:
	gcc -I src/include -L src/lib net.h -o  test drawing.c -lmingw32 -lSDL2main -lSDL2 -lm