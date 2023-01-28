#pragma once
#ifndef TETRIS_AI_H
#define TETRIS_AI_H

#include "constants.h"
#include "Piece.h"
#include "tetrisFunctions.h"
#include "geneticAlgorithm.h"

#include <vector>
#include <set>
#include <thread>
#include <chrono>
#include <cmath>

void executeListOfActions(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *&piece, const string& actionsList, Piece *&heldPiece, array<int, 4> statArr, int delay = 0, bool pField = false);
pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>> getAllActionsAndTheirPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *piece, Piece *heldPiece, array<int, 4> statArr);
int getColumnHeight(char field[FIELD_HEIGHT][FIELD_WIDTH], int column);
int bumpinessCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfBlocksInRightmostLaneCheck(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr);
int numOfBlocksAboveHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfClearingLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]);
int tetrisCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]);
int clearingNotFourLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]);
int getMaxHeight(int minY);
int numOfGlobalHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfPillarsCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn = 0, int endColumn = FIELD_WIDTH);
double getScore(char field[FIELD_HEIGHT][FIELD_WIDTH], const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params = scoreParams);
pair<string, double> getBestPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params);
array<int, 4> AI(bool printGame = true, const double *params = scoreParams);
void testAI(int numOfGame);

#endif //TETRIS_AI_H
