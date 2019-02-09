#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"

#define WIDTH 512
#define HEIGHT 512

#define FPS 120

#define MAX(x, y) x > y ? x : y
#define MIN(x, y) x < y ? x : y

void render(SDL_Renderer *renderer);
void parse_events(SDL_Event *event);

void randomize_map();
void draw_grid(SDL_Renderer *renderer);
void draw_map(SDL_Renderer *renderer);
void propagate();

#endif