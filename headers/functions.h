#pragma once
#ifndef TETRIS_FUNCTIONS_H
#define TETRIS_FUNCTIONS_H

#include <string>

using namespace std;

void initRand();
int max_(int n, ...);
int min_(int n, ...);
bool equals(int n, ...);
int getRandInt(int start, int end);
double getRandDouble(float start, float end, int numOfDecimalPlaces);
void bias(int &n, int max, int bias);
void swapInArr(int *arr, int pos1, int pos2);
void randSort(int *arr, int arrSize);
float getPercent(int dividend, int divisor, int precision = 2);
bool smpStr(const string& a, const string& b);
void printStr(const string& str);
bool getTrueWithProbability(int probability);
char **createMatrix(int height, int width);
char **connectMatrixHorizontal(char **m1, char **m2, char emptyPixel, int m1Height, int m1Width, int m2Height, int m2Width, int m2TopBias = 0, int m2LeftBias = 0);
char **connectMatrixVertical(char **m1, char **m2, char emptyPixel, int m1Height, int m1Width, int m2Height, int m2Width, int m2TopBias = 0, int m2LeftBias = 0);
void error(string str);

#endif //TETRIS_FUNCTIONS_H
