all:
	gcc -I src/include -L src/lib main.c net.c -o drawalong.exe -lmingw32 -lSDL2main -lSDL2 -lws2_32