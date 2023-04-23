#include <array>
#include "config.h"
#include <iostream>

std::array<std::array<int, SHS>, SWS> tp;

void average_neighbor(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            int average = current[i][j];
            int neighbors = 1;
            for (int k = 0; k < 8; k++)
            {
                if (i + DIRECTIONS[k][0] >= 0 && i + DIRECTIONS[k][0] < SWS && j + DIRECTIONS[k][1] >= 0 && j + DIRECTIONS[k][1] < SHS)
                {
                    average += current[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]];
                    neighbors++;
                }
            }
            average = (int)average / neighbors;
            display[i][j] = average;
        }
    }
}

void bilinear(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    int scale = 1;
    double xscale = (double)SHS / (SHS - scale);
    double yscale = (double)SWS / (SWS - scale);
    for (int i = 0; i < SWS; i++)
    {
        int iprime = (int)floor(i / xscale);
        for (int j = 0; j < SHS; j++)
        {
            int jprime = (int)floor(j / yscale);
            int coord1 = current[iprime][jprime];
            int coord2 = current[iprime][jprime + 1];
            int coord3 = current[iprime + 1][jprime];
            int coord4 = current[iprime + 1][jprime + 1];
            double fi = i / xscale - iprime;
            double fj = j / yscale - jprime;
            display[i][j] = (1 - fi) * (1 - fj) * coord1 + (1 - fi) * fj * coord2 +
                            fi * (1 - fj) * coord3 + fi * fj * coord4;
        }
    }
}

void bicubic(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    double xscale = (double)SHS / (SHS - 1);
    double yscale = (double)SWS / (SWS - 1);
    // weighted matrix
    auto weighted = [](double x, double y) -> double
    {
        return x * x * y * y * (9 - 6 * x - 6 * y + 4 * x * y);
    };
    for (int i = 0; i < SWS; i++)
    {
        int iprime = (int)(i / xscale);
        for (int j = 0; j < SHS; j++)
        {
            int jprime = (int)(j / yscale);
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

void none(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            display[i][j] = current[i][j];
        }
    }
}

void nulzo(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            if (current[i][j] > current[i - 1][j - 1])
                display[i][j] = (int)((current[i][j] + 1 / (current[i][j]) + 1) * 2);
            else
                display[i][j] = current[i][j];
        }
    }
}

void meteor(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    tp = current;
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            display[i][j] = current[i][j];
            int avg = 0;
            int avgCount = 0;
            for (int k = 0; k < 8; k++)
            {
                avg += display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]];
                if (display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]] != 0)
                {
                    avgCount++;
                }
            }
            tp[i][j] = (avgCount > 0) ? ((avg / avgCount) + current[i][j]) % COLOR_SIZE : display[i][j];
        }
    }
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            int avg = 0;
            int avgCount = 0;
            for (int k = 0; k < 8; k++)
            {
                avg += tp[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]];
            }
            display[i][j] = (avgCount > 0) ? (tp[i][j] + (avg / avgCount)) % COLOR_SIZE : tp[i][j];
        }
    }
}

void cosmicDust(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    std::array<std::array<int, SHS>, SWS> tmp = current;
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            display[i][j] = current[i][j];
            int avg = 0;
            int avgCount = 0;
            for (int k = 0; k < 8; k++)
            {
                avg += display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]];
                if (display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]] != 0)
                {
                    avgCount++;
                }
            }
            display[i][j] = (avgCount > 0) ? ((avg / avgCount) + current[i][j]) % COLOR_SIZE : display[i][j];
        }
    }
}

void solarized(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    std::array<std::array<int, SHS>, SWS> tmp = current;
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            display[i][j] = current[i][j];
            int avg = 0;
            int avgCount = 0;
            for (int k = 0; k < 8; k++)
            {
                avg += display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]];
                if (display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]] != 0)
                {
                    avgCount++;
                }
            }
            for (int k = 0; k < 8; k++)
            {
                display[i + DIRECTIONS[k][0]][j + DIRECTIONS[k][1]] = (avgCount > 0) ? avg / avgCount : BORN;
            }
            display[i][j] = (avgCount > 0) ? ((avg / avgCount) + current[i][j]) % COLOR_SIZE : display[i][j];
        }
    }
}

void erm(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            if (current[i][j] > BORN)
            {
                int avg = 0;
                for(int k = 0; k < 8; k++)
                {
                    avg += current[i+DIRECTIONS[k][0]][j+DIRECTIONS[k][1]];
                }
                display[i][j] = (avg / 8 + current[i][j]) % COLOR_SIZE;
            }
            else
            {
                display[i][j] = COLOR_SIZE - current[i][j];
            }
        }
    }
}
