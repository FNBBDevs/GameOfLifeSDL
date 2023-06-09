#pragma once
#include <array>
#include "config.h"
#include "kernels.h"
#include <iostream>

std::array<std::array<int, SHS>, SWS> tp;

// ============================= Averaging ====================================

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

// =========================== Interpolation ==================================

void bilinear(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    int scale = 1;
    double xscale = (double)SHS / (SHS - scale);
    double yscale = (double)SWS / (SWS - scale);
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

// ============================== Kernels ======================================

void gaussianBlur(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    int sum;
    sum = 0;
    int halfSize = (GAUSSIAN3SIZE / 2);
    for (int i = 2; i < SHS - 2; i++)
    {
        for (int j = 2; j < SWS - 2; j++)
        {
            int sum = 0;
            for (int kernelI = -halfSize; kernelI <= halfSize; kernelI++)
            {
                for (int kernelJ = -halfSize; kernelJ <= halfSize; kernelJ++)
                {
                    double kernalValue = GAUSSIAN3[kernelI][kernelJ];
                    sum += int(current[i + kernelI][j + kernelJ] * kernalValue);
                }
            }
            display[i][j] = sum > COLOR_SIZE ? COLOR_SIZE : sum;
        }
    }
}

void laplacianEdges(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    int halfX = (int)(5 / 2);
    int halfY = halfX;
    for (int i = 2; i < SHS - 5; i++)
    {
        for (int j = 2; j < SWS - 5; j++)
        {
            int sum = 0;
            for (int n = -halfY; n < halfY; n++)
            {
                for (int m = -halfX; m < halfX; m++)
                {
                    int kval = laplacian5[m + 1][n + 1];
                    sum += current[i - m][j - n] * kval;
                }
            }
            sum = std::max(sum, 0);
            if( i > 1 and i < SHS / 2)
            display[i][j] = sum > COLOR_SIZE ? COLOR_SIZE : sum;
            else display[i][j] = current[i][j];
        }
    }
}

void sobelEdges(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    int halfX = (int)(3 / 2);
    int halfY = halfX;
    for (int i = 2; i < SHS - 5; i++)
    {
        for (int j = 2; j < SWS - 5; j++)
        {
            int vMaskSum = 0;
            int hMaskSum = 0;
            for (int n = -halfY; n < halfY; n++)
            {
                for (int m = -halfX; m < halfX; m++)
                {
                    int kernelValueV = sobel1[m + 1][n + 1];
                    vMaskSum += current[i - m][j - n] * kernelValueV;
                    int kernelValueH = sobel2[m + 1][n + 1];
                    hMaskSum += current[i - m][j - n] * kernelValueH;
                }
            }
            int sum = vMaskSum + hMaskSum;
            sum = std::max(sum, 0);
            if( i > 1 and i < SHS / 2)
            display[i][j] = sum > COLOR_SIZE ? COLOR_SIZE : sum;
            else display[i][j] = current[i][j];
        }
    }
}

// ========================== Visual Effects ===================================

void none(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    display = current;
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

void dispersion(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &buffered)
{
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            int smoothness = 0;
            if (current[i][j] >= BORN)
                buffered[i][j] = 0;
            else if (current[i][j] == 0)
            {
                int maxNeighbor = std::max({buffered[i - 1][j], buffered[i + 1][j], buffered[i][j - 1], buffered[i][j + 1], buffered[i - 1][j - 1], buffered[i + 1][j - 1], buffered[i - 1][j + 1], buffered[i + 1][j + 1]});
                if (maxNeighbor > 0 && current[i][j] != BORN)
                {
                    buffered[i][j] = (int)(maxNeighbor / 1.1);
                }
            }
            else
                buffered[i][j] = current[i][j];

            smoothness += ((buffered[i - 1][j] + buffered[i + 1][j] + buffered[i][j - 1] + buffered[i][j + 1] + buffered[i - 1][j - 1] + buffered[i + 1][j - 1] + buffered[i - 1][j + 1] + buffered[i + 1][j + 1])) / 8;

            if (current[i][j] != buffered[i][j])
                buffered[i][j] = (int)((smoothness + buffered[i][j] - 1) / 2);
            else
                buffered[i][j] = current[i][j];
        }
    }
}

void dispersion2(std::array<std::array<int, SHS>, SWS> &buffered, std::array<std::array<int, SHS>, SWS> &temp)
{
    for (int i = 0; i < SWS; i++)
    {
        for (int j = 0; j < SHS; j++)
        {
            if (i > 10 && i < SWS - 10 && j > 10 && j < SHS - 10)
            {
                int smoothness;
                smoothness = 0;
                int maxNeighbor = std::max({buffered[i - 1][j], buffered[i + 1][j], buffered[i][j - 1], buffered[i][j + 1], buffered[i - 1][j - 1], buffered[i + 1][j - 1], buffered[i - 1][j + 1], buffered[i + 1][j + 1]});
                if ((buffered[i][j] <= BORN) && (buffered[i][j] > 0))
                {
                    temp[i][j] = (int)(maxNeighbor / 1.5);
                }
                if (temp[i][j] == buffered[i][j])
                {
                    temp[i][j] = 0;
                }
                else
                {
                    if(buffered[i][j] < ALIVE && buffered[i][j] > BORN) temp[i][j] = (int)(maxNeighbor * 1.2) % COLOR_SIZE;
                    else temp[i][j] = 0;
                }
                //smoothness += ((buffered[i - 1][j] + buffered[i + 1][j] + buffered[i][j - 1] + buffered[i][j + 1] + buffered[i - 1][j - 1] + buffered[i + 1][j - 1] + buffered[i - 1][j + 1] + buffered[i + 1][j + 1])) / 8;
            }
            else
            {
                temp[i][j] = 0;
            }
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
    std::array<std::array<int, SHS>, SWS> tmp;
    tmp = current;
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
    std::array<std::array<int, SHS>, SWS> tmp;
    tmp = current;
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

void lensFlare(std::array<std::array<int, SHS>, SWS> &current, std::array<std::array<int, SHS>, SWS> &display)
{
    for (int i = 1; i < SWS - 1; i++)
    {
        for (int j = 1; j < SHS - 1; j++)
        {
            int avg = 0;
            int bounds = (i > BLUR && i < SWS - BLUR) && (j > BLUR && j < SHS - BLUR);
            if (bounds)
            {
                for (int k = 1; k <= BLUR; k++)
                {
                    avg += ((current[i - k][j] + current[i + k][j] + current[i][j - k] + current[i][j + k] + current[i - k][j - k] + current[i + k][j - k] + current[i - k][j + k] + current[i + k][j + k]) / BOCA) % COLOR_SIZE;
                }
                display[i][j] = avg / BLUR;
            }

            if ((current[i][j] <= BORN && current[i][j] >= BORN / 2) && bounds)
                display[i][j] = current[i][j];
            if ((current[i][j] >= BORN) && bounds)
                display[i][j] = ALIVE;
        }
    }
}