#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"
#include "map.h"

#define TILE_WIDTH WIDTH / MAP_W
#define TILE_HEIGHT HEIGHT / MAP_H

int mouse_pressed = 0, key_pressed = 0, key = 0;
int running = 0;
long ticks = 0;
SDL_Point mouse_pos;
SDL_Point target;

long propagation_time = 0;
int propagation_delay = 10;

SDL_Point A[MAP_SIZE], B[4];
int A_i = 0;
int i = 0;

_map map, buffer;

void init()
{
    m_init(map, 0);
    m_init(buffer, 0);
    randomize_map();
    target.x = target.y = 0;
}

void draw_grid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int x = 0; x <= MAP_W; x++)
        SDL_RenderDrawLine(renderer, x * (TILE_WIDTH), 0, x * (TILE_WIDTH), HEIGHT);
    for (int y = 0; y <= MAP_H; y++)
        SDL_RenderDrawLine(renderer, 0, y * (TILE_HEIGHT), WIDTH, y * (TILE_HEIGHT));
}

void randomize_map()
{
}

void draw_map(SDL_Renderer *renderer)
{
    for (int y = 0; y < MAP_H; y++)
    {
        for (int x = 0; x < MAP_W; x++)
        {
            SDL_Rect rect;
            rect.x = x * (TILE_WIDTH);
            rect.y = y * (TILE_HEIGHT);
            rect.w = (TILE_WIDTH);
            rect.h = (TILE_HEIGHT);
            if (map[m_pos(x, y)] == -1)
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            else
            {
                float val = buffer[m_pos(x, y)];
                if (val == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }
                else
                {
                    float ratio = (val / (MAP_W + MAP_H));
                    float r = 0;
                    float g = 0;
                    float b = 255;
                    SDL_SetRenderDrawColor(renderer, (int)r, (int)g, (int)b, 255);
                }
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void propagate()
{

    if (i == A_i)
        return;

    int B_i = 0;
    do
    {
        SDL_Point p = A[i];
        if (buffer[m_pos(p.x, p.y - 1)] == 0 && p.y - 1 >= 0)
        {
            buffer[m_pos(p.x, p.y - 1)] = buffer[m_pos(p.x, p.y)] + 1;
            SDL_Point np;
            np.x = p.x;
            np.y = p.y - 1;
            B[B_i++] = np;
        }
        if (buffer[m_pos(p.x + 1, p.y)] == 0 && p.x + 1 < MAP_W)
        {
            buffer[m_pos(p.x + 1, p.y)] = buffer[m_pos(p.x, p.y)] + 1;
            SDL_Point np;
            np.x = p.x + 1;
            np.y = p.y;
            B[B_i++] = np;
        }
        if (buffer[m_pos(p.x, p.y + 1)] == 0 && p.y + 1 < MAP_H)
        {
            buffer[m_pos(p.x, p.y + 1)] = buffer[m_pos(p.x, p.y)] + 1;
            SDL_Point np;
            np.x = p.x;
            np.y = p.y + 1;
            B[B_i++] = np;
        }
        if (buffer[m_pos(p.x - 1, p.y)] == 0 && p.x - 1 >= 0)
        {
            buffer[m_pos(p.x - 1, p.y)] = buffer[m_pos(p.x, p.y)] + 1;
            SDL_Point np;
            np.x = p.x - 1;
            np.y = p.y;
            B[B_i++] = np;
        }
        for (int j = 0; j < B_i; j++)
        {
            A[A_i++] = B[j];
        }
        i++;
    } while (B_i == 0 && A_i != i);
}

void render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    propagate();
    draw_map(renderer);
    //draw_grid(renderer);

    SDL_RenderPresent(renderer);
}

void parse_events(SDL_Event *event)
{
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
            running = 0;
        else if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            mouse_pressed = 1;
            m_copy(map, buffer);
            target.x = mouse_pos.x / (TILE_WIDTH);
            target.y = mouse_pos.y / (TILE_HEIGHT);
            printf("%d\n", target.x);
            A_i = i = 0;
            A[A_i++] = target;
            buffer[m_pos(target.x, target.y)] = 1;
        }
        else if (event->type == SDL_MOUSEBUTTONUP)
            mouse_pressed = 0;
        else if (event->type == SDL_KEYDOWN)
        {
            key_pressed = 1;
            key = event->key.keysym.sym;
            if (key == SDLK_r)
            {
                m_init(buffer, 0);
                A_i = i;
            }
            else
            {
                int x = mouse_pos.x / (TILE_WIDTH);
                int y = mouse_pos.y / (TILE_HEIGHT);
                int val = map[m_pos(x, y)];
                if (key == SDLK_c)
                    map[m_pos(x, y)] = 0;
                else
                    map[m_pos(x, y)] = -1;
            }
        }
        else if (event->type == SDL_KEYUP)
        {
            key_pressed = 0;
            key = event->key.keysym.sym;
        }
    }
}

int main(int argv, char *args[])
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);
    init();
    running = 1;
    SDL_Event event;
    while (running)
    {
        parse_events(&event);
        if (ticks + 1000 / FPS > SDL_GetTicks())
            continue;
        ticks = SDL_GetTicks();
        render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}