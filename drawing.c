#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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
    int brushSize;
} appState;

typedef struct {
    SDL_Rect buttonBox;
    SDL_Color buttonColor;
    const char* label;
} UIButton;

// Calculate line interpolation for drawing
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    // Bresenham's line algo implementation
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);

    float stepX = dx / length;
    float stepY = dy / length;

    for (float i = 0; i <= length; i += 1.0f) {
        int drawX = (int)(x1 + stepX * i);
        int drawY = (int)(y1 + stepY * i);
        SDL_Rect brushRect = { drawX - size / 2, drawY - size / 2, size, size };
        SDL_RenderFillRect(renderer, &brushRect);
    }
    
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
    
    // Note: You created this texture but are currently drawing directly to the renderer in the loop below.
    SDL_Texture *canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (NULL == renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    appState state = { .currColor = {0, 0, 0, 255}, .previewColor = {0, 0, 0, 255}, .isDrawing = false, .isErasing = false };
    UIButton btnColor = { {10, 10, BUTTON_WIDTH, 30}, {100, 100, 100, 255}, "COLOR" };
    UIButton btnErase = { {120, 10, BUTTON_WIDTH, 30}, {200, 200, 200, 255}, "ERASE" };

    bool is_running = true;
    SDL_Event AppEvent; 
    // Mouse = Draw; E = Toggle Erase

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Default brush size; can increment/decrement with keys -/+ respectively
    state.brushSize = 5;

    while (is_running) {
        while (SDL_PollEvent(&AppEvent)) {
            if (AppEvent.type == SDL_QUIT) {
                is_running = false;
            } 
            else if(AppEvent.type == SDL_KEYDOWN) {
                if (AppEvent.key.keysym.sym == SDLK_e) {
                    state.isErasing = !state.isErasing;
                }
            }
            else if (AppEvent.type == SDL_MOUSEBUTTONDOWN) {
                int mx = AppEvent.button.x;
                int my = AppEvent.button.y;

                if (mx >= btnColor.buttonBox.x && mx <= btnColor.buttonBox.x + btnColor.buttonBox.w &&
                    my >= btnColor.buttonBox.y && my <= btnColor.buttonBox.y + btnColor.buttonBox.h) {
                    // Color button clicked
                    int r, g, b;
                    printf("Current Color: (%d, %d, %d)\n", state.currColor.r, state.currColor.g, state.currColor.b);
                    printf("Enter new RGB values (0-255) separated by spaces (e.g., 255 0 0): ");

                    // ^ TODO: Replace with prompt window? + preview within prompt window
                    
                    if (scanf("%d %d %d", &r, &g, &b) == 3) {
                        state.currColor.r = (Uint8)r;
                        state.currColor.g = (Uint8)g;
                        state.currColor.b = (Uint8)b;
                        state.previewColor = state.currColor;
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
            }
            else if (AppEvent.type == SDL_MOUSEBUTTONUP) {
                state.isDrawing = false;
            }
            else if (AppEvent.type == SDL_MOUSEMOTION) {
                if (state.isDrawing) {
                    int mx = AppEvent.motion.x;
                    int my = AppEvent.motion.y;
                    SDL_Color drawColor = state.isErasing ? (SDL_Color){255, 255, 255, 255} : state.currColor;
                    
                    drawLine(renderer, state.mouseX, state.mouseY, mx, my, state.brushSize, drawColor);
                    
                    state.mouseX = mx;
                    state.mouseY = my;
                    SDL_RenderPresent(renderer);
                }
            } 
        }
        SDL_SetRenderDrawColor(renderer, btnColor.buttonColor.r, btnColor.buttonColor.g, btnColor.buttonColor.b, 255);
        SDL_RenderFillRect(renderer, &btnColor.buttonBox);

        SDL_SetRenderDrawColor(renderer, btnErase.buttonColor.r, btnErase.buttonColor.g, btnErase.buttonColor.b, 255);
        SDL_RenderFillRect(renderer, &btnErase.buttonBox);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}