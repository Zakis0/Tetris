#include "../headers/Piece.h"

void Piece::copyPiece(const Piece &piece) {
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

char Piece::getPieceChar() {
    return pieceChar;
}
array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> Piece::getCords() const {
    return {make_pair(x1, y1), make_pair(x2, y2), make_pair(x3, y3), make_pair(x4, y4)};
}

bool Piece::isPixelsInField(int dX, int dY) const {
    if (max_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) >= FIELD_HEIGHT ||
        max_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) >= FIELD_WIDTH ||
        min_(NUM_OF_BLOCK_IN_PIECE, y1 + dY, y2 + dY, y3 + dY, y4 + dY) < 0 ||
        min_(NUM_OF_BLOCK_IN_PIECE, x1 + dX, x2 + dX, x3 + dX, x4 + dX) < 0) {
        return false;
    }
    return true;
}
bool Piece::isPossiblePosition(char field[FIELD_HEIGHT][FIELD_WIDTH], int dX, int dY) const {
    return isPixelsInField(dX, dY) &&
           field[y1 + dY][x1 + dX] == EMPTY_PIXEL &&
           field[y2 + dY][x2 + dX] == EMPTY_PIXEL &&
           field[y3 + dY][x3 + dX] == EMPTY_PIXEL &&
           field[y4 + dY][x4 + dX] == EMPTY_PIXEL;
}

void Piece::cordBias(int dX, int dY) {
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

bool Piece::canMoveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    clearPixels(field);
    if (isPossiblePosition(field, 0, 1)) {
        setPixels(field);
        return true;
    }
    setPixels(field);
    return false;
}

void Piece::moveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    clearPixels(field);
    if (isPossiblePosition(field, 0, 1)) {
        cordBias(0, 1);
    }
    setPixels(field);
}
void Piece::hardDrop(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    while (canMoveDown(field)) {
        moveDown(field);
    }
}
void Piece::moveRight(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    clearPixels(field);
    if (isPossiblePosition(field, 1, 0)) {
        cordBias(1, 0);
    }
    setPixels(field);
}
void Piece::moveLeft(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    clearPixels(field);
    if (isPossiblePosition(field, -1, 0)) {
        cordBias(-1, 0);
    }
    setPixels(field);
}

void Piece::clearPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]) const {
    field[y1][x1] = EMPTY_PIXEL;
    field[y2][x2] = EMPTY_PIXEL;
    field[y3][x3] = EMPTY_PIXEL;
    field[y4][x4] = EMPTY_PIXEL;
}
void Piece::setPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    field[y1][x1] = pieceChar;
    field[y2][x2] = pieceChar;
    field[y3][x3] = pieceChar;
    field[y4][x4] = pieceChar;
}

void Piece::printCord() {
    cout << x1 << ", " << y1 << endl;
    cout << x2 << ", " << y2 << endl;
    cout << x3 << ", " << y3 << endl;
    cout << x4 << ", " << y4 << endl;
}
void Piece::printCenter() {
    cout << xCenter << " " << yCenter << endl;
}

void Piece::changeRotNum(bool clockwise) {
    if (clockwise) {
        bias(rotNum, NUM_OF_ROTATION, 1);
    }
    else {
        bias(rotNum, NUM_OF_ROTATION, -1);
    }
}

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

void Piece::setInitialState() {

}
void Piece::spin(bool clockwise) {
    changeRotNum(clockwise);
}
void Piece::wallKickSpin(char (*field)[10], bool clockwise) {}

I_Block::I_Block() : Piece() {
    pieceName = I_BLOCK_NAME;
    pieceChar = I_BlockChar;
    setInitialState();
}
void I_Block::setInitialState() {
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
void I_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void I_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    I_Test(field, clockwise);
    setPixels(field);
}

J_Block::J_Block() : Piece() {
    pieceName = J_BLOCK_NAME;
    pieceChar = J_BlockChar;
    setInitialState();
}
void J_Block::setInitialState() {
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
void J_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void J_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    JLTSZ_Test(field, clockwise);
    setPixels(field);
}

L_Block::L_Block() : Piece() {
    pieceName = L_BLOCK_NAME;
    pieceChar = L_BlockChar;
    setInitialState();
}
void L_Block::setInitialState() {
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
void L_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void L_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    JLTSZ_Test(field, clockwise);
    setPixels(field);
}

O_Block::O_Block() : Piece() {
    pieceName = O_BLOCK_NAME;
    pieceChar = O_BlockChar;
    setInitialState();
}
void O_Block::setInitialState() {
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
void O_Block::spin(bool clockwise) {}
void O_Block::wallKickSpin(bool clockwise) {}

S_Block::S_Block() : Piece() {
    pieceName = S_BLOCK_NAME;
    pieceChar = S_BlockChar;
    setInitialState();
}
void S_Block::setInitialState() {
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
void S_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void S_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    JLTSZ_Test(field, clockwise);
    setPixels(field);
}

T_Block::T_Block() : Piece() {
    pieceName = T_BLOCK_NAME;
    pieceChar = T_BlockChar;
    setInitialState();
}
void T_Block::setInitialState() {
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
void T_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void T_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    JLTSZ_Test(field, clockwise);
    setPixels(field);
}

Z_Block::Z_Block() : Piece() {
    pieceName = Z_BLOCK_NAME;
    pieceChar = Z_BlockChar;
    setInitialState();
}
void Z_Block::setInitialState() {
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
void Z_Block::spin(bool clockwise) {
    changeRotNum(clockwise);
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
void Z_Block::wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise) {
    clearPixels(field);
    spin(clockwise);
    JLTSZ_Test(field, clockwise);
    setPixels(field);
}