#include <string>
#include <iostream>
#include <cstdarg>
#include <array>
#include <list>
#include <unistd.h>
#include <vector>
#include <set>
#include <valarray>
#include <chrono>
#include <thread>

using namespace std;

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 16

const int AI_MOVES_DELAY = 0;

const char I_BlockChar = '@';
const char J_BlockChar = '#';
const char L_BlockChar = '$';
const char O_BlockChar = '%';
const char S_BlockChar = '&';
const char T_BlockChar = 'G';
const char Z_BlockChar = 'W';

#define FIELD_INDENT 4

const char EMPTY_PIXEL = '.';

const char CONTROL_MOVE_DOWN = 's';
const char CONTROL_MOVE_LEFT = 'a';
const char CONTROL_MOVE_RIGHT = 'd';
const char CONTROL_HARD_DROP = 'w';
const char CONTROL_SPIN_CLOCKWISE = 'e';
const char CONTROL_SPIN_COUNTERCLOCKWISE = 'q';
const char CONTROL_HELD_PIECE = 'r';

const int SCORE_1_LINE = 100;
const int SCORE_2_LINE = 300;
const int SCORE_3_LINE = 700;
const int SCORE_4_LINE = 1500;

///////////////////////////////////////////

#define LENGTH_OF_ARRAY_WITH_RAND_PIECES 7

const int NUM_OF_SWAP_IN_RAND_SORT = 30;

//////////////////////////////////////////////////

const double BUMPINESS_WEIGHT = 0;
const double BLOCKS_ABOVE_HOLES_WEIGHT = 0;
const double MINIMISE_HEIGHT_WEIGHT = 10000;
const double HOLES_UNDER_BLOCKS_WEIGHT = 0;
const double CLEARING_NOT_FOUR_LINES_WEIGHT = 0;
const double BLOCKS_IN_RIGHTMOST_LANE_WEIGHT = 0;
const double TETRIS_WEIGHT = 0;
const double NUM_OF_PILLARS_WEIGHT = 0;
//const double CLEARING_LINES_WEIGHT = -2.2;

#define NUM_OF_SCORE_PARAMS 8

// константы задачи
#define GEN_LENGTH NUM_OF_SCORE_PARAMS // длина подлежащей оптимизации битовой строки
#define GEN_MIN_START_VALUE 0
#define GEN_MAX_START_VALUE 10.0
#define GEN_MUTATION_STEP 0.5
#define TETRIS_COOLNESS 5

// константы генетического алгоритма
#define POPULATION_SIZE 100 // количество индивидуумов в популяции
#define CROSSOVER_PROBABILITY 90 // вероятность скрещивания
#define MUTATION_PROBABILITY 15 // вероятность мутации гена индивидуума
#define MAX_GENERATIONS 500 // максимальное количество поколений

#define NUM_OF_THREADS 10

//double scoreParams[NUM_OF_SCORE_PARAMS] = {BUMPINESS_WEIGHT, BLOCKS_ABOVE_HOLES_WEIGHT, MINIMISE_HEIGHT_WEIGHT, HOLES_UNDER_BLOCKS_WEIGHT, CLEARING_NOT_FOUR_LINES_WEIGHT, BLOCKS_IN_RIGHTMOST_LANE_WEIGHT, TETRIS_WEIGHT, NUM_OF_PILLARS_WEIGHT};
double scoreParams[NUM_OF_SCORE_PARAMS] = {2.3, 2.7, 3.9, 5.8, 2.1, 0.9, 9, 1};
// 1, 2, 4, 9, 9.5, 0, 0, 0

const string I_BLOCK_NAME = "I_Block";
const string J_BLOCK_NAME = "J_Block";
const string L_BLOCK_NAME = "L_Block";
const string O_BLOCK_NAME = "O_Block";
const string S_BLOCK_NAME = "S_Block";
const string T_BLOCK_NAME = "T_Block";
const string Z_BLOCK_NAME = "Z_Block";

const int NUM_OF_ROTATION = 4;
const int NUM_OF_BLOCK_IN_PIECE = 4;
const int NUM_OF_PIECES = 7;

const int MOVE_DOWN = 0;
const int MOVE_LEFT = 1;
const int MOVE_RIGHT = 2;
const int HARD_DROP = 3;
const int SPIN_CLOCKWISE = 4;
const int SPIN_COUNTERCLOCKWISE = 5;
const int HELD_PIECE = 6;

const int FIELD_MIDDLE = FIELD_WIDTH / 2 - 1;

int posInArrOfRandPiece;

int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES];

void initField(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field[i][j] = EMPTY_PIXEL;
        }
    }
}

void fillArrayOfRandPieceTypes() {
    for (int i = 0; i < LENGTH_OF_ARRAY_WITH_RAND_PIECES; ++i) {
        arrOfRandPieceTypes[i] = i % NUM_OF_PIECES;
    }
}

void initRand() {
    srand(time(nullptr));
}

void init(char field[FIELD_HEIGHT][FIELD_WIDTH], int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines) {
    initRand();
    initField(field);

    score = 0;
    totalClearedLines = 0;
    numOfTetris = 0;
    numOfClearingLines = 0;

    posInArrOfRandPiece = 0;

    fillArrayOfRandPieceTypes();
}

void printIntArr(int *arr, int arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        cout << arr[i] << ", ";
    }
    cout << endl;
}

int max_(int n, ...) {
    int result = 0, cur;
    va_list factor;
    va_start(factor, n);
    for (int i = 0; i < n; ++i) {
        cur = va_arg(factor, int);
        if (cur > result) {
            result = cur;
        }
    }
    va_end(factor);
    return result;
}

int min_(int n, ...) {
    int result = FIELD_HEIGHT, cur;
    va_list factor;
    va_start(factor, n);
    for (int i = 0; i < n; ++i) {
        cur = va_arg(factor, int);
        if (cur < result) {
            result = cur;
        }
    }
    va_end(factor);
    return result;
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

class Piece {
protected:
    int x1, y1, x2, y2, x3, y3, x4, y4;
    float xCenter, yCenter;
    char pieceChar;
    int rotNum = 0;

public:
    string pieceName;

    void copyPiece(const Piece &piece) {
        x1 = piece.x1;
        y1 = piece.y1;
        x2 = piece.x2;
        y2 = piece.y2;
        x3 = piece.x3;
        y3 = piece.y3;
        x4 = piece.x4;
        y4 = piece.y4;

        xCenter = piece.xCenter;
        yCenter = piece.yCenter;

        rotNum = piece.rotNum;

        pieceChar = piece.pieceChar;

        pieceName = piece.pieceName;
    }
    char getPieceChar() {
        return pieceChar;
    }
    bool isPixelsInField(int dX = 0, int dY = 0) const {
        if (max_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) >= FIELD_HEIGHT ||
            max_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) >= FIELD_WIDTH ||
            min_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) < 0 ||
            min_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) < 0) {
            return false;
        }
        return true;
    }
    bool isPossiblePosition(char field[FIELD_HEIGHT][FIELD_WIDTH], int dX = 0, int dY = 0) const {
        return isPixelsInField(dX, dY) &&
               field[y1 + dY][x1 + dX] == EMPTY_PIXEL &&
               field[y2 + dY][x2 + dX] == EMPTY_PIXEL &&
               field[y3 + dY][x3 + dX] == EMPTY_PIXEL &&
               field[y4 + dY][x4 + dX] == EMPTY_PIXEL;
    }
    void cordBias(int dX, int dY) {
        x1 += dX;
        x2 += dX;
        x3 += dX;
        x4 += dX;
        y1 += dY;
        y2 += dY;
        y3 += dY;
        y4 += dY;

        xCenter += dX;
        yCenter += dY;
    }
    bool canMoveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        clearPixels(field);
        if (isPossiblePosition(field, 0, 1)) {
            setPixels(field);
            return true;
        }
        setPixels(field);
        return false;
    }
    void moveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        clearPixels(field);
        if (isPossiblePosition(field, 0, 1)) {
            cordBias(0, 1);
        }
        setPixels(field);
    }
    void hardDrop(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        while (canMoveDown(field)) {
            moveDown(field);
        }
    }
    void moveRight(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        clearPixels(field);
        if (isPossiblePosition(field, 1, 0)) {
            cordBias(1, 0);
        }
        setPixels(field);
    }
    void moveLeft(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        clearPixels(field);
        if (isPossiblePosition(field, -1, 0)) {
            cordBias(-1, 0);
        }
        setPixels(field);
    }
    void clearPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]) const {
        field[y1][x1] = EMPTY_PIXEL;
        field[y2][x2] = EMPTY_PIXEL;
        field[y3][x3] = EMPTY_PIXEL;
        field[y4][x4] = EMPTY_PIXEL;
    }
    void setPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
        field[y1][x1] = pieceChar;
        field[y2][x2] = pieceChar;
        field[y3][x3] = pieceChar;
        field[y4][x4] = pieceChar;
    }
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> getCords() const {
        return {make_pair(x1, y1), make_pair(x2, y2), make_pair(x3, y3), make_pair(x4, y4)};
    }
    void printCord() {
        cout << x1 << ", " << y1 << endl;
        cout << x2 << ", " << y2 << endl;
        cout << x3 << ", " << y3 << endl;
        cout << x4 << ", " << y4 << endl;
    }
    void printCenter() {
        cout << xCenter << " " << yCenter << endl;
    }
    void JLTSZ_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
    void I_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);

    virtual void setInitialState() {}
    virtual void spin(bool clockwise) {
        if (clockwise) {
            bias(rotNum, NUM_OF_ROTATION, 1);
        }
        else {
            bias(rotNum, NUM_OF_ROTATION, -1);
        }
    }
    virtual void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {}
};

