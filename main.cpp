#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <array>
using namespace std;

#define RATIO 1
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

int colors[][4] = {
        { 148, 241, 243, 255 },
        { 146, 240, 243, 255 },
        { 145, 239, 242, 255 },
        { 143, 238, 242, 255 },
        { 142, 237, 242, 255 },
        { 141, 236, 241, 255 },
        { 139, 235, 241, 255 },
        { 138, 233, 240, 255 },
        { 137, 232, 240, 255 },
        { 135, 231, 240, 255 },
        { 134, 230, 239, 255 },
        { 132, 229, 239, 255 },
        { 131, 228, 238, 255 },
        { 130, 227, 238, 255 },
        { 128, 226, 238, 255 },
        { 127, 224, 237, 255 },
        { 125, 223, 237, 255 },
        { 124, 222, 237, 255 },
        { 123, 221, 236, 255 },
        { 121, 220, 236, 255 },
        { 120, 219, 236, 255 },
        { 118, 218, 235, 255 },
        { 117, 217, 235, 255 },
        { 116, 216, 235, 255 },
        { 114, 214, 234, 255 },
        { 113, 213, 234, 255 },
        { 111, 212, 234, 255 },
        { 110, 211, 233, 255 },
        { 108, 210, 233, 255 },
        { 107, 209, 233, 255 },
        { 106, 208, 232, 255 },
        { 104, 207, 232, 255 },
        { 103, 206, 232, 255 },
        { 101, 205, 232, 255 },
        { 100, 204, 231, 255 },
        { 98, 203, 231, 255 },
        { 97, 202, 231, 255 },
        { 95, 200, 231, 255 },
        { 94, 199, 230, 255 },
        { 92, 198, 230, 255 },
        { 91, 197, 230, 255 },
        { 90, 196, 230, 255 },
        { 88, 195, 229, 255 },
        { 87, 194, 229, 255 },
        { 85, 193, 229, 255 },
        { 84, 192, 229, 255 },
        { 82, 191, 229, 255 },
        { 81, 190, 228, 255 },
        { 79, 189, 228, 255 },
        { 78, 188, 228, 255 },
        { 76, 187, 228, 255 },
        { 75, 186, 228, 255 },
        { 73, 185, 227, 255 },
        { 72, 183, 227, 255 },
        { 70, 182, 227, 255 },
        { 69, 181, 227, 255 },
        { 67, 180, 227, 255 },
        { 66, 179, 227, 255 },
        { 64, 178, 227, 255 },
        { 63, 177, 226, 255 },
        { 61, 176, 226, 255 },
        { 60, 175, 226, 255 },
        { 58, 174, 226, 255 },
        { 56, 173, 226, 255 },
        { 55, 172, 226, 255 },
        { 53, 171, 226, 255 },
        { 52, 170, 226, 255 },
        { 50, 169, 225, 255 },
        { 49, 168, 225, 255 },
        { 47, 166, 225, 255 },
        { 46, 165, 225, 255 },
        { 44, 164, 225, 255 },
        { 43, 163, 225, 255 },
        { 42, 162, 225, 255 },
        { 40, 161, 225, 255 },
        { 39, 160, 225, 255 },
        { 37, 159, 225, 255 },
        { 36, 158, 224, 255 },
        { 35, 157, 224, 255 },
        { 33, 156, 224, 255 },
        { 32, 154, 224, 255 },
        { 31, 153, 224, 255 },
        { 30, 152, 224, 255 },
        { 29, 151, 224, 255 },
        { 28, 150, 224, 255 },
        { 27, 149, 224, 255 },
        { 26, 148, 224, 255 },
        { 25, 147, 223, 255 },
        { 25, 145, 223, 255 },
        { 24, 144, 223, 255 },
        { 24, 143, 223, 255 },
        { 24, 142, 223, 255 },
        { 24, 141, 223, 255 },
        { 24, 140, 222, 255 },
        { 24, 139, 222, 255 },
        { 24, 137, 222, 255 },
        { 25, 136, 222, 255 },
        { 25, 135, 221, 255 },
        { 26, 134, 221, 255 },
        { 27, 133, 221, 255 },
        { 28, 131, 220, 255 },
        { 29, 130, 220, 255 },
        { 30, 129, 220, 255 },
        { 31, 128, 219, 255 },
        { 33, 127, 219, 255 },
        { 34, 125, 218, 255 },
        { 35, 124, 217, 255 },
        { 37, 123, 217, 255 },
        { 38, 122, 216, 255 },
        { 39, 121, 215, 255 },
        { 41, 119, 214, 255 },
        { 42, 118, 213, 255 },
        { 43, 117, 212, 255 },
        { 45, 116, 211, 255 },
        { 46, 115, 210, 255 },
        { 47, 113, 209, 255 },
        { 48, 112, 207, 255 },
        { 50, 111, 206, 255 },
        { 51, 110, 205, 255 },
        { 52, 109, 203, 255 },
        { 53, 108, 202, 255 },
        { 54, 107, 200, 255 },
        { 54, 106, 199, 255 },
        { 55, 105, 197, 255 },
        { 56, 104, 195, 255 },
        { 57, 103, 194, 255 },
        { 57, 102, 192, 255 },
        { 58, 101, 190, 255 },
        { 58, 100, 188, 255 },
        { 59, 99, 186, 255 },
        { 59, 98, 185, 255 },
        { 60, 97, 183, 255 },
        { 60, 96, 181, 255 },
        { 60, 95, 179, 255 },
        { 60, 94, 177, 255 },
        { 61, 93, 175, 255 },
        { 61, 92, 173, 255 },
        { 61, 91, 172, 255 },
        { 61, 90, 170, 255 },
        { 61, 90, 168, 255 },
        { 61, 89, 166, 255 },
        { 61, 88, 164, 255 },
        { 61, 87, 162, 255 },
        { 61, 86, 160, 255 },
        { 61, 85, 158, 255 },
        { 61, 84, 156, 255 },
        { 61, 84, 155, 255 },
        { 61, 83, 153, 255 },
        { 61, 82, 151, 255 },
        { 60, 81, 149, 255 },
        { 60, 80, 147, 255 },
        { 60, 79, 145, 255 },
        { 60, 79, 143, 255 },
        { 60, 78, 142, 255 },
        { 59, 77, 140, 255 },
        { 59, 76, 138, 255 },
        { 59, 75, 136, 255 },
        { 59, 75, 134, 255 },
        { 58, 74, 132, 255 },
        { 58, 73, 131, 255 },
        { 58, 72, 129, 255 },
        { 57, 71, 127, 255 },
        { 57, 71, 125, 255 },
        { 56, 70, 124, 255 },
        { 56, 69, 122, 255 },
        { 56, 68, 120, 255 },
        { 55, 67, 118, 255 },
        { 55, 67, 117, 255 },
        { 54, 66, 115, 255 },
        { 54, 65, 113, 255 },
        { 54, 64, 112, 255 },
        { 53, 63, 110, 255 },
        { 53, 63, 108, 255 },
        { 52, 62, 106, 255 },
        { 52, 61, 105, 255 },
        { 51, 60, 103, 255 },
        { 51, 60, 102, 255 },
        { 50, 59, 100, 255 },
        { 50, 58, 98, 255 },
        { 49, 57, 97, 255 },
        { 49, 56, 95, 255 },
        { 48, 56, 93, 255 },
        { 48, 55, 92, 255 },
        { 47, 54, 90, 255 },
        { 47, 53, 89, 255 },
        { 46, 52, 87, 255 },
        { 45, 52, 86, 255 },
        { 45, 51, 84, 255 },
        { 44, 50, 82, 255 },
        { 44, 49, 81, 255 },
        { 43, 49, 79, 255 },
        { 42, 48, 78, 255 },
        { 42, 47, 76, 255 },
        { 41, 46, 75, 255 },
        { 41, 45, 73, 255 },
        { 40, 45, 72, 255 },
        { 39, 44, 70, 255 },
        { 39, 43, 69, 255 },
        { 38, 42, 67, 255 },
        { 38, 41, 66, 255 },
        { 37, 41, 64, 255 },
        { 36, 40, 63, 255 },
        { 36, 39, 62, 255 },
        { 35, 38, 60, 255 },
        { 34, 37, 59, 255 },
        { 34, 37, 57, 255 },
        { 33, 36, 56, 255 },
        { 32, 35, 54, 255 },
        { 31, 34, 53, 255 },
        { 31, 33, 52, 255 },
        { 30, 33, 50, 255 },
        { 29, 32, 49, 255 },
        { 29, 31, 47, 255 },
        { 28, 30, 46, 255 },
        { 27, 29, 45, 255 },
        { 26, 28, 43, 255 },
        { 26, 28, 42, 255 },
        { 25, 27, 41, 255 },
        { 24, 26, 39, 255 },
        { 23, 25, 38, 255 },
        { 23, 24, 37, 255 },
        { 22, 23, 35, 255 },
        { 21, 22, 34, 255 },
        { 20, 22, 32, 255 },
        { 20, 21, 31, 255 },
        { 19, 20, 30, 255 },
        { 18, 19, 28, 255 },
        { 17, 18, 27, 255 },
        { 16, 17, 26, 255 },
        { 15, 16, 24, 255 },
        { 15, 15, 23, 255 },
        { 14, 15, 22, 255 },
        { 13, 14, 21, 255 },
        { 12, 13, 19, 255 },
        { 11, 12, 18, 255 },
        { 10, 11, 17, 255 },
        { 9, 10, 15, 255 },
        { 9, 9, 14, 255 },
        { 8, 8, 13, 255 },
        { 7, 7, 11, 255 },
        { 6, 6, 10, 255 },
        { 5, 6, 9, 255 },
        { 5, 5, 7, 255 },
        { 4, 4, 6, 255 },
        { 4, 4, 5, 255 },
        { 3, 3, 4, 255 },
        { 2, 3, 4, 255 },
        { 2, 2, 3, 255 },
        { 2, 2, 2, 255 },
        { 1, 1, 2, 255 },
        { 1, 1, 1, 255 },
        { 1, 1, 1, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 0, 0, 0, 255 },
        { 1, 1, 1, 255 },
        { 1, 1, 1, 255 },
        { 2, 1, 1, 255 },
        { 2, 1, 2, 255 },
        { 3, 2, 2, 255 },
        { 4, 2, 2, 255 },
        { 5, 3, 3, 255 },
        { 6, 3, 3, 255 },
        { 7, 3, 4, 255 },
        { 8, 4, 5, 255 },
        { 9, 5, 5, 255 },
        { 11, 5, 6, 255 },
        { 12, 6, 7, 255 },
        { 13, 6, 7, 255 },
        { 15, 7, 8, 255 },
        { 16, 8, 9, 255 },
        { 17, 8, 10, 255 },
        { 19, 9, 11, 255 },
        { 20, 10, 12, 255 },
        { 21, 10, 13, 255 },
        { 23, 11, 13, 255 },
        { 24, 12, 14, 255 },
        { 25, 13, 15, 255 },
        { 27, 13, 16, 255 },
        { 28, 14, 17, 255 },
        { 29, 15, 17, 255 },
        { 31, 15, 18, 255 },
        { 32, 16, 19, 255 },
        { 33, 17, 20, 255 },
        { 34, 17, 20, 255 },
        { 36, 18, 21, 255 },
        { 37, 19, 22, 255 },
        { 38, 19, 22, 255 },
        { 40, 20, 23, 255 },
        { 41, 20, 24, 255 },
        { 42, 21, 24, 255 },
        { 44, 22, 25, 255 },
        { 45, 22, 26, 255 },
        { 46, 23, 26, 255 },
        { 48, 23, 27, 255 },
        { 49, 24, 28, 255 },
        { 50, 25, 28, 255 },
        { 52, 25, 29, 255 },
        { 53, 26, 29, 255 },
        { 54, 26, 30, 255 },
        { 56, 27, 31, 255 },
        { 57, 27, 31, 255 },
        { 58, 28, 32, 255 },
        { 60, 28, 32, 255 },
        { 61, 29, 33, 255 },
        { 62, 29, 33, 255 },
        { 64, 30, 34, 255 },
        { 65, 31, 34, 255 },
        { 66, 31, 35, 255 },
        { 68, 32, 35, 255 },
        { 69, 32, 36, 255 },
        { 70, 33, 36, 255 },
        { 72, 33, 37, 255 },
        { 73, 34, 37, 255 },
        { 74, 34, 37, 255 },
        { 76, 35, 38, 255 },
        { 77, 35, 38, 255 },
        { 78, 35, 39, 255 },
        { 80, 36, 39, 255 },
        { 81, 36, 39, 255 },
        { 82, 37, 40, 255 },
        { 84, 37, 40, 255 },
        { 85, 38, 40, 255 },
        { 86, 38, 41, 255 },
        { 88, 39, 41, 255 },
        { 89, 39, 41, 255 },
        { 90, 40, 42, 255 },
        { 92, 40, 42, 255 },
        { 93, 41, 42, 255 },
        { 95, 41, 42, 255 },
        { 96, 42, 43, 255 },
        { 97, 42, 43, 255 },
        { 99, 43, 43, 255 },
        { 100, 43, 43, 255 },
        { 101, 44, 43, 255 },
        { 103, 44, 43, 255 },
        { 104, 44, 44, 255 },
        { 105, 45, 44, 255 },
        { 107, 45, 44, 255 },
        { 108, 46, 44, 255 },
        { 109, 46, 44, 255 },
        { 111, 47, 44, 255 },
        { 112, 47, 44, 255 },
        { 114, 48, 44, 255 },
        { 115, 48, 44, 255 },
        { 116, 49, 44, 255 },
        { 118, 49, 44, 255 },
        { 119, 50, 44, 255 },
        { 120, 50, 44, 255 },
        { 122, 51, 44, 255 },
        { 123, 51, 44, 255 },
        { 124, 52, 44, 255 },
        { 126, 52, 44, 255 },
        { 127, 53, 44, 255 },
        { 128, 53, 44, 255 },
        { 129, 54, 44, 255 },
        { 131, 55, 44, 255 },
        { 132, 55, 43, 255 },
        { 133, 56, 43, 255 },
        { 135, 56, 43, 255 },
        { 136, 57, 43, 255 },
        { 137, 57, 43, 255 },
        { 138, 58, 43, 255 },
        { 140, 59, 42, 255 },
        { 141, 59, 42, 255 },
        { 142, 60, 42, 255 },
        { 143, 61, 42, 255 },
        { 145, 61, 41, 255 },
        { 146, 62, 41, 255 },
        { 147, 63, 41, 255 },
        { 148, 63, 41, 255 },
        { 150, 64, 40, 255 },
        { 151, 65, 40, 255 },
        { 152, 65, 40, 255 },
        { 153, 66, 39, 255 },
        { 154, 67, 39, 255 },
        { 156, 67, 39, 255 },
        { 157, 68, 38, 255 },
        { 158, 69, 38, 255 },
        { 159, 70, 38, 255 },
        { 160, 70, 37, 255 },
        { 161, 71, 37, 255 },
        { 162, 72, 37, 255 },
        { 163, 73, 36, 255 },
        { 165, 74, 36, 255 },
        { 166, 75, 36, 255 },
        { 167, 75, 35, 255 },
        { 168, 76, 35, 255 },
        { 169, 77, 35, 255 },
        { 170, 78, 34, 255 },
        { 171, 79, 34, 255 },
        { 172, 80, 33, 255 },
        { 173, 81, 33, 255 },
        { 174, 82, 33, 255 },
        { 175, 82, 32, 255 },
        { 176, 83, 32, 255 },
        { 177, 84, 31, 255 },
        { 178, 85, 31, 255 },
        { 179, 86, 31, 255 },
        { 180, 87, 30, 255 },
        { 181, 88, 30, 255 },
        { 182, 89, 30, 255 },
        { 183, 90, 29, 255 },
        { 184, 91, 29, 255 },
        { 185, 92, 28, 255 },
        { 186, 93, 28, 255 },
        { 187, 94, 28, 255 },
        { 187, 95, 27, 255 },
        { 188, 96, 27, 255 },
        { 189, 97, 27, 255 },
        { 190, 98, 26, 255 },
        { 191, 99, 26, 255 },
        { 192, 100, 26, 255 },
        { 193, 101, 25, 255 },
        { 193, 103, 25, 255 },
        { 194, 104, 25, 255 },
        { 195, 105, 24, 255 },
        { 196, 106, 24, 255 },
        { 197, 107, 24, 255 },
        { 198, 108, 23, 255 },
        { 198, 109, 23, 255 },
        { 199, 110, 23, 255 },
        { 200, 111, 23, 255 },
        { 201, 113, 22, 255 },
        { 202, 114, 22, 255 },
        { 202, 115, 22, 255 },
        { 203, 116, 22, 255 },
        { 204, 117, 22, 255 },
        { 205, 118, 21, 255 },
        { 205, 119, 21, 255 },
        { 206, 121, 21, 255 },
        { 207, 122, 21, 255 },
        { 207, 123, 21, 255 },
        { 208, 124, 21, 255 },
        { 209, 125, 21, 255 },
        { 210, 127, 21, 255 },
        { 210, 128, 21, 255 },
        { 211, 129, 21, 255 },
        { 212, 130, 21, 255 },
        { 212, 131, 21, 255 },
        { 213, 133, 21, 255 },
        { 214, 134, 21, 255 },
        { 214, 135, 21, 255 },
        { 215, 136, 22, 255 },
        { 216, 138, 22, 255 },
        { 216, 139, 22, 255 },
        { 217, 140, 22, 255 },
        { 217, 141, 22, 255 },
        { 218, 143, 23, 255 },
        { 219, 144, 23, 255 },
        { 219, 145, 23, 255 },
        { 220, 146, 24, 255 },
        { 220, 148, 24, 255 },
        { 221, 149, 25, 255 },
        { 222, 150, 25, 255 },
        { 222, 152, 25, 255 },
        { 223, 153, 26, 255 },
        { 223, 154, 26, 255 },
        { 224, 155, 27, 255 },
        { 224, 157, 28, 255 },
        { 225, 158, 28, 255 },
        { 226, 159, 29, 255 },
        { 226, 161, 29, 255 },
        { 227, 162, 30, 255 },
        { 227, 163, 31, 255 },
        { 228, 165, 31, 255 },
        { 228, 166, 32, 255 },
        { 229, 167, 33, 255 },
        { 229, 169, 33, 255 },
        { 230, 170, 34, 255 },
        { 230, 171, 35, 255 },
        { 231, 173, 35, 255 },
        { 231, 174, 36, 255 },
        { 232, 175, 37, 255 },
        { 232, 177, 38, 255 },
        { 233, 178, 39, 255 },
        { 233, 180, 39, 255 },
        { 234, 181, 40, 255 },
        { 234, 182, 41, 255 },
        { 234, 184, 42, 255 },
        { 235, 185, 43, 255 },
        { 235, 186, 44, 255 },
        { 236, 188, 45, 255 },
        { 236, 189, 46, 255 },
        { 237, 191, 46, 255 },
        { 237, 192, 47, 255 },
        { 238, 193, 48, 255 },
        { 238, 195, 49, 255 },
        { 238, 196, 50, 255 },
        { 239, 198, 51, 255 },
        { 239, 199, 52, 255 },
        { 240, 201, 53, 255 },
        { 240, 202, 54, 255 },
        { 240, 203, 55, 255 },
        { 241, 205, 56, 255 },
        { 241, 206, 57, 255 },
        { 241, 208, 58, 255 },
        { 242, 209, 59, 255 },
        { 242, 211, 60, 255 },
        { 243, 212, 61, 255 },
        { 243, 214, 62, 255 },
        { 243, 215, 63, 255 },
        { 244, 217, 64, 255 },
        { 244, 218, 65, 255 },
        { 244, 220, 67, 255 },
        { 245, 221, 68, 255 },
        { 245, 222, 69, 255 }
};


const int ALIVE = int(sizeof(colors) / sizeof(colors[0]));
const int BORN = int(3.0/5.0 * ALIVE);

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
                        for (int k = 0; k < 8; k++)
                                nextgen[i + directions[k][0]][j + directions[k][1]] = BORN;
                    }
                    else
                    {
                        //if (neighbors > 1 && neighbors < 4)
                        if (neighbors > 1 and neighbors < 5)
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
                    if (neighbors == 2)
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