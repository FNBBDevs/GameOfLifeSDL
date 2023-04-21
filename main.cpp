#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <array>
#include "colors.h"
using namespace std;

#define RATIO 1
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 800
#define SWS (SCREEN_WIDTH / RATIO)
#define SHS (SCREEN_HEIGHT / RATIO)

random_device rd;
mt19937 gen(rd());

auto &colors = amethystReverse;

uniform_int_distribution<> distrib(0, amethystSize - 1);
uniform_int_distribution<> deez(0, 1);

array<array<int, SHS>, SWS> current;
array<array<int, SHS>, SWS> nextgen;
array<array<int, SHS>, SWS> display;

int directions[8][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

const int ALIVE = int(sizeof(colors) / sizeof(colors[0]));
const int BORN = int(4.5 / 5.0 * ALIVE);

void randomFill()
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            current[i][j] = (deez(gen)) == 1 ? 0 : BORN;
            nextgen[i][j] = 0;
            display[i][j] = 0;
        }
    }
}

void nearest_neighbor()
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            int average = current[i][j];
            int neighbors = 1;
            for (int k = 0; k < 8; k++)
            {
                if (i + directions[k][0] >= 0 && i + directions[k][0] < SWS && j + directions[k][1] >= 0 && j + directions[k][1] < SHS)
                {
                    average += current[i + directions[k][0]][j + directions[k][1]];
                    neighbors++;
                }
            }
            average = (int)average / neighbors;
            display[i][j] = average;
        }
    }
}

void bilinear()
{
    return;
}

void none()
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            display[i][j] = current[i][j];
        }
    }
}

void antialias(string type)
{
    if (type == "nearest")
    {
        nearest_neighbor();
    }
    else if (type == "bilinear")
    {
        bilinear();
    }
    else {
        none();
    }
}

int main(int argc, char **argv)
{
    SDL_Window *window = nullptr;
    SDL_Surface *screen;
    SDL_Event e;
    window = SDL_CreateWindow("Game Of life", 20, 20, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
                        // for (int k = 0; k < 8; k++)
                        //         nextgen[i + directions[k][0]][j + directions[k][1]] = BORN;
                    }
                    else
                    {
                        // if (neighbors > 1 && neighbors < 4)
                        if (neighbors > 1 and neighbors < 4)
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
                        nextgen[i][j] = (current[i][j] <= 0) ? 0 : current[i][j] - 1;
                    }
                }

                Uint32 color =
                    ((colors[display[i][j]][0] << screen->format->Rshift) & screen->format->Rmask) |
                    ((colors[display[i][j]][1] << screen->format->Gshift) & screen->format->Gmask) |
                    ((colors[display[i][j]][2] << screen->format->Bshift) & screen->format->Bmask) |
                    ((colors[display[i][j]][3] << screen->format->Ashift) & screen->format->Amask);

                for (int y = 0; y < RATIO; ++y)
                {
                    for (int x = 0; x < RATIO; ++x)
                    {
                        Uint8 *pixel = (Uint8 *)screen->pixels;
                        pixel += ((RATIO * j + y) * screen->pitch) + ((RATIO * i + x) * sizeof(Uint32));
                        *((Uint32 *)pixel) = color;
                    }
                }
            }
        }
        antialias("");
        SDL_UnlockSurface(screen);
        SDL_UpdateWindowSurface(window);
        swap(front, back);
    }

    return 0;
}