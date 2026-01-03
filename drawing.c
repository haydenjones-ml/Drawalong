#include <SDL2/SDL.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Test Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (NULL == window) {
        printf("Failed to create window: ");
    }
    SDL_Event windowEvent;

    while(SDL_PollEvent(&windowEvent) > 0) {
        if (SDL_PollEvent(&windowEvent)) {
            if (SDL_QUIT == windowEvent.type) {
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}