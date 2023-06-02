#pragma once
#ifndef TETRIS_TETRIS_FUNCTIONS_H
#define TETRIS_TETRIS_FUNCTIONS_H

#include "../../Config/constants.h"
#include "../Functions/functions.h"
#include "../Piece/Piece.h"
#include "../MyQueue/myQueue.h"
#include "../Matrix/matrix.h"

#include <array>
#include <list>
#include <queue>
#include <unistd.h>

void fillArrayOfRandPieceTypes(int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES]);
void initField(Matrix& field);
void init(Matrix& field, int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines, int& posInArrOfRandPiece, int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES]);
void arrayBiasToLeftTop(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
void printField(Matrix& field, Piece *heldPiece, array<int, 4> statArr, Queue &queueOfRandPieceTypes);
void swapPiece(Piece *&a, Piece *&b);
list<string> getInputList(bool notO_Block);
int minPosX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos);
int minPosY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos, int endPos);
void sortXY(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr);
void setArraysPixels(Matrix& field, const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, char pixel);
void downgradeMatrix(Matrix& field, int row);
int clearLines(Matrix& field, int& numOfClearingLines);
void checkLines(Matrix& field, int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines);
bool checkLose(Matrix& field);
int getMinX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMaxX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMinY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMaxY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
Piece *createPiece(Piece *piece);
Piece *createPieceByCode(int code);
Piece *getRandPiece(Queue &queueOfRandPieceTypes, int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES], int& posInArrOfRandPiece);
void endGame(int score, int totalClearedLines, int numOfTetris, int numOfClearingLines);
int getAction(char c);
void doAction(Matrix& field, Piece *&piece, int action, Piece *&heldPiece);
void player();

#endif //TETRIS_TETRIS_FUNCTIONS_H
