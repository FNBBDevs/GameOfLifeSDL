#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <array>
#include <functional>
#include "../include/config.h"
#include "../include/interpolations.h"
using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distrib(0, COLOR_SIZE - 1);
uniform_int_distribution<> deez(0, 1);
uniform_int_distribution<> spant(0, 1000000);
array<array<int, SHS>, SWS> current;
array<array<int, SHS>, SWS> nextgen;
array<array<int, SHS>, SWS> display;
array<array<int, SHS>, SWS> tmp;

void randomFill()
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            current[i][j] = (deez(gen)) == 1 ? 0 : ALIVE;
            nextgen[i][j] = 0;
            display[i][j] = 0;
            tmp[i][j] = 0;
        }
    }
}

int main(int argc, char **argv)
{
    SDL_Window *window = nullptr;
    SDL_Surface *screen;
    SDL_Event e;
    window = SDL_CreateWindow("Game of Life", 20, 20, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;

    auto front = &current;
    auto back = &nextgen;

    randomFill();

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }
        auto &current = *front;
        auto &nextgen = *back;
        SDL_LockSurface(screen);

        if (DECAY)
        {
            for (int i = 1; i < SWS - 1; i++)
            {
                for (int j = 1; j < SHS - 1; j++)
                {
                    int neighbors = (current[i + 0][j + 1] >= BORN) + (current[i + 1][j + 0] >= BORN) + (current[i - 1][j + 0] >= BORN) + (current[i + 0][j - 1] >= BORN) + (current[i + 1][j + 1] >= BORN) + (current[i + 1][j - 1] >= BORN) + (current[i - 1][j + 1] >= BORN) + (current[i - 1][j - 1] >= BORN);

                    if (current[i][j] >= BORN)
                    {
                        if (current[i][j] == ALIVE)
                        {
                            nextgen[i][j] = BORN - 1;
                            if (EXPLOSION)
                            {
                                for (int k = 0; k < 8; k++)
                                    nextgen[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]] = BORN;
                            }
                        }
                        else
                        {
                            if (neighbors > 1 && neighbors < 4)
                            {
                                nextgen[i][j] = current[i][j] + 1;
                            }
                            else
                            {
                                nextgen[i][j] = BORN - 1;
                            }
                        }
                    }
                    else
                    {
                        if (neighbors == 3)
                        {
                            nextgen[i][j] = BORN;
                        }
                        else
                        {
                            if (SPANT && spant(gen) == 4200)
                            {
                                nextgen[i][j] = BORN;
                            }
                            else
                            {
                                nextgen[i][j] = (current[i][j] <= 0) ? 0 : current[i][j] - 1;
                            }
                        }
                    }

                    for (int y = 0; y < RATIO; ++y)
                    {
                        for (int x = 0; x < RATIO; ++x)
                        {
                            Uint8 *pixel = (Uint8 *)screen->pixels;
                            pixel += ((RATIO * j + y) * screen->pitch) + ((RATIO * i + x) * sizeof(Uint32));
                            *((Uint32 *)pixel) = colors[display[i][j]];
                        }
                    }
                }
            }
        }
        else
        {
            for (int i = 1; i < SWS - 1; i++)
            {
                for (int j = 1; j < SHS - 1; j++)
                {
                    int neighbors = (current[i + 0][j + 1] > 0) + (current[i + 1][j + 0] > 0) + (current[i - 1][j + 0] > 0) + (current[i + 0][j - 1] > 0) + (current[i + 1][j + 1] > 0) + (current[i + 1][j - 1] > 0) + (current[i - 1][j + 1] > 0) + (current[i - 1][j - 1] > 0); 
                    if (current[i][j] > 0) // ALIVE
                    {
                        if (neighbors > 1 && neighbors < 4)
                        {
                            nextgen[i][j] = COLOR_SIZE-1;
                        }
                        else
                        {
                            nextgen[i][j] = 0;
                        }
                    }
                    else
                    {
                        if (neighbors == 3)
                        {
                            nextgen[i][j] = COLOR_SIZE - 1;
                        }
                        else
                        {
                            nextgen[i][j] = 0;
                        }
                    }

                    for (int y = 0; y < RATIO; ++y)
                    {
                        for (int x = 0; x < RATIO; ++x)
                        {
                            Uint8 *pixel = (Uint8 *)screen->pixels;
                            pixel += ((RATIO * j + y) * screen->pitch) + ((RATIO * i + x) * sizeof(Uint32));
                            *((Uint32 *)pixel) = colors[display[i][j]];
                        }
                    }
                }
            }
        }

        INTERPOLATION(current, display);
        SDL_UnlockSurface(screen);
        SDL_UpdateWindowSurface(window);
        swap(front, back);
    }
    return 0;
}