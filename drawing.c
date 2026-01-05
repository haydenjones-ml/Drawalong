#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 100;

// Structs for UI elements and drawing app states
typedef struct {
    SDL_Color currColor;
    SDL_Color previewColor;
    bool isDrawing;
    bool isErasing;
    int mouseX, mouseY;
} AppState;

typedef struct {
    SDL_Rect buttonBox;
    SDL_Color buttonColor;
    const char* label;
} UIButton;

// Calculate line interpolation for drawing
// Potential TODO: Implement Bresenham's line algorithm by hand? Might be fun and faster for later.
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

int main(int argc, char* argv[]) {
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (NULL == renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    AppState state = { .currentColor = {0, 0, 0, 255}, .previewColor = {0, 0, 0, 255}, .isDrawing = false, .eraserMode = false };
    
    UIButton btnColor = { {10, 10, BTN_WIDTH, 30}, {100, 100, 100, 255}, "COLOR" };
    UIButton btnErase = { {120, 10, BTN_WIDTH, 30}, {200, 200, 200, 255}, "ERASE" };

    SDL_Event windowEvent;
    bool is_running = true;
    SDL_Event AppEvent;
    // Mouse = Draw; E = Toggle Erase


    while (is_running) {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT) {
                is_running = false;
            }
        } else if(AppEvent.type == SDL_KEYDOWN) {
            if (AppEvent.key.keysym.sym == SDLK_e) {
                appState.isErasing = !appState.isErasing;
            }
        }

        else if (AppEvent.type == SDL_MOUSEBUTTONDOWN) {
            int mx = AppEvent.button.x;
            int my = AppEvent.button.y;

            if (mx >= btnColor.buttonBox.x && mx <= btnColor.buttonBox.x + btnColor.buttonBox.w &&
                my >= btnColor.buttonBox.y && my <= btnColor.buttonBox.y + btnColor.buttonBox.h) {
                // Color button clicked
               int r, g, b;
               printf("Current Color: (%d, %d, %d)\n", state.currentColor.r, state.currentColor.g, state.currentColor.b);
               printf("Enter new RGB values (0-255) separated by spaces (e.g., 255 0 0): ");
               if (scanf("%d %d %d", &r, &g, &b) == 3) {
                   state.currentColor.r = (Uint8)r;
                   state.currentColor.g = (Uint8)g;
                   state.currentColor.b = (Uint8)b;
                   state.previewColor = state.currentColor;
               } else {
                   printf("Invalid input. Color not changed.\n");
               }
            } else if (mx >= btnErase.buttonBox.x && mx <= btnErase.buttonBox.x + btnErase.buttonBox.w &&
                       my >= btnErase.buttonBox.y && my <= btnErase.buttonBox.y + btnErase.buttonBox.h) {
                state.isErasing = !state.isErasing;
            } else {
                // Start drawing
                state.isDrawing = true;
                state.mouseX = mx;
                state.mouseY = my;
            }
        
        // RENDERING CODE HERE
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}