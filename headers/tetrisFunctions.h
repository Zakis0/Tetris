#pragma once
#ifndef TETRIS_TETRIS_FUNCTIONS_H
#define TETRIS_TETRIS_FUNCTIONS_H

#include "constants.h"
#include "functions.h"
#include "Piece.h"
#include "myQueue.h"

#include <array>
#include <list>
#include <queue>
#include <unistd.h>

void fillArrayOfRandPieceTypes();
void initField(char field[FIELD_HEIGHT][FIELD_WIDTH]);
void init(char field[FIELD_HEIGHT][FIELD_WIDTH], int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines);
void arrayBiasToLeftTop(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
bool arrContains(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> arr, pair<int, int> cords);
void printField(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *heldPiece, array<int, 4> statArr);
void fillMatrixWith(char **matrix, int height, int width, char c);
void printMatrix(char **matrix, int height, int width);
void swapPiece(Piece *&a, Piece *&b);
list<string> getInputList(bool notO_Block);
int minPosX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos);
int minPosY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos, int endPos);
void sortXY(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr);
void setArraysPixels(char field[FIELD_HEIGHT][FIELD_WIDTH], const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, char pixel);
void downgradeMatrix(char field[FIELD_HEIGHT][FIELD_WIDTH], int row);
int clearLines(char field[FIELD_HEIGHT][FIELD_WIDTH], int& numOfClearingLines);
void checkLines(char field[FIELD_HEIGHT][FIELD_WIDTH], int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines);
bool checkLose(char field[FIELD_HEIGHT][FIELD_WIDTH]);
int getMinX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMaxX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMinY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int getMaxY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
Piece *createPiece(Piece *piece);
Piece *createPieceByCode(int code);
Piece *getRandPiece();
void endGame(int score, int totalClearedLines, int numOfTetris, int numOfClearingLines);
int getAction(char c);
void doAction(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *&piece, int action, Piece *&heldPiece);
void player();

#endif //TETRIS_TETRIS_FUNCTIONS_H
