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
void error(string str);
void roundToZero(double &n);
double round(double number, int digits);

#endif //TETRIS_FUNCTIONS_H