void Piece::JLTSZ_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    if(isPossiblePosition(field)) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, -1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isPossiblePosition(field, 0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(field, -1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isPossiblePosition(field, 0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(field, -1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, 1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isPossiblePosition(field, 0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(field, 1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, 1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isPossiblePosition(field, 0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(field, 1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, 1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isPossiblePosition(field, 0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(field, 1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isPossiblePosition(field, 0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(field, -1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, -1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isPossiblePosition(field, 0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(field, -1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, 1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isPossiblePosition(field, 0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(field, 1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
}

void Piece::I_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    if(isPossiblePosition(field)) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, -2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(field, 1, -2)) {
                cordBias(1, -2);
                return;
            }
            if (isPossiblePosition(field, -2, 1)) {
                cordBias(-2, 1);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(field, -2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, -2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isPossiblePosition(field, 1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, 2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isPossiblePosition(field, 2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(field, 2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, 2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isPossiblePosition(field, -1, -1)) {
                cordBias(-1, -1);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isPossiblePosition(field, 2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, 2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isPossiblePosition(field, -1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(field, -2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, -2, 1)) {
                cordBias(-2, 1);
                return;
            }
            if (isPossiblePosition(field, 1, -1)) {
                cordBias(1, -1);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(field, -2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(field, 1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(field, -2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isPossiblePosition(field, 1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(field, -1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(field, 2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(field, -1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isPossiblePosition(field, 2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
    }
}

class I_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE - 1;
        y1 = 0;
        x2 = FIELD_MIDDLE;
        y2 = 0;
        x3 = FIELD_MIDDLE + 1;
        y3 = 0;
        x4 = FIELD_MIDDLE + 2;
        y4 = 0;

        xCenter = FIELD_MIDDLE + 0.5f;
        yCenter = 0.5f;

        rotNum = 0;
    }

    I_Block() : Piece() {
        pieceName = I_BLOCK_NAME;
        pieceChar = I_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter - 1.5f;
            y1 = yCenter - 0.5f;
            x2 = xCenter - 0.5f;
            y2 = yCenter - 0.5f;
            x3 = xCenter + 0.5f;
            y3 = yCenter - 0.5f;
            x4 = xCenter + 1.5f;
            y4 = yCenter - 0.5f;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 0.5f;
            y1 = yCenter - 1.5f;
            x2 = xCenter + 0.5f;
            y2 = yCenter - 0.5f;
            x3 = xCenter + 0.5f;
            y3 = yCenter + 0.5f;
            x4 = xCenter + 0.5f;
            y4 = yCenter + 1.5f;
        }
        else if (rotNum == 2) {
            x1 = xCenter + 1.5f;
            y1 = yCenter + 0.5f;
            x2 = xCenter + 0.5f;
            y2 = yCenter + 0.5f;
            x3 = xCenter - 0.5f;
            y3 = yCenter + 0.5f;
            x4 = xCenter - 1.5f;
            y4 = yCenter + 0.5f;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 0.5f;
            y1 = yCenter + 1.5f;
            x2 = xCenter - 0.5f;
            y2 = yCenter + 0.5f;
            x3 = xCenter - 0.5f;
            y3 = yCenter - 0.5f;
            x4 = xCenter - 0.5f;
            y4 = yCenter - 1.5f;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        I_Test(field, clockwise);
        setPixels(field);
    }
};

class J_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE - 1;
        y1 = 0;
        x2 = FIELD_MIDDLE - 1;
        y2 = 1;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE + 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE;
        yCenter = 1;

        rotNum = 0;
    }
    J_Block() : Piece() {
        pieceName = J_BLOCK_NAME;
        pieceChar = J_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter - 1;
            y1 = yCenter - 1;
            x2 = xCenter - 1;
            y2 = yCenter + 0;
            x4 = xCenter + 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 1;
            y1 = yCenter - 1;
            x2 = xCenter + 0;
            y2 = yCenter - 1;
            x4 = xCenter + 0;
            y4 = yCenter + 1;
        }
        else if (rotNum == 2) {
            x1 = xCenter + 1;
            y1 = yCenter + 1;
            x2 = xCenter + 1;
            y2 = yCenter + 0;
            x4 = xCenter - 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 1;
            y1 = yCenter + 1;
            x2 = xCenter + 0;
            y2 = yCenter + 1;
            x4 = xCenter + 0;
            y4 = yCenter - 1;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        JLTSZ_Test(field, clockwise);
        setPixels(field);
    }
};

class L_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE + 1;
        y1 = 0;
        x2 = FIELD_MIDDLE - 1;
        y2 = 1;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE + 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE;
        yCenter = 1;

        rotNum = 0;
    }
    L_Block() : Piece() {
        pieceName = L_BLOCK_NAME;
        pieceChar = L_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter + 1;
            y1 = yCenter - 1;
            x2 = xCenter - 1;
            y2 = yCenter + 0;
            x4 = xCenter + 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 1;
            y1 = yCenter + 1;
            x2 = xCenter + 0;
            y2 = yCenter - 1;
            x4 = xCenter + 0;
            y4 = yCenter + 1;
        }
        else if (rotNum == 2) {
            x1 = xCenter - 1;
            y1 = yCenter + 1;
            x2 = xCenter + 1;
            y2 = yCenter + 0;
            x4 = xCenter - 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 1;
            y1 = yCenter - 1;
            x2 = xCenter + 0;
            y2 = yCenter + 1;
            x4 = xCenter + 0;
            y4 = yCenter - 1;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        JLTSZ_Test(field, clockwise);
        setPixels(field);
    }
};

class O_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE;
        y1 = 0;
        x2 = FIELD_MIDDLE + 1;
        y2 = 0;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE + 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE + 0.5f;
        yCenter = 0.5f;

        rotNum = 0;
    }
    O_Block() : Piece() {
        pieceName = O_BLOCK_NAME;
        pieceChar = O_BlockChar;
        setInitialState();
    }
    void spin(bool clockwise) {}
    void wallKickSpin(bool clockwise) {}
};

class S_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE;
        y1 = 0;
        x2 = FIELD_MIDDLE + 1;
        y2 = 0;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE - 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE;
        yCenter = 1;

        rotNum = 0;
    }
    S_Block() : Piece() {
        pieceName = S_BLOCK_NAME;
        pieceChar = S_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter + 1;
            y1 = yCenter - 1;
            x2 = xCenter + 0;
            y2 = yCenter - 1;
            x4 = xCenter - 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 1;
            y1 = yCenter + 1;
            x2 = xCenter + 1;
            y2 = yCenter + 0;
            x4 = xCenter + 0;
            y4 = yCenter - 1;
        }
        else if (rotNum == 2) {
            x1 = xCenter - 1;
            y1 = yCenter + 1;
            x2 = xCenter + 0;
            y2 = yCenter + 1;
            x4 = xCenter + 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 1;
            y1 = yCenter - 1;
            x2 = xCenter - 1;
            y2 = yCenter + 0;
            x4 = xCenter + 0;
            y4 = yCenter + 1;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        JLTSZ_Test(field, clockwise);
        setPixels(field);
    }
};

class T_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE;
        y1 = 0;
        x2 = FIELD_MIDDLE - 1;
        y2 = 1;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE + 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE;
        yCenter = 1;

        rotNum = 0;
    }
    T_Block() : Piece() {
        pieceName = T_BLOCK_NAME;
        pieceChar = T_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter + 0;
            y1 = yCenter - 1;
            x2 = xCenter - 1;
            y2 = yCenter + 0;
            x4 = xCenter + 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 1;
            y1 = yCenter + 0;
            x2 = xCenter + 0;
            y2 = yCenter - 1;
            x4 = xCenter + 0;
            y4 = yCenter + 1;
        }
        else if (rotNum == 2) {
            x1 = xCenter + 0;
            y1 = yCenter + 1;
            x2 = xCenter + 1;
            y2 = yCenter + 0;
            x4 = xCenter - 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 1;
            y1 = yCenter + 0;
            x2 = xCenter + 0;
            y2 = yCenter + 1;
            x4 = xCenter + 0;
            y4 = yCenter - 1;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        JLTSZ_Test(field, clockwise);
        setPixels(field);
    }
};

class Z_Block: public Piece {
public:
    void setInitialState() override {
        x1 = FIELD_MIDDLE - 1;
        y1 = 0;
        x2 = FIELD_MIDDLE;
        y2 = 0;
        x3 = FIELD_MIDDLE;
        y3 = 1;
        x4 = FIELD_MIDDLE + 1;
        y4 = 1;

        xCenter = FIELD_MIDDLE;
        yCenter = 1;

        rotNum = 0;
    }
    Z_Block() : Piece() {
        pieceName = Z_BLOCK_NAME;
        pieceChar = Z_BlockChar;
        setInitialState();
    }

    void spin(bool clockwise) override {
        Piece::spin(clockwise);
        if (rotNum == 0) {
            x1 = xCenter - 1;
            y1 = yCenter - 1;
            x2 = xCenter + 0;
            y2 = yCenter - 1;
            x4 = xCenter + 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 1) {
            x1 = xCenter + 1;
            y1 = yCenter - 1;
            x2 = xCenter + 1;
            y2 = yCenter + 0;
            x4 = xCenter + 0;
            y4 = yCenter + 1;
        }
        else if (rotNum == 2) {
            x1 = xCenter + 1;
            y1 = yCenter + 1;
            x2 = xCenter + 0;
            y2 = yCenter + 1;
            x4 = xCenter - 1;
            y4 = yCenter + 0;
        }
        else if (rotNum == 3) {
            x1 = xCenter - 1;
            y1 = yCenter + 1;
            x2 = xCenter - 1;
            y2 = yCenter + 0;
            x4 = xCenter + 0;
            y4 = yCenter - 1;
        }
    }
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
        clearPixels(field);
        spin(clockwise);
        JLTSZ_Test(field, clockwise);
        setPixels(field);
    }
};

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

Piece *getRandPiece() {
    posInArrOfRandPiece %= LENGTH_OF_ARRAY_WITH_RAND_PIECES;
    if (posInArrOfRandPiece == 0) {
        randSort(arrOfRandPieceTypes, LENGTH_OF_ARRAY_WITH_RAND_PIECES);
    }
    switch (arrOfRandPieceTypes[posInArrOfRandPiece++]) {
        case 0: return new I_Block();
        case 1: return new J_Block();
        case 2: return new L_Block();
        case 3: return new O_Block();
        case 4: return new S_Block();
        case 5: return new T_Block();
        case 6: return new Z_Block();
    }
}

void arrayBiasToLeftTop(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {

    while (min_(NUM_OF_BLOCK_IN_PIECE, arr[0].first, arr[1].first, arr[2].first, arr[3].first) > 0) {
        --arr[0].first;
        --arr[1].first;
        --arr[2].first;
        --arr[3].first;
    }
    while (min_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second) > 0) {
        --arr[0].second;
        --arr[1].second;
        --arr[2].second;
        --arr[3].second;
    }
}

bool arrContains(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> arr, pair<int, int> cords) {
    for (int i = 0; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first == cords.first && arr[i].second == cords.second) {
            return true;
        }
    }
    return false;
}

void printField(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *heldPiece) {
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> pieceCord = heldPiece->getCords();
    arrayBiasToLeftTop(pieceCord);
    for (int j = 0; j < FIELD_WIDTH; ++j) {
        cout << field[0][j] << " ";
    }
    cout << string(FIELD_INDENT, ' ');
    cout << "Held piece:" << endl;
    for (int i = 1; i < 3; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            cout << field[i][j] << " ";
        }
        cout << string(FIELD_INDENT * 1.5, ' ');
        for (int k = 0; k < NUM_OF_BLOCK_IN_PIECE; ++k) {
            if (arrContains(pieceCord, make_pair(k, i - 1))) {
                cout << heldPiece->getPieceChar();
            }
            else {
                cout << " ";
            }
            cout << " ";
        }
        cout << endl;
    }

    for (int i = 3; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            cout << field[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void downgradeMatrix(char field[FIELD_HEIGHT][FIELD_WIDTH], int row) {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        field[row][i] = EMPTY_PIXEL;
    }
    for (int i = row - 1; i >= 0; --i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field[i + 1][j] = field[i][j];
            field[i][j] = EMPTY_PIXEL;
        }
    }
}

int clearLines(char field[FIELD_HEIGHT][FIELD_WIDTH], int& numOfClearingLines) {
    int numOfClearLines = 0;
    bool filled;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        filled = true;
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == EMPTY_PIXEL) {
                filled = false;
                break;
            }
        }
        if (filled) {
            downgradeMatrix(field, i);
            ++numOfClearLines;
        }
    }
    if (numOfClearLines > 0) {
        ++numOfClearingLines;
    }
    return numOfClearLines;
}

int getAction(char c) {
    switch (c) {
        case CONTROL_MOVE_DOWN: {
            return 0;
        }
        case CONTROL_MOVE_LEFT: {
            return 1;
        }
        case CONTROL_MOVE_RIGHT: {
            return 2;
        }
        case CONTROL_HARD_DROP: {
            return 3;
        }
        case CONTROL_SPIN_CLOCKWISE: {
            return 4;
        }
        case CONTROL_SPIN_COUNTERCLOCKWISE: {
            return 5;
        }
        case CONTROL_HELD_PIECE: {
            return 6;
        }
    }
    return '0';
}

void swap(Piece *&a, Piece *&b) {
    Piece *temp = a;
    a = b;
    b = temp;
}

void doAction(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *&piece, int action, Piece *&heldPiece) {
    switch (action) {
        case MOVE_DOWN: {
            piece->moveDown(field);
            break;
        }
        case MOVE_LEFT: {
            piece->moveLeft(field);
            break;
        }
        case MOVE_RIGHT: {
            piece->moveRight(field);
            break;
        }
        case HARD_DROP: {
            piece->hardDrop(field);
            break;
        }
        case SPIN_CLOCKWISE: {
            piece->wallKickSpin(field, true);
            break;
        }
        case SPIN_COUNTERCLOCKWISE: {
            piece->wallKickSpin(field, false);
            break;
        }
        case HELD_PIECE: {
            swap(piece, heldPiece);
            heldPiece->clearPixels(field);
            heldPiece->setInitialState();
            piece->setPixels(field);
            break;
        }
    }
}

void checkLines(char field[FIELD_HEIGHT][FIELD_WIDTH], int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines) {
    int numOfClearedLines = clearLines(field, numOfClearingLines);
    totalClearedLines += numOfClearedLines;
    switch (numOfClearedLines) {
        case 1: {
            score += SCORE_1_LINE;
            break;
        }
        case 2: {
            score += SCORE_2_LINE;
            break;
        }
        case 3: {
            score += SCORE_3_LINE;
            break;
        }
        case 4: {
            score += SCORE_4_LINE;
            ++numOfTetris;
            break;
        }
    }
}

bool checkLose(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        if (field[0][i] != EMPTY_PIXEL) return true;
    }
    return false;
}

float getPercent(int dividend, int divisor, int precision = 2) {
    if (divisor == 0) {
        return 0;

    }
    return floor((float)dividend / divisor * 100 * pow(10, precision)) / pow(10, precision);
}

void endGame(int score, int totalClearedLines, int numOfTetris, int numOfClearingLines) {
    cout << "Score: " << score << endl;
    cout << "Num of cleared lines: " << totalClearedLines << endl;
    cout << "Tetris rate: " << getPercent(numOfTetris, numOfClearingLines) << "%" << endl;
}

list<string> getInputList(bool notO_Block) {
    list<string> result;
    for (int numOfHorMov = FIELD_WIDTH / 2 - 1; numOfHorMov >= -(FIELD_WIDTH / 2 - 1); --numOfHorMov) {
        for (int numOfRot = NUM_OF_ROTATION - 1; numOfRot >= -(NUM_OF_ROTATION - 1); --numOfRot) {
            string str = "";
            if (notO_Block) {
                if (numOfRot > 0) str += string(numOfRot, CONTROL_SPIN_CLOCKWISE);
                else str += string(abs(numOfRot), CONTROL_SPIN_COUNTERCLOCKWISE);
            }
            if (numOfHorMov > 0) str += string(numOfHorMov, CONTROL_MOVE_LEFT);
            else str += string(abs(numOfHorMov), CONTROL_MOVE_RIGHT);
            str += string(1, CONTROL_HARD_DROP);
            result.push_back(str);
        }
    }
    return result;
}

void executeListOfActions(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *&piece, const string& actionsList, Piece *&heldPiece, int delay = 0, bool pField = false) {
    for (char action: actionsList) {
        doAction(field, piece, getAction(action), heldPiece);
        if (pField) {
            printField(field, heldPiece);
        }
        usleep(delay);
    }
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

Piece *createPiece(Piece *piece) {
    if (smpStr(piece->pieceName, I_BLOCK_NAME)) {
        return new I_Block();
    }
    else if (smpStr(piece->pieceName, J_BLOCK_NAME)) {
        return new J_Block();
    }
    else if (smpStr(piece->pieceName, L_BLOCK_NAME)) {
        return new L_Block();
    }
    else if (smpStr(piece->pieceName, O_BLOCK_NAME)) {
        return new O_Block();
    }
    else if (smpStr(piece->pieceName, S_BLOCK_NAME)) {
        return new S_Block();
    }
    else if (smpStr(piece->pieceName, T_BLOCK_NAME)) {
        return new T_Block();
    }
    else if (smpStr(piece->pieceName, Z_BLOCK_NAME)) {
        return new Z_Block();
    }
}

int minPosX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos) {
    int mPos = startPos;
    for (int i = startPos; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first < arr[mPos].first) {
            mPos = i;
        }
    }
    return mPos;
}

int minPosY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos, int endPos) {
    int mPos = startPos;
    for (int i = startPos; i < endPos; ++i) {
        if (arr[i].second < arr[mPos].second) {
            mPos = i;
        }
    }
    return mPos;
}

void sortXY(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr) {
    int mPos;
    pair<int, int> temp;

    for (int i = 0; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        mPos = minPosX(arr, i);
        temp = arr[mPos];
        arr[mPos] = arr[i];
        arr[i] = temp;
    }

    int curX = arr[0].first, curXFirstPos = 0;
    for (int i = 1; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first != curX) {
            for (int j = curXFirstPos; j < i; ++j) {
                mPos = minPosY(arr, j, i);
                temp = arr[mPos];
                arr[mPos] = arr[j];
                arr[j] = temp;
            }
            curXFirstPos = i;
            curX = arr[i].first;
        }
    }
    for (int i = curXFirstPos; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        mPos = minPosY(arr, i, NUM_OF_BLOCK_IN_PIECE);
        temp = arr[mPos];
        arr[mPos] = arr[i];
        arr[i] = temp;
    }
}

void setArraysPixels(char field[FIELD_HEIGHT][FIELD_WIDTH], const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, char pixel) {
    for (pair<int, int> cord : arr) {
        field[cord.second][cord.first] = pixel;
    }
}

pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
        getAllActionsAndTheirPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *piece, Piece *heldPiece) {
    set<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positionsSet;
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> cordsArr;

    vector<string> actions;
    vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positions;

    Piece *tempPiece = piece;

    list<string> actionsList = getInputList((tempPiece->pieceName != O_BLOCK_NAME));

    for (const string &str: actionsList) {
        tempPiece = createPiece(piece);
        executeListOfActions(field, tempPiece, str, heldPiece);

        if (!tempPiece->canMoveDown(field) && tempPiece->isPixelsInField()) {
            cordsArr = tempPiece->getCords();
            sortXY(cordsArr);
            if (positionsSet.insert(cordsArr).second) {
                actions.push_back(str);
                positions.push_back(cordsArr);
            }
        }
        tempPiece->clearPixels(field);
        delete tempPiece;
    }
    return make_pair(actions, positions);
}

int getMinX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return arr[0].first;
}

int getMaxX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return arr[NUM_OF_BLOCK_IN_PIECE - 1].first;
}

int getMinY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return min_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second);
}

int getMaxY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return max_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second);
}

int getColumnHeight(char field[FIELD_HEIGHT][FIELD_WIDTH], int column) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        if (field[i][column] != EMPTY_PIXEL) {
            return FIELD_HEIGHT - i;
        }
    }
    return 0;
}

int bumpinessCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH) {
    // calculate different between columns height
    int sum = 0;

    if (startColumn < 0) {
        startColumn = 0;
    }
    if (endColumn > FIELD_WIDTH - 1) {
        endColumn = FIELD_WIDTH - 1;
    }

    for (int i = startColumn; i < endColumn; ++i) {
        sum += abs(getColumnHeight(field, i) - getColumnHeight(field, i + 1));
    }
    return sum;
}

int numOfBlocksInRightmostLaneCheck(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    int num = 0;
    for (pair<int, int> cords : arr) {
        if (cords.first == FIELD_WIDTH - 1) {
            ++num;
        }
    }
    return num;
}

int numOfBlocksAboveHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = FIELD_HEIGHT - 1; i >= 0; --i) {
            if (!flag && field[i][j] != EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field[i][j] != EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfClearingLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int num = 0;
    bool filled;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        filled = true;
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == EMPTY_PIXEL) {
                filled = false;
                break;
            }
        }
        if (filled) {
            ++num;
        }
    }
    return num;
}

int tetrisCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    if (numOfClearingLinesCheck(field) == 4) {
        return 1;
    }
    return 0;
}

int clearingNotFourLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int num = numOfClearingLinesCheck(field);
    if (num == 4) {
        return 1;
    }
    else {
        return -1;
    }
}

int getMaxHeight(int minY) {
    return FIELD_HEIGHT - minY;
}

int numOfGlobalHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = 0; i < FIELD_HEIGHT; ++i) {
            if (!flag && field[i][j] == EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field[i][j] == EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfPillarsCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH) {
    int numOfPillars = 0;
    for (int j = startColumn; j < endColumn; ++j) {
        if (j == 0) {
            if (getColumnHeight(field, j + 1) - getColumnHeight(field, j) >= 3) {
                ++numOfPillars;
            }
        }
        else if (j == FIELD_WIDTH - 1) {
            if (getColumnHeight(field, j - 1) - getColumnHeight(field, j) >= 3) {
                ++numOfPillars;
            }
        }
        else {
            if ((getColumnHeight(field, j - 1) - getColumnHeight(field, j) >= 3) &&
                (getColumnHeight(field, j + 1) - getColumnHeight(field, j) >= 3)) {
                ++numOfPillars;
            }
        }
    }
    return numOfPillars;
}

double getScore(char field[FIELD_HEIGHT][FIELD_WIDTH], const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params = scoreParams) {
    double posScore = 0;

    int maxX = getMaxX(arr);
    int minX = getMinX(arr);
    int maxY = getMaxY(arr);
    int minY = getMinY(arr);

    posScore += params[0] * bumpinessCheck(field, minX - 1, maxX + 1);

    posScore += params[1] * numOfBlocksAboveHolesCheck(field, minX, maxX + 1);
    posScore += params[2] * getMaxHeight(minY);
    posScore += params[3] * numOfGlobalHolesCheck(field, minX, maxX + 1);
    posScore += params[4] * clearingNotFourLinesCheck(field);
    if (maxX == FIELD_WIDTH - 1) {
        posScore += params[5] * numOfBlocksInRightmostLaneCheck(arr);
    }
    posScore += -params[6] * tetrisCheck(field);
    posScore += params[7] * numOfPillarsCheck(field, minX, maxX + 1);
    return posScore;
}

pair<string, double> getBestPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params) {
    int bestPosIndex = 0;
    double curPosScore, bestPosScore = 10000000;

    for (int i = 0; i < posList.first.size(); ++i) {
        setArraysPixels(field, posList.second[i], '@');
        curPosScore = getScore(field, posList.second[i], params);
        setArraysPixels(field, posList.second[i], EMPTY_PIXEL);
        if (curPosScore < bestPosScore) {
            bestPosScore = curPosScore;
            bestPosIndex = i;
        }
    }
    return make_pair(posList.first[bestPosIndex], bestPosScore);
}

void player() {
    char field[FIELD_HEIGHT][FIELD_WIDTH];
    int score, totalClearedLines, numOfTetris, numOfClearingLines;
    init(field, score, totalClearedLines, numOfTetris, numOfClearingLines);

    Piece *nextPiece = getRandPiece(), *curPiece, *heldPiece = new I_Block();
    char input;

    while (!checkLose(field)) {
        curPiece = nextPiece;
        nextPiece = getRandPiece();
        curPiece->setPixels(field);
        printField(field, heldPiece);

        while (curPiece->canMoveDown(field)) {
            cin >> input;
            doAction(field, curPiece, getAction(input), heldPiece);
            printField(field, heldPiece);
        }
        curPiece->~Piece();
        checkLines(field, score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    endGame(score, totalClearedLines, numOfTetris, numOfClearingLines);
}

// {score, totalClearedLines, numOfTetris, numOfClearingLines}
array<int, 4> AI(bool printGame = true, const double *params = scoreParams) {
    char field[FIELD_HEIGHT][FIELD_WIDTH];
    int score, totalClearedLines, numOfTetris, numOfClearingLines;
    init(field, score, totalClearedLines, numOfTetris, numOfClearingLines);

    Piece *nextPiece = getRandPiece(), *curPiece, *heldPiece = new I_Block();

    string actionsToBestPosition;

    while (!checkLose(field)) {
        curPiece = nextPiece;
        nextPiece = getRandPiece();
        curPiece->setPixels(field);
        if (printGame) {
            printField(field, heldPiece);
        }
        auto allActionsAndTheirPosition = getAllActionsAndTheirPosition(field, curPiece, heldPiece);
        auto allActionsAndTheirPositionHeldPiece = getAllActionsAndTheirPosition(field, heldPiece, heldPiece);

        pair<string, double> bestPosition = getBestPosition(field, allActionsAndTheirPosition, params);
        pair<string, double> bestPositionHeldPiece = getBestPosition(field, allActionsAndTheirPositionHeldPiece, params);

        if (bestPosition.second <= bestPositionHeldPiece.second) {
            actionsToBestPosition = bestPosition.first;
        }
        else {
            actionsToBestPosition = CONTROL_HELD_PIECE + bestPositionHeldPiece.first;
        }

        executeListOfActions(field, curPiece, actionsToBestPosition, heldPiece, AI_MOVES_DELAY, printGame);

        delete curPiece;
        checkLines(field, score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    if (printGame) {
        printField(field, heldPiece);
        endGame(score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    return {score, totalClearedLines, numOfTetris, numOfClearingLines};
}

void testAI(int numOfGame) {
    float avgScore = 0, avgClearedLines = 0, avgTetrisRate = 0;
    array<int, 4> result;
    vector <thread> thVec;
    auto totalStartTime = chrono::high_resolution_clock::now();
    auto totalEndTime = chrono::high_resolution_clock::now();
    cout << "Score; Cleared lines; Tetris rate; Duration" << endl;

    for (int i = 0; i < numOfGame; ++i) {
        thVec.emplace_back(
                [&result, &avgScore, &avgClearedLines, &avgTetrisRate, &totalStartTime, &totalEndTime](){
                    result = AI(false);
                    avgScore += result[0];
                    avgClearedLines += result[1];
                    avgTetrisRate += getPercent(result[2], result[3]);

                    totalEndTime = chrono::high_resolution_clock::now();
                    chrono::duration<float> totalDuration = totalEndTime - totalStartTime;

                    cout << result[0] << " " << result[1] << " " <<
                    getPercent(result[2], result[3]) << "% " << totalDuration.count() << endl;
                });
    }
    for (int i = 0; i < numOfGame; ++i) {
        thVec.at(i).join();
    }
    avgScore /= numOfGame;
    avgClearedLines /= numOfGame;
    avgTetrisRate = floor(avgTetrisRate / numOfGame * 100) / 100;

    cout << "avgScore: " << avgScore << endl;
    cout << "avgClearedLines: " << avgClearedLines << endl;
    cout << "avgTetrisRate: " << avgTetrisRate << "%" << endl;
}

bool getTrueWithProbability(int probability) {
    return getRandInt(0, 100) < probability;
}

class Species {
public:
    double *gens;

    Species() {
        gens = new double[GEN_LENGTH];
        for (int i = 0; i < GEN_LENGTH; ++i) {
            gens[i] = getRandDouble(GEN_MIN_START_VALUE, GEN_MAX_START_VALUE, 1);
        }
    }
    Species(const double gen[GEN_LENGTH]) {
        gens = new double[GEN_LENGTH];
        for (int i = 0; i < GEN_LENGTH; ++i) {
            gens[i] = gen[i];
        }
    }
    void printGens() const {
        for (int i = 0; i < GEN_LENGTH; ++i) {
            cout << gens[i] << ", ";
        }
        cout << endl;
    }
};

Species* getClone(const Species& species) {
    return new Species(species.gens);
}

int fitness(Species species) {
    array<int, 4> result = AI(false, species.gens);
    if (result[3] != 0) {
        return (int)(result[0] * (1 + TETRIS_COOLNESS * (double)result[2] / result[3]));
    }
    return result[0];
}

void getFitnessFromTo(pair<Species, int> population[POPULATION_SIZE], int start = 0, int end = POPULATION_SIZE) {
    for (int i = start; i < end; ++i) {
        population[i].second = fitness(population[i].first);
    }
}

void getPopulationFitness(pair<Species, int> population[POPULATION_SIZE]) {
    vector <thread> thVec;
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.emplace_back(getFitnessFromTo, ref(population), i * POPULATION_SIZE / NUM_OF_THREADS, (i + 1) * (POPULATION_SIZE / NUM_OF_THREADS));
    }
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.at(i).join();
    }
}

void fillPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].first = Species();
    }
    getPopulationFitness(population);
}

bool equals(int a, int b, int c) {
    return a == b && a == c;
}

int getBestSpecies(const pair<Species, int> population[POPULATION_SIZE], int s1, int s2, int s3) {
    if (population[s1].second >= population[s2].second && population[s1].second >= population[s3].second) {
        return s1;
    }
    if (population[s2].second >= population[s1].second && population[s2].second >= population[s3].second) {
        return s2;
    }
    return s3;
}

void selectionPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    Species selected[POPULATION_SIZE];
    Species *clone;
    int s1, s2, s3;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        do {
            s1 = getRandInt(0, POPULATION_SIZE);
            s2 = getRandInt(0, POPULATION_SIZE);
            s3 = getRandInt(0, POPULATION_SIZE);
        } while (equals(s1, s2, s2));
        clone = getClone(population[getBestSpecies(population, s1, s2, s3)].first);
        selected[i] = *clone;
    }
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].first = selected[i];
    }
}

void crossover(Species &s1, Species &s2) {
    if (!getTrueWithProbability(CROSSOVER_PROBABILITY)) return;
    int genCatPos = getRandInt(0, GEN_LENGTH);
    int temp;
    for (int i = genCatPos; i < GEN_LENGTH; ++i) {
        temp = s1.gens[i];
        s1.gens[i] = s2.gens[i];
        s2.gens[i] = temp;
    }
}

void crossoverPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
        crossover(population[2 * i].first, population[2 * i + 1].first);
    }
}

void genMutation(pair<Species, int> population[POPULATION_SIZE], int arrPos, int genPos) {
    if (getRandInt(0, 2)) {
        population[arrPos].first.gens[genPos] += GEN_MUTATION_STEP;
    }
    else {
        population[arrPos].first.gens[genPos] -= GEN_MUTATION_STEP;
    }
}

void mutationPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < GEN_LENGTH; ++j) {
            if (getTrueWithProbability(MUTATION_PROBABILITY)) {
                genMutation(population, i, j);
            }
        }
    }
}

float getAvgPopulationFitness(const pair<Species, int> population[POPULATION_SIZE]) {
    int avgFitness = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        avgFitness += population[i].second;
    }
    return (float)avgFitness / POPULATION_SIZE;
}

int getBestSpeciesPos(pair<Species, int> population[POPULATION_SIZE]) {
    int maxFitness = 0, posOfBestSpecies = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        if (population[i].second > maxFitness) {
            maxFitness = population[i].second;
            posOfBestSpecies = i;
        }
    }
    return posOfBestSpecies;
}

void swapInArr(pair<Species, int> arr[POPULATION_SIZE], int pos1, int pos2) {
    pair<Species, int> temp = arr[pos1];
    arr[pos1] = arr[pos2];
    arr[pos2] = temp;
}

void sortArrByFitness(pair<Species, int> population[POPULATION_SIZE]) {
    int posOfMax;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        posOfMax = i;
        for (int j = i; j < POPULATION_SIZE; ++j) {
            if (population[j].second > population[posOfMax].second) {
                posOfMax = j;
            }
        }
        swapInArr(population, i, posOfMax);
    }
}

void trainAI() {
    initRand();
    pair<Species, int> population[POPULATION_SIZE];
    int bestSpeciesPos = 0;

    fillPopulation(population);

    auto totalStartTime = chrono::high_resolution_clock::now();
    auto generationStartTime = chrono::high_resolution_clock::now();
    auto generationEndTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX_GENERATIONS; ++i) {
        generationStartTime = chrono::high_resolution_clock::now();
        selectionPopulation(population);
        crossoverPopulation(population);
        mutationPopulation(population);

        getPopulationFitness(population);

        bestSpeciesPos = getBestSpeciesPos(population);

        cout << "Generation: " << i + 1 << endl;

        cout << "Best fitness: " << population[bestSpeciesPos].second << endl;
        cout << "Avg fitness: " << getAvgPopulationFitness(population) << endl;

        cout << "Gens of best species: " << population[bestSpeciesPos].first.gens[0];

        for (int j = 1; j < GEN_LENGTH; ++j) {
            cout << ", " << population[bestSpeciesPos].first.gens[j];
        }

        generationEndTime = chrono::high_resolution_clock::now();
        chrono::duration<float> generationDuration = generationEndTime - generationStartTime;
        chrono::duration<float> totalDuration = generationEndTime - totalStartTime;

        cout << endl << "Generation time: " << generationDuration.count() << endl;
        cout << "Total time: " << totalDuration.count() << endl << endl;
    }
}

int main() {
//    player();
//    AI();
//    testAI(10);
    trainAI();
    return 0;
}
