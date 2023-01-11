#include <bits/stdc++.h>
#include <cmath>

using namespace std;

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 16

const int NUM_OF_ROTATION = 4;
const int NUM_OF_BLOCK_IN_PIECE = 4;

const char I_BlockChar = '@';
const char J_BlockChar = '#';
const char L_BlockChar = '$';
const char O_BlockChar = '%';
const char S_BlockChar = '&';
const char T_BlockChar = 'G';
const char Z_BlockChar = 'W';

const char EMPTY_PIXEL_CHAR = '-';

char field[FIELD_HEIGHT][FIELD_WIDTH];

const int FIELD_MIDDLE = FIELD_WIDTH / 2 - 1;

void initField() {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field[i][j] = EMPTY_PIXEL_CHAR;
        }
    }
}

void init() {
    srand(time(nullptr));
    initField();
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

void inc(int &n, int max) {
    n = (n + 1) % max;
}
void dec(int &n, int max) {
    if (n == 0) {
        n = max - 1;
    }
    else {
        --n;
    }
}

class Piece {
protected:
    int x1, y1, x2, y2, x3, y3, x4, y4;
    float xCenter, yCenter;
    char pieceChar;

    int rotNum = 0;

public:
    bool isFieldEmpty(int dX, int dY) const {
        if (max_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) >= FIELD_HEIGHT ||
            max_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) >= FIELD_WIDTH ||
            min_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) < 0 ||
            min_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) < 0) {
            return false;
        }
        return field[y1 + dY][x1 + dX] == EMPTY_PIXEL_CHAR &&
               field[y2 + dY][x2 + dX] == EMPTY_PIXEL_CHAR &&
               field[y3 + dY][x3 + dX] == EMPTY_PIXEL_CHAR &&
               field[y4 + dY][x4 + dX] == EMPTY_PIXEL_CHAR;
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
    }
    void cantMoveDown() {
        setPixels();
    }
    bool moveDown() {
        clearPixels();
        if (max_(NUM_OF_BLOCK_IN_PIECE, y1, y2, y3, y4) + 1 < FIELD_HEIGHT &&
            isFieldEmpty(0, 1)) {
            cordBias(0, 1);
            ++yCenter;
            setPixels();
            return true;
        }
        setPixels();
        return false;
    }
    void moveRight() {
        clearPixels();
        if (max_(NUM_OF_BLOCK_IN_PIECE, x1, x2, x3, x4) + 1 < FIELD_WIDTH &&
            isFieldEmpty(1, 0)) {
            cordBias(1, 0);
            ++xCenter;
        }
        setPixels();
    }
    void moveLeft() {
        clearPixels();
        if (min_(NUM_OF_BLOCK_IN_PIECE, x1, x2, x3, x4) >= 0 &&
            isFieldEmpty(-1, 0)) {
            cordBias(-1, 0);
            --xCenter;
        }
        setPixels();
    }
    void clearPixels() const {
        field[y1][x1] = EMPTY_PIXEL_CHAR;
        field[y2][x2] = EMPTY_PIXEL_CHAR;
        field[y3][x3] = EMPTY_PIXEL_CHAR;
        field[y4][x4] = EMPTY_PIXEL_CHAR;
    }
    void setPixels() const {
        field[y1][x1] = pieceChar;
        field[y2][x2] = pieceChar;
        field[y3][x3] = pieceChar;
        field[y4][x4] = pieceChar;
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
    virtual void spin(bool clockwise) {
        if (clockwise) {
            inc(rotNum, NUM_OF_ROTATION);
        }
        else {
            dec(rotNum, NUM_OF_ROTATION);
        }
    }
    void JLTSZ_Test(bool clockwise);
    void I_Test(bool clockwise);
    virtual void wallKickSpin(bool clockwise) {}
};

