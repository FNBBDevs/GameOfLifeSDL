#pragma once
#include "colors.h"
#include <string>

constexpr auto &colors = ocean;
constexpr int COLOR_SIZE = (int) (sizeof(colors) / sizeof(colors[0]));
constexpr int RATIO = 1;
constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 1000;
constexpr int SWS = SCREEN_WIDTH / RATIO;
constexpr int SHS = SCREEN_HEIGHT / RATIO;
constexpr int DIRECTIONS[][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
constexpr int ALIVE = COLOR_SIZE-1;
//constexpr int BORN = int(5.0/ 100.0 * ALIVE);
constexpr int BORN = int(4.0/ 5.0 * ALIVE);
constexpr int BLUR = 10;
constexpr int BOCA = 2;
constexpr bool EXPLOSION = false;
constexpr bool SPANT = false;

#include "interpolations.h"
constexpr auto INTERPOLATION = none;
