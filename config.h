#pragma once
#include "colors.h"
#include <string>

constexpr auto &colors = prideReverse;
constexpr int COLOR_SIZE = (int) (sizeof(colors) / sizeof(colors[0]));
constexpr int RATIO = 2;
constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 1000;
constexpr int SWS = SCREEN_WIDTH / RATIO;
constexpr int SHS = SCREEN_HEIGHT / RATIO;
constexpr int DIRECTIONS[][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
constexpr int ALIVE = COLOR_SIZE-1;
constexpr int BORN = int(4.0 / 5.0 * ALIVE);
constexpr bool EXPLOSION = false;

/* 
This include is here because interpolations.h is dependent on the above variables.
I am not a C++ dev.
*/
#include "interpolations.h"
constexpr auto INTERPOLATION = solarized;