void Piece::JLTSZ_Test(bool clockwise) {
    if(isFieldEmpty(0, 0)) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(-1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isFieldEmpty(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isFieldEmpty(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(-1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isFieldEmpty(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isFieldEmpty(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isFieldEmpty(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isFieldEmpty(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isFieldEmpty(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isFieldEmpty(1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(1, -1)) {
                cordBias(1, -1);
                return;
            }
            if (isFieldEmpty(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isFieldEmpty(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(-1, 1)) {
                cordBias(-1, 1);
                return;
            }
            if (isFieldEmpty(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isFieldEmpty(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(-1, -1)) {
                cordBias(-1, -1);
                return;
            }
            if (isFieldEmpty(0, 2)) {
                cordBias(0, 2);
                return;
            }
            if (isFieldEmpty(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(1, 1)) {
                cordBias(1, 1);
                return;
            }
            if (isFieldEmpty(0, -2)) {
                cordBias(0, -2);
                return;
            }
            if (isFieldEmpty(1, -2)) {
                cordBias(1, -2);
                return;
            }
        }
    }
}

void Piece::I_Test(bool clockwise) {
    if(isFieldEmpty(0, 0)) return;
    if (clockwise) {
        if (rotNum == 0) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isFieldEmpty(1, -2)) {
                cordBias(1, -2);
                return;
            }
            if (isFieldEmpty(-2, 1)) {
                cordBias(-2, 1);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isFieldEmpty(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(-2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isFieldEmpty(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isFieldEmpty(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isFieldEmpty(2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isFieldEmpty(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isFieldEmpty(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
    }
    else {
        if (rotNum == 0) {
            if (isFieldEmpty(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(2, 1)) {
                cordBias(2, 1);
                return;
            }
            if (isFieldEmpty(-1, -2)) {
                cordBias(-1, -2);
                return;
            }
        }
        else if (rotNum == 1) {
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isFieldEmpty(1, -2)) {
                cordBias(1, -2);
                return;
            }
            if (isFieldEmpty(-2, 1)) {
                cordBias(-2, 1);
                return;
            }
        }
        else if (rotNum == 2) {
            if (isFieldEmpty(-2, 0)) {
                cordBias(-2, 0);
                return;
            }
            if (isFieldEmpty(1, 0)) {
                cordBias(1, 0);
                return;
            }
            if (isFieldEmpty(-2, -1)) {
                cordBias(-2, -1);
                return;
            }
            if (isFieldEmpty(1, 2)) {
                cordBias(1, 2);
                return;
            }
        }
        else if (rotNum == 3) {
            if (isFieldEmpty(-1, 0)) {
                cordBias(-1, 0);
                return;
            }
            if (isFieldEmpty(2, 0)) {
                cordBias(2, 0);
                return;
            }
            if (isFieldEmpty(-1, 2)) {
                cordBias(-1, 2);
                return;
            }
            if (isFieldEmpty(2, -1)) {
                cordBias(2, -1);
                return;
            }
        }
    }
}

class I_Block: public Piece {
public:
    I_Block() : Piece() {
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
        
        setPixels();
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

        setPixels();
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

        setPixels();
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

        setPixels();
    }
    void wallKickSpin(bool clockwise) {}
};

class S_Block: public Piece {
public:
    S_Block() : Piece() {
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

        setPixels();
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

        setPixels();
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

        setPixels();
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
        field[row][i] = EMPTY_PIXEL_CHAR;
    }
    for (int i = row - 1; i >= 0; --i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field[i + 1][j] = field[i][j];
            field[i][j] = EMPTY_PIXEL_CHAR;
        }
    }
}

int clearLines() {
    bool filled = true;
    int numOfClearLines = 0;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == EMPTY_PIXEL_CHAR) {
                filled = false;
                break;
            }
        }
        if (filled) {
            downgradeMatrix(i);
            ++numOfClearLines;
        }
        filled = true;
    }
    return numOfClearLines;
}

void tetris() {
    init();
    Piece *piece;
    int numOfClearedLines = 0;
    while (true) {
        piece = getRandPiece();
        while (piece->moveDown()) {
            switch (getRand(0, 3)) {
                case 0: {
                    piece->moveLeft();
                    break;
                }
                case 1: {
                    piece->moveRight();
                    break;
                }
            }
            switch (getRand(0, 3)) {
                case 0: {
                    piece->wallKickSpin(true);
                    break;
                }
                case 1: {
                    piece->wallKickSpin(false);
                    break;
                }
            }
            printField();
            usleep(500000);
        }
        piece->~Piece();
        if (numOfClearedLines = clearLines()) {
            cout << numOfClearedLines << endl;
        }
    }
}

int main() {
    tetris();
    return 0;
}