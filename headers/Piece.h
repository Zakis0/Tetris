#pragma once
#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include "constants.h"
#include "functions.h"

#include <array>
#include <iostream>

using namespace std;

class Piece {
protected:
    int x1, y1, x2, y2, x3, y3, x4, y4;
    float xCenter, yCenter;
    char pieceChar;
    int rotNum = 0;

public:
    string pieceName;

    void copyPiece(const Piece &piece);
    char getPieceChar();
    bool isPixelsInField(int dX = 0, int dY = 0) const;
    bool isPossiblePosition(char field[FIELD_HEIGHT][FIELD_WIDTH], int dX = 0, int dY = 0) const;
    void cordBias(int dX, int dY);
    bool canMoveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    void moveDown(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    void hardDrop(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    void moveRight(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    void moveLeft(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    void clearPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]) const;
    void setPixels(char field[FIELD_HEIGHT][FIELD_WIDTH]);
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> getCords() const;
    void printCord();
    void printCenter();
    void JLTSZ_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
    void I_Test(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
    void changeRotNum(bool clockwise);

    virtual void setInitialState();
    virtual void spin(bool clockwise);
    virtual void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class I_Block: public Piece {
public:
    I_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class J_Block: public Piece {
public:
    J_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class L_Block: public Piece {
public:
    L_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class O_Block: public Piece {
public:
    O_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(bool clockwise);
};

class S_Block: public Piece {
public:
    S_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class T_Block: public Piece {
public:
    T_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

class Z_Block: public Piece {
public:
    Z_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(char field[FIELD_HEIGHT][FIELD_WIDTH], bool clockwise);
};

#endif //TETRIS_PIECE_H
