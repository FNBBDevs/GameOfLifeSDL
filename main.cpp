#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <array>
#include "colors.h"
using namespace std;

#define RATIO 4
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 1400
#define SWS (SCREEN_WIDTH / RATIO)
#define SHS (SCREEN_HEIGHT / RATIO)


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distrib(0, 255);
uniform_int_distribution<> deez(0, 1);

array<array<int, SWS>, SHS> current;
array<array<int, SWS>, SHS> nextgen;

int directions[8][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

auto &colors = copper;


const int ALIVE = int(sizeof(colors) / sizeof(colors[0]));
const int BORN = int(4.5/5.0 * ALIVE);

void randomFill()
{
    for (int i = 0; i < SHS; i++)
    {
        for (int j = 0; j < SWS; j++)
        {
            current[i][j] = (deez(gen)) == 1 ? 0 : BORN;
            nextgen[i][j] = 0;
        }
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

        for (int i = 1; i < SHS-1; i++)
        {
            for (int j = 1; j < SWS-1; j++)
            {
                int neighbors = (current[i + 0][j + 1] >= BORN)
							  + (current[i + 1][j + 0] >= BORN)
							  + (current[i - 1][j + 0] >= BORN)
                              + (current[i + 0][j - 1] >= BORN)
							  + (current[i + 1][j + 1] >= BORN)
                              + (current[i + 1][j - 1] >= BORN)
							  + (current[i - 1][j + 1] >= BORN)
							  + (current[i - 1][j - 1] >= BORN);

                if (current[i][j] >= BORN)
                {
                    if (current[i][j] == ALIVE)
                    {
                        nextgen[i][j] = BORN - 1;
                        //for (int k = 0; k < 8; k++)
                        //        nextgen[i + directions[k][0]][j + directions[k][1]] = BORN;
                    }
                    else
                    {
                        //if (neighbors > 1 && neighbors < 4)
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
                    ((colors[current[i][j]][0] << screen->format->Rshift) & screen->format->Rmask) |
                    ((colors[current[i][j]][1] << screen->format->Gshift) & screen->format->Gmask) |
                    ((colors[current[i][j]][2] << screen->format->Bshift) & screen->format->Bmask) |
                    ((colors[current[i][j]][3] << screen->format->Ashift) & screen->format->Amask);

                
                for (int y = 0; y < RATIO; ++y)
				{
					for (int x = 0; x < RATIO; ++x)
					{
						Uint8 *pixel = (Uint8 *) screen->pixels;
						pixel += ((RATIO * j + y) * screen->pitch) + ((RATIO * i + x) * sizeof(Uint32));
						*((Uint32 *) pixel) = color;
					}
				}
            }
        }
        SDL_UnlockSurface(screen);
        SDL_UpdateWindowSurface(window);
        swap(front, back);
    }

    return 0;
}