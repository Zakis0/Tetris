#include <string>
#include <iostream>
#include <cstdarg>
#include <array>
#include <list>
#include <unistd.h>
#include <vector>
#include <set>
#include <valarray>
#include <any>

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

const char EMPTY_PIXEL = '.';

const char CONTROL_MOVE_DOWN = 's';
const char CONTROL_MOVE_LEFT = 'a';
const char CONTROL_MOVE_RIGHT = 'd';
const char CONTROL_HARD_DROP = 'w';
const char CONTROL_SPIN_CLOCKWISE = 'e';
const char CONTROL_SPIN_COUNTERCLOCKWISE = 'q';
const char CONTROL_HELD_PIECE = ' ';

const int SCORE_1_LINE = 100;
const int SCORE_2_LINE = 300;
const int SCORE_3_LINE = 700;
const int SCORE_4_LINE = 1500;

///////////////////////////////////////////

#define LENGTH_OF_ARRAY_WITH_RAND_PIECES 7

const int NUM_OF_SWAP_IN_RAND_SORT = 30;

//////////////////////////////////////////////////

const double BUMPINESS_WEIGHT = 0.1;
const double BLOCKS_ABOVE_HOLES_WEIGHT = 1.9;
const double MINIMISE_HEIGHT_WEIGHT = 1;
const double HOLES_UNDER_BLOCKS_WEIGHT = 1;
const double CLEARING_LINES_WEIGHT = -2.2;
const double CLEARING_NOT_FOUR_LINES_WEIGHT = 0;
const double BLOCKS_IN_RIGHTMOST_LANE_WEIGHT = 0;


#define NUM_OF_SCORE_PARAMS 6

// константы задачи
#define GEN_LENGTH NUM_OF_SCORE_PARAMS // длина подлежащей оптимизации битовой строки
#define GEN_MIN_START_VALUE (-10.0)
#define GEN_MAX_START_VALUE 10.0
#define GEN_MUTATION_STEP 0.5

// константы генетического алгоритма
#define POPULATION_SIZE 1000 // количество индивидуумов в популяции
#define CROSSOVER_PROBABILITY 90 // вероятность скрещивания
#define MUTATION_PROBABILITY 50 // вероятность мутации гена индивидуума
#define MAX_GENERATIONS 500 // максимальное количество поколений


#define USE_INIT_GENS 0

double initGens[GEN_LENGTH] {1.0, 2.0, 3.0, 4.0, 5.0, 4.3};

double scoreParams[NUM_OF_SCORE_PARAMS] = {BUMPINESS_WEIGHT, BLOCKS_ABOVE_HOLES_WEIGHT, MINIMISE_HEIGHT_WEIGHT, HOLES_UNDER_BLOCKS_WEIGHT, CLEARING_NOT_FOUR_LINES_WEIGHT, BLOCKS_IN_RIGHTMOST_LANE_WEIGHT};

//double scoreParams[NUM_OF_SCORE_PARAMS] = {0, -0.3, 0.1, 2.2, -1, 1.0};

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

char field[FIELD_HEIGHT][FIELD_WIDTH];

const int FIELD_MIDDLE = FIELD_WIDTH / 2 - 1;

int score;
int totalClearedLines;
int numOfTetris;
int numOfClearingLines;

int posInArrOfRandPiece;

int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES];

void initField() {
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

void init() {
    initRand();
    initField();

    score = 0;
    totalClearedLines = 0;
    numOfTetris = 0;
    numOfClearingLines = 0;

    posInArrOfRandPiece = 0;

    fillArrayOfRandPieceTypes();
}

void printField() {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            cout << field[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
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
public:
    int x1, y1, x2, y2, x3, y3, x4, y4;
    float xCenter, yCenter;
    char pieceChar;
    int rotNum = 0;

public:
    string pieceName;

    void copyPiece(const Piece &piece) {
        pieceChar = piece.pieceChar;

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
    bool isPossiblePosition(int dX = 0, int dY = 0) const {
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
    bool canMoveDown() {
        clearPixels();
        if (isPossiblePosition(0, 1)) {
            setPixels();
            return true;
        }
        setPixels();
        return false;
    }
    void moveDown() {
        clearPixels();
        if (isPossiblePosition(0, 1)) {
            cordBias(0, 1);
        }
        setPixels();
    }
    void hardDrop() {
        while (canMoveDown()) {
            moveDown();
        }
    }
    void moveRight() {
        clearPixels();
        if (isPossiblePosition(1, 0)) {
            cordBias(1, 0);
        }
        setPixels();
    }
    void moveLeft() {
        clearPixels();
        if (isPossiblePosition(-1, 0)) {
            cordBias(-1, 0);
        }
        setPixels();
    }
    void clearPixels() const {
        field[y1][x1] = EMPTY_PIXEL;
        field[y2][x2] = EMPTY_PIXEL;
        field[y3][x3] = EMPTY_PIXEL;
        field[y4][x4] = EMPTY_PIXEL;
    }
    void setPixels() const {
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
    void JLTSZ_Test(bool clockwise);
    void I_Test(bool clockwise);

    virtual void spin(bool clockwise) {
        if (clockwise) {
            bias(rotNum, NUM_OF_ROTATION, 1);
        }
        else {
            bias(rotNum, NUM_OF_ROTATION, -1);
        }
    }
    virtual void wallKickSpin(bool clockwise) {}
};

void Piece::JLTSZ_Test(bool clockwise) {
    if(isPossiblePosition()) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(-1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isPossiblePosition(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(-1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isPossiblePosition(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isPossiblePosition(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isPossiblePosition(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isPossiblePosition(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(-1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isPossiblePosition(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(-1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isPossiblePosition(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isPossiblePosition(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isPossiblePosition(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isPossiblePosition(1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
}

void Piece::I_Test(bool clockwise) {
    if(isPossiblePosition()) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(1, -2)) {
                cordBias(1, -2);
                return;
            }
            if (isPossiblePosition(-2, 1)) {
                cordBias(-2, 1);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(-2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isPossiblePosition(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isPossiblePosition(2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isPossiblePosition(-1, -1)) {
                cordBias(-1, -1);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isPossiblePosition(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isPossiblePosition(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isPossiblePosition(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(-2, 1)) {
                cordBias(-2, 1);
                return;
            }
            if (isPossiblePosition(1, -1)) {
                cordBias(1, -1);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isPossiblePosition(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isPossiblePosition(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isPossiblePosition(-2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isPossiblePosition(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isPossiblePosition(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isPossiblePosition(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isPossiblePosition(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isPossiblePosition(2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
    }
}

class I_Block: public Piece {
public:
    I_Block() : Piece() {
        pieceName = I_BLOCK_NAME;

        pieceChar = I_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        I_Test(clockwise);
        setPixels();
    }
};

class J_Block: public Piece {
public:
    J_Block() : Piece() {
        pieceName = J_BLOCK_NAME;

        pieceChar = J_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        JLTSZ_Test(clockwise);
        setPixels();
    }
};

class L_Block: public Piece {
public:
    L_Block() : Piece() {
        pieceName = L_BLOCK_NAME;

        pieceChar = L_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        JLTSZ_Test(clockwise);
        setPixels();
    }
};

class O_Block: public Piece {
public:
    O_Block() : Piece() {
        pieceName = O_BLOCK_NAME;

        pieceChar = O_BlockChar;

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
    }
    void spin(bool clockwise) {}
    void wallKickSpin(bool clockwise) {}
};

class S_Block: public Piece {
public:
    S_Block() : Piece() {
        pieceName = S_BLOCK_NAME;

        pieceChar = S_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        JLTSZ_Test(clockwise);
        setPixels();
    }
};

class T_Block: public Piece {
public:
    T_Block() : Piece() {
        pieceName = T_BLOCK_NAME;

        pieceChar = T_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        JLTSZ_Test(clockwise);
        setPixels();
    }
};

class Z_Block: public Piece {
public:
    Z_Block() : Piece() {
        pieceName = Z_BLOCK_NAME;

        pieceChar = Z_BlockChar;

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
    void wallKickSpin(bool clockwise) {
        clearPixels();
        spin(clockwise);
        JLTSZ_Test(clockwise);
        setPixels();
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

void downgradeMatrix(int row) {
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

int clearLines() {
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
            downgradeMatrix(i);
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

void doAction(Piece *piece, int action) {
    switch (action) {
        case MOVE_DOWN: {
            piece->moveDown();
            break;
        }
        case MOVE_LEFT: {
            piece->moveLeft();
            break;
        }
        case MOVE_RIGHT: {
            piece->moveRight();
            break;
        }
        case HARD_DROP: {
            piece->hardDrop();
            break;
        }
        case SPIN_CLOCKWISE: {
            piece->wallKickSpin(true);
            break;
        }
        case SPIN_COUNTERCLOCKWISE: {
            piece->wallKickSpin(false);
            break;
        }
        case HELD_PIECE: {
            break;
        }
    }
}

void checkLines() {
    int numOfClearedLines = clearLines();
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

bool checkLose() {
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

void endGame() {
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

void executeListOfActions(Piece *piece, const string& actionsList, int delay = 0, bool pField = false) {
    for (char action: actionsList) {
        doAction(piece, getAction(action));
        if (pField) {
            printField();
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

void setArraysPixels(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, char pixel) {
    for (pair<int, int> cord : arr) {
        field[cord.second][cord.first] = pixel;
    }
}

pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
        getAllActionsAndTheirPosition(Piece *piece) {
    set<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positionsSet;
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> cordsArr;

    vector<string> actions;
    vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positions;

    Piece *tempPiece;

    list<string> actionsList = getInputList((tempPiece->pieceName != O_BLOCK_NAME));

    for (const string &str: actionsList) {
        tempPiece = createPiece(piece);
        executeListOfActions(tempPiece, str);

        if (!tempPiece->canMoveDown() && tempPiece->isPixelsInField()) {
            cordsArr = tempPiece->getCords();
            sortXY(cordsArr);
            if (positionsSet.insert(cordsArr).second) {
                actions.push_back(str);
                positions.push_back(cordsArr);
            }
        }
        tempPiece->clearPixels();
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

int getColumnHeight(int column) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        if (field[i][column] != EMPTY_PIXEL) {
            return FIELD_HEIGHT - i;
        }
    }
    return 0;
}

int bumpinessCheck(int startColumn = 0, int endColumn = FIELD_WIDTH) {
    // calculate different between columns height
    int sum = 0;

    if (startColumn < 0) {
        startColumn = 0;
    }
    if (endColumn > FIELD_WIDTH - 1) {
        endColumn = FIELD_WIDTH - 1;
    }

    for (int i = startColumn; i < endColumn; ++i) {
        sum += abs(getColumnHeight(i) - getColumnHeight(i + 1));
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

int numOfBlocksAboveHolesCheck(int startColumn = 0, int endColumn = FIELD_WIDTH) {
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

int numOfClearingLinesCheck() {
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

int clearingNotFourLinesCheck() {
    int num = numOfClearingLinesCheck();
    if (num == 0) {
        return 0;
    }
    else if (num != 4) {
        return 1;
    }
}

int getMaxHeight(int minY) {
    return FIELD_HEIGHT - minY;
}

int numOfGlobalHolesCheck(int startColumn = 0, int endColumn = FIELD_WIDTH) {
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

double getScore(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params = scoreParams) {
    double posScore = 0;

    int maxX = getMaxX(arr);
    int minX = getMinX(arr);
    int maxY = getMaxY(arr);
    int minY = getMinY(arr);

    posScore += params[0] * bumpinessCheck(minX - 1, maxX + 1);

    posScore += params[1] * numOfBlocksAboveHolesCheck(minX, maxX + 1);
    posScore += params[2] * getMaxHeight(minY);
    posScore += params[3] * numOfGlobalHolesCheck(minX, maxX + 1);
//    posScore += params[4] * numOfClearingLinesCheck();

    posScore += params[4] * clearingNotFourLinesCheck();
    if (maxX == FIELD_WIDTH - 1) {
        posScore += params[5] * numOfBlocksInRightmostLaneCheck(arr);
    }
    return posScore;
}

string getBestPosition(const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params) {
    int bestPosIndex = 0;
    double curPosScore, bestPosScore = 10000000;

    for (int i = 0; i < posList.first.size(); ++i) {
        setArraysPixels(posList.second[i], '@');
        curPosScore = getScore(posList.second[i], params);
        setArraysPixels(posList.second[i], EMPTY_PIXEL);
        if (curPosScore < bestPosScore) {
            bestPosScore = curPosScore;
            bestPosIndex = i;
        }
    }
    return posList.first[bestPosIndex];
}

void player() {
    init();

    Piece *nextPiece = getRandPiece(), *curPiece;
    char input;

    while (!checkLose()) {
        curPiece = nextPiece;
        nextPiece = getRandPiece();
        curPiece->setPixels();
        printField();

        while (curPiece->canMoveDown()) {
            cin >> input;
            doAction(curPiece, getAction(input));
            printField();
        }
        curPiece->~Piece();
        checkLines();
    }
    endGame();
}

// {score, totalClearedLines, numOfTetris, numOfClearingLines}
array<int, 4> AI(bool printGame = true, const double *params = scoreParams) {
    init();

    Piece *nextPiece = getRandPiece(), *curPiece;
    string actionsToBestPosition;

    while (!checkLose()) {
        curPiece = nextPiece;
        nextPiece = getRandPiece();
        curPiece->setPixels();
        if (printGame) {
            printField();
        }
        pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
                allActionsAndTheirPosition = getAllActionsAndTheirPosition(curPiece);

        actionsToBestPosition = getBestPosition(allActionsAndTheirPosition, params);

        executeListOfActions(curPiece, actionsToBestPosition, AI_MOVES_DELAY, printGame);

        delete curPiece;
        checkLines();
    }
    if (printGame) {
        printField();
        endGame();
    }
    return {score, totalClearedLines, numOfTetris, numOfClearingLines};
}

int testMove() {
    init();
    Piece *piece;
    piece = new I_Block();executeListOfActions(piece, "qw");piece->setPixels();printField();delete piece;
}

void test() {
    init();

//    Piece *piece = getRandPiece();
    cout << getRandDouble(2.5, 3.5, 1);
}

void testAI(int numOfGame) {
    float avgScore = 0, avgClearedLines = 0, avgTetrisRate = 0;
    array<int, 4> result;
    for (int i = 0; i < numOfGame; ++i) {
        result = AI(false);
        avgScore += result[0];
        avgClearedLines += result[1];
        avgTetrisRate += getPercent(result[2], result[3]);
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
            if (USE_INIT_GENS) {
                gens[i] = initGens[i];
            }
            else {
                gens[i] = getRandDouble(GEN_MIN_START_VALUE, GEN_MAX_START_VALUE, 1);
            }
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
    return (int)(result[0] * ((double)result[2] / result[3]));
}

void getPopulationFitness(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].second = fitness(population[i].first);
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
    int maxFitness = 0, posOfBestSpecies;
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
    int bestSpeciesPos;

    fillPopulation(population);

    for (int i = 0; i < MAX_GENERATIONS; ++i) {
        selectionPopulation(population);
        crossoverPopulation(population);
        mutationPopulation(population);

        getPopulationFitness(population);

        bestSpeciesPos = getBestSpeciesPos(population);

        cout << "Generation: " << i + 1 << endl;

        cout << "Best fitness: " << population[bestSpeciesPos].second << endl;
        cout << "Avg fitness: " << getAvgPopulationFitness(population) << endl;

        cout << "Gens: " << population[bestSpeciesPos].first.gens[i];

        for (int i = 1; i < GEN_LENGTH; ++i) {
            cout << ", " << population[bestSpeciesPos].first.gens[i];
        }
        cout << endl << endl;
    }
}

int main() {
//    player();
//    testAI(10);
//    AI();
//    testMove();
//    test();
    trainAI();
    return 0;
}
