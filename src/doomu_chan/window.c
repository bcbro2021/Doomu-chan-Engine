#include "window.h"
#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Window* create_win(int width,int height, const char* title) {
    if ((SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed. SDL Error: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window creation failed. SDL Error: %s\n", SDL_GetError());
    }

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer creation failed. SDL Error: %s\n", SDL_GetError());
    }

    return renderer;
}

void clear_renderer(SDL_Renderer* renderer, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}