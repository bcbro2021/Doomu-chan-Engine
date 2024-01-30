#include <SDL2/SDL.h>

#ifndef WINDOW_H
#define WINDOW_H

// function declarations
SDL_Window* create_win(int width,int height, const char* title);
SDL_Renderer* create_renderer(SDL_Window* window);

void clear_renderer(SDL_Renderer* renderer, int r, int g, int b, int a);

#endif