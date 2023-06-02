#include "functions.h"

#include <cstdarg>
#include <valarray>
#include "../../Config/constants.h"
#include <iostream>

void initRand() {
    srand(time(nullptr));
}

int max_(int n, ...) {
    if (n < 2) {
        cerr << "Less then 2 params";
        return false;
    }
    int result, cur;
    va_list params;
    va_start(params, n);

    result = va_arg(params, int);
    for (int i = 1; i < n; ++i) {
        cur = va_arg(params, int);
        if (cur > result) {
            result = cur;
        }
    }
    va_end(params);
    return result;
}

int min_(int n, ...) {
    if (n < 2) {
        cerr << "Less then 2 params";
        return false;
    }
    int result, cur;
    va_list params;
    va_start(params, n);

    result = va_arg(params, int);
    for (int i = 1; i < n; ++i) {
        cur = va_arg(params, int);
        if (cur < result) {
            result = cur;
        }
    }
    va_end(params);
    return result;
}

bool equals(int n, ...) {
    if (n < 2) {
        cerr << "Less then 2 params";
        return false;
    }
    int cur, prev;
    va_list params;
    va_start(params, n);

    prev = va_arg(params, int);
    for (int i = 1; i < n; ++i) {
        cur = va_arg(params, int);
        if (cur != prev) {
            va_end(params);
            return false;
        }
        prev = cur;
    }
    va_end(params);
    return true;
}

int getRandInt(int start, int end) {
    return start + (rand() % (end - start));
}

double getRandDouble(float start, float end, int numOfDecimalPlaces) {
    return (double)getRandInt((int)(start * pow(10, numOfDecimalPlaces)), (int)(end * pow(10, numOfDecimalPlaces))) / pow(10, numOfDecimalPlaces);
}

void bias(int &n, int max, int bias) {
    n = (n + max + bias) % max;
}

void swapInArr(int *arr, int pos1, int pos2) {
    int temp = arr[pos1];
    arr[pos1] = arr[pos2];
    arr[pos2] = temp;
}

void randSort(int *arr, int arrSize) {
    int pos1;
    int pos2;
    for (int i = 0; i < NUM_OF_SWAP_IN_RAND_SORT; ++i) {
        pos1 = getRandInt(0, arrSize);
        pos2 = getRandInt(0, arrSize);
        swapInArr(arr, pos1, pos2);
    }
}

float getPercent(int dividend, int divisor, int precision) {
    if (divisor == 0) {
        return 0;

    }
    return floor((float)dividend / divisor * 100 * pow(10, precision)) / pow(10, precision);
}

bool smpStr(const string& a, const string& b) {
    if (a.length() != b.length()) return false;
    for (int i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void printStr(const string& str) {
    for (char c : str) {
        cout << c;
    }
    cout << endl;
}

bool getTrueWithProbability(int probability) {
    return getRandInt(0, 100) < probability;
}

void roundToZero(double &n) {
    if (abs(n) < 0.001) {
        n = 0;
    }
}

double round(double number, int digits) {
    double result = number;
    result *= pow(10, digits);
    result = round(result);
    result /= pow(10, digits);
    return result;
}
