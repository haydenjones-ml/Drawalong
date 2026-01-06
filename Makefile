all:
	gcc -I src/include -L src/lib -o test drawing.c -lmingw32 -lSDL2main -lSDL2 -lm
	# new makefile after winsock implmenetation: 	gcc -I src/include -L src/lib -o test drawing.c net.c -lmingw32 -lSDL2main -lSDL2 -lm