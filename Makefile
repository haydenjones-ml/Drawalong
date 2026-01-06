all:
	gcc -I src/include -L src/lib drawing.c net.c -o test.exe -lmingw32 -lSDL2main -lSDL2 -lws2_32