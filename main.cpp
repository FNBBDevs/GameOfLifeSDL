#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <array>
#include "colors.h"
using namespace std;

#define RATIO 2
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define SWS (SCREEN_WIDTH / RATIO)
#define SHS (SCREEN_HEIGHT / RATIO)

random_device rd;
mt19937 gen(rd());

auto &colors = flamingo;
uniform_int_distribution<> distrib(0, flamingoSize - 1);
uniform_int_distribution<> deez(0, 1);

array<array<int, SHS>, SWS> current;
array<array<int, SHS>, SWS> nextgen;
array<array<int, SHS>, SWS> display;

int directions[8][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

const int ALIVE = int(sizeof(colors) / sizeof(colors[0]));
const int BORN = int(4.5 / 5.0 * ALIVE);
const bool EXPLOSION = false;

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

void average_neighgbor()
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

// https://en.wikipedia.org/wiki/Bilinear_interpolation
void bilinear() {
    int scale = 1;
    double xscale = (double)SHS / (SHS-scale);
    double yscale = (double)SWS / (SWS-scale);
    for (int i = 0; i < SWS; i++) {
        int iprime = (int)floor (i / xscale);
        for (int j = 0; j < SHS; j++) {
            int jprime = (int)floor (j / yscale);
            int coord1 = current[iprime][jprime];
            int coord2 = current[iprime][jprime+1];
            int coord3 = current[iprime+1][jprime];
            int coord4 = current[iprime + 1][jprime + 1];
            double fi = i / xscale - iprime;
            double fj = j / yscale - jprime;
            display[i][j] = (1 - fi) * (1 - fj) * coord1 + (1 - fi) * fj * coord2 +
            fi * (1 - fj) * coord3 + fi * fj * coord4;
        }
    }
}

// https://en.wikipedia.org/wiki/Bicubic_interpolation
void bicubic(){
    double xscale = (double)SHS / (SHS - 1);
    double yscale = (double)SWS / (SWS - 1);  
    // weighted matrix  
    auto weighted = [](double x, double y) -> double {
      return x * x * y * y * (9 - 6 * x - 6 * y + 4 * x * y);
    };
    for (int i = 0; i < SWS; i++) { 
        int iprime = (int) (i / xscale);  
        for (int j = 0; j < SHS; j++) {   
            int jprime = (int) (j / yscale);
            int coord1 = current[iprime][jprime];
            int coord2 = current[iprime][jprime + 1];
            int coord3 = current[iprime + 1][jprime];
            int coord4 = current[iprime + 1][jprime + 1]; 
            double fi = i / xscale - iprime;
            double fj = j / yscale - jprime;  
            display[i][j] = weighted(1 - fi, 1 - fj) * coord1 + weighted(1 - fi, fj) * coord2 +
                weighted(fi, 1 - fj) * coord3 + weighted(fi, fj) * coord4;
        }
    }
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

void nulzo(){

    for (int i=0; i<SHS-1; i++){
        for (int j=0; j<SWS-1; j++){
            if(current[i][j] > current[i+1][j+1]) display[i+1][j+1] = (int)((current[i][j] + 1 / (current[i+1][j+1]) + 1) * 2);
            if(current[i][j] > current[i-1][j-1]) display[i-1][j-1] = (int)((current[i][j] + 1 / (current[i-1][j-1]) + 1) * 2);
            else display[i][j] = current[i][j];
        }
    }

}

constexpr unsigned int strhash(const char *s, int offset=0){
    return !s[offset] ? 5381 : (strhash(s, offset+1)*33) ^ s[offset];
}

void antialias(const char *antialiasType)
{
    switch(strhash(antialiasType)){
        case strhash("navg"): average_neighgbor(); break;
        case strhash("bilinear"): bilinear(); break;
        case strhash("bicubic"): bicubic(); break;
        case strhash("nulzo"): nulzo(); break;
        default: none();
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
                        if(EXPLOSION) {
                        for (int k = 0; k < 8; k++)
                                nextgen[i + directions[k][0]][j + directions[k][1]] = BORN;
                        }
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
        antialias("nulzo");
        SDL_UnlockSurface(screen);
        SDL_UpdateWindowSurface(window);
        swap(front, back);
    }
    return 0;
}