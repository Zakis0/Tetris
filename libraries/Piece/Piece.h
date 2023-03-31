#pragma once
#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include "../../Config/constants.h"
#include "../Functions/functions.h"
#include "../Matrix/matrix.h"

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

    virtual ~Piece();
    char getPieceChar();
    bool isPixelsInField(int dX = 0, int dY = 0) const;
    bool isPossiblePosition(Matrix& field, int dX = 0, int dY = 0) const;
    void cordBias(int dX, int dY);
    bool canMoveDown(Matrix& field);
    void moveDown(Matrix& field);
    void hardDrop(Matrix& field);
    void moveRight(Matrix& field);
    void moveLeft(Matrix& field);
    void clearPixels(Matrix& field) const;
    void setPixels(Matrix& field);
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> getCords() const;

    void JLTSZ_Test(Matrix& field, bool clockwise);
    void I_Test(Matrix& field, bool clockwise);
    void changeRotNum(bool clockwise);

    friend ostream& operator<<(ostream& out, Piece& piece);

    virtual void setInitialState() = 0;
    virtual void spin(bool clockwise) = 0;
    virtual void wallKickSpin(Matrix& field, bool clockwise);
};

struct I_Block: Piece {
    I_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

struct J_Block: Piece {
    J_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

struct L_Block: Piece {
    L_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

struct O_Block: Piece {
    O_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(bool clockwise);
};

struct S_Block: Piece {
    S_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

struct T_Block: Piece {
    T_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

struct Z_Block: Piece {
    Z_Block();
    void setInitialState() override;
    void spin(bool clockwise) override;
    void wallKickSpin(Matrix& field, bool clockwise) final;
};

#endif //TETRIS_PIECE_H
