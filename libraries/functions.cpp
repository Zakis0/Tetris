#include "../headers/functions.h"

#include <cstdarg>
#include <valarray>
#include "../headers/constants.h"
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

char **createMatrix(int height, int width) {
    char **result = new char*[height];
    for (int i = 0; i < height; ++i) {
        result[i] = new char[width];
    }
    return result;
}

void error(string str) {
    cerr << str << endl;
    exit(-1);
}

char **connectMatrixHorizontal(char **m1, char **m2, char emptyPixel, int m1Height, int m1Width, int m2Height, int m2Width, int m2TopBias, int m2LeftBias) {
    char **connectedMatrix;
    int connectedMatrixHeight, connectedMatrixWidth;

    if (m1Height < m2Height + m2TopBias) {
        connectedMatrixHeight = m1Height + m2TopBias;
        connectedMatrixWidth = m1Width + m2Width + m2LeftBias;
    }
    else {
        connectedMatrixHeight = m1Height;
        connectedMatrixWidth = m1Width + m2Width + m2LeftBias;
    }

    if (connectedMatrixHeight > MAX_SCREEN_HEIGHT ||
        connectedMatrixWidth > MAX_SCREEN_WIDTH) {
        error("Screen size exceeded");
    }

    connectedMatrix = createMatrix(connectedMatrixHeight, connectedMatrixWidth);

    for (int i = 0; i < m1Height; ++i) {
        for (int j = 0; j < m1Width + m2Width + m2LeftBias; ++j) {
            connectedMatrix[i][j] = emptyPixel;
        }
    }
    for (int i = 0; i < m1Height; ++i) {
        for (int j = 0; j < m1Width; ++j) {
            connectedMatrix[i][j] = m1[i][j];
        }
    }
    for (int i = 0; i < m2Height; ++i) {
        for (int j = 0; j < m2Width; ++j) {
            connectedMatrix[i + m2TopBias][j + m1Width + m2LeftBias] = m2[i][j];
        }
    }
    return connectedMatrix;
}

char **connectMatrixVertical(char **m1, char **m2, char emptyPixel, int m1Height, int m1Width, int m2Height, int m2Width, int m2TopBias, int m2LeftBias) {
    char **connectedMatrix;
    int connectedMatrixHeight, connectedMatrixWidth;

    if (m1Width < m1Width + m2LeftBias) {
        connectedMatrixHeight = m1Height + m2Height + m2TopBias;
        connectedMatrixWidth = m1Width + m2LeftBias;
    }
    else {
        connectedMatrixHeight = m1Height + m2Height + m2TopBias;
        connectedMatrixWidth = m1Width;
    }

    if (connectedMatrixHeight > MAX_SCREEN_HEIGHT ||
        connectedMatrixWidth > MAX_SCREEN_WIDTH) {
        error("Screen size exceeded");
    }
    connectedMatrix = createMatrix(m1Height + m2Height + m2TopBias, m1Width);

    for (int i = 0; i < m1Height + m2Height + m2TopBias; ++i) {
        for (int j = 0; j < m1Width + m2LeftBias; ++j) {
            connectedMatrix[i][j] = emptyPixel;
        }
    }
    for (int i = 0; i < m1Height; ++i) {
        for (int j = 0; j < m1Width; ++j) {
            connectedMatrix[i][j] = m1[i][j];
        }
    }
    for (int i = 0; i < m2Height; ++i) {
        for (int j = 0; j < m2Width; ++j) {
            connectedMatrix[i + m1Height + m2TopBias][j + m2LeftBias] = m2[i][j];
        }
    }
    for (int i = 0; i < m2Height + m1Height + m2TopBias; ++i) {
        for (int j = 0; j < m2Width + m2LeftBias; ++j) {
        }
    }
    return connectedMatrix;
}