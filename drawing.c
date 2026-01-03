#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // 1. Initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL Init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Test Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (NULL == window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    // 2. The Main Loop Setup
    SDL_Event windowEvent;
    bool is_running = true; // Flag to keep the loop going

    // 3. The "Game Loop"
    while (is_running) {
        // Handle ALL pending events for this frame
        while (SDL_PollEvent(&windowEvent)) {
            // Check for the close button (X)
            if (windowEvent.type == SDL_QUIT) {
                is_running = false;
            }
        }
        
        // (Rendering code will go here later)
    }

    // 4. Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}