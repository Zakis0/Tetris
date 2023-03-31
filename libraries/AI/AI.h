#pragma once
#ifndef TETRIS_AI_H
#define TETRIS_AI_H

#include "../../Config/constants.h"
#include "../Piece/Piece.h"
#include "../TetrisFunctions/tetrisFunctions.h"
#include "../GeneticAlgorithm/geneticAlgorithm.h"

#include <vector>
#include <set>
#include <thread>
#include <chrono>
#include <cmath>

void executeListOfActions(Matrix& field, Piece *&piece, const string& actionsList, Piece *&heldPiece, array<int, 4> statArr, Queue& queueOfRandPieceTypes, int delay = 0, bool pField = false);
pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>> getAllActionsAndTheirPosition(Matrix& field, Piece *piece, Piece *heldPiece, array<int, 4> statArr, Queue& queueOfRandPieceTypes);
int getColumnHeight(Matrix& field, int column);
int bumpinessCheck(Matrix& field, int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfBlocksInRightmostLaneCheck(Matrix& field);
int numOfBlocksAboveHolesCheck(Matrix& field, int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfClearingLinesCheck(Matrix& field);
int tetrisCheck(Matrix& field);
int clearingNotFourLinesCheck(Matrix& field);
int getMaxHeight(int minY);
int numOfGlobalHolesCheck(Matrix& field, int startColumn = 0, int endColumn = FIELD_WIDTH);
int numOfPillarsCheck(Matrix& field, int startColumn = 0, int endColumn = FIELD_WIDTH);
double getScore(Matrix& field, const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params = scoreParams);
pair<string, double> getBestPosition(Matrix& field, const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params);
array<int, 4> AI(bool printGame = true, const double *params = scoreParams, int AIDelay = AI_MOVES_DELAY);
void testAI(int numOfGame);

#endif //TETRIS_AI_H
