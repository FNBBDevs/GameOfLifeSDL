#include <array>
#include "config.h"

void average_neighbor(std::array<std::array<int, SHS>, SWS>& current, std::array<std::array<int, SHS>, SWS>& display)
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

// https://en.wikipedia.org/wiki/Bilinear_interpolation
void bilinear(std::array<std::array<int, SHS>, SWS>& current, std::array<std::array<int, SHS>, SWS>& display)
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

// https://en.wikipedia.org/wiki/Bicubic_interpolation
void bicubic(std::array<std::array<int, SHS>, SWS>& current, std::array<std::array<int, SHS>, SWS>& display)
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

void none(std::array<std::array<int, SHS>, SWS>& current, std::array<std::array<int, SHS>, SWS>& display)
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            display[i][j] = current[i][j];
        }
    }
}

void nulzo(std::array<std::array<int, SHS>, SWS>& current, std::array<std::array<int, SHS>, SWS>& display)
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