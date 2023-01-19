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

const char EMPTY_PIXEL = '-';

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

/////////////////////////////////

const double BUMPINESS_WEIGHT = 0;
const double BLOCKS_IN_RIGHTMOST_LANE_WEIGHT = 0;
const double BLOCKS_ABOVE_HOLES_WEIGHT = 0;
const double CLEARING_NOT_FOUR_LINES_WEIGHT = 0;
const double MINIMISE_HEIGHT_WEIGHT = 1.5;
const double MINIMISE_GLOBAL_HOLES_WEIGHT = 2;

const string I_BLOCK_NAME = "I_Block";
const string J_BLOCK_NAME = "J_Block";
const string L_BLOCK_NAME = "L_Block";
const string O_BLOCK_NAME = "O_Block";
const string S_BLOCK_NAME = "S_Block";
const string T_BLOCK_NAME = "T_Block";
const string Z_BLOCK_NAME = "Z_Block";

const int NUM_OF_ROTATION = 4;
const int NUM_OF_BLOCK_IN_PIECE = 4;

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

void initField() {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field[i][j] = EMPTY_PIXEL;
        }
    }
}

void init() {
    srand(time(nullptr));

    initField();

    score = 0;
    totalClearedLines = 0;
    numOfTetris = 0;
    numOfClearingLines = 0;
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

int getRand(int start, int end) {
    return start + (rand() % end);
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

Piece *getRandPiece() {
    switch (getRand(0, 7)) {
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

double getScore(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    double posScore = 0;

    int maxX = getMaxX(arr);
    int minX = getMinX(arr);
    int maxY = getMaxY(arr);
    int minY = getMinY(arr);

    posScore += BUMPINESS_WEIGHT * bumpinessCheck(minX - 1, maxX + 1);

    if (maxX == FIELD_WIDTH - 1) {
        posScore += BLOCKS_IN_RIGHTMOST_LANE_WEIGHT * numOfBlocksInRightmostLaneCheck(arr);
    }

    posScore += BLOCKS_ABOVE_HOLES_WEIGHT * numOfBlocksAboveHolesCheck(minX, maxX + 1);
    posScore += CLEARING_NOT_FOUR_LINES_WEIGHT * numOfClearingLinesCheck();
    posScore += MINIMISE_HEIGHT_WEIGHT * getMaxHeight(minY);
    posScore += MINIMISE_GLOBAL_HOLES_WEIGHT * numOfGlobalHolesCheck(minX, maxX + 1);

    return posScore;
}

string getBestPosition(const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList) {
    int bestPosIndex = 0;
    double curPosScore, bestPosScore = 10000000;

    for (int i = 0; i < posList.first.size(); ++i) {
        setArraysPixels(posList.second[i], '@');
        curPosScore = getScore(posList.second[i]);
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

array<int, 4> AI(bool printGame = true) {
    Piece *nextPiece = getRandPiece(), *curPiece;
    string actionsToBestPosition;

    init();

    while (!checkLose()) {
        curPiece = nextPiece;
        nextPiece = getRandPiece();
        curPiece->setPixels();
        if (printGame) {
            printField();
        }

        pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
                allActionsAndTheirPosition = getAllActionsAndTheirPosition(curPiece);

        actionsToBestPosition = getBestPosition(allActionsAndTheirPosition);

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

    piece = new O_Block();executeListOfActions(piece, "aaaaqqadaddaddaqeeqqqqeeqeeqeeeddadaw");piece->setPixels();printField();delete piece;
//    piece = new Z_Block();executeListOfActions(piece, "aaaaeew");piece->setPixels();printField();delete piece;
//    piece = new T_Block();executeListOfActions(piece, "aaaw");piece->setPixels();printField();delete piece;
}

void test() {
    init();
//    for (int i = 0; i < FIELD_HEIGHT; ++i) {
//        field[FIELD_HEIGHT - 1 - i][FIELD_WIDTH - 1] = '@';
//    }
//    field[FIELD_HEIGHT - 1][FIELD_WIDTH - 1] = EMPTY_PIXEL;
//
//    for (int i = 0; i < FIELD_HEIGHT; ++i) {
//        field[FIELD_HEIGHT - 1 - i][FIELD_WIDTH - 3] = '@';
//    }
//    field[FIELD_HEIGHT - 1][FIELD_WIDTH - 3] = EMPTY_PIXEL;

    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> arr =
            {make_pair(0, 14), make_pair(1, 14), make_pair(2, 14), make_pair(0, 12)};
    setArraysPixels(arr, '@');

    printField();
    cout << numOfGlobalHolesCheck(0, 3) << endl;
//    cout << getColumnHeight(FIELD_WIDTH - 2) << endl;
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

int main() {
//    player();
    testAI(10);
//    AI();
//    testMove();
//    test();
    return 0;
}