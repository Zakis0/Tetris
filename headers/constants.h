#pragma once
#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H

#include <string>

#include "config.h"

#define NUM_OF_SCORE_PARAMS 8

#define PIECE_FIELD_HEIGHT 2
#define PIECE_FIELD_WIDTH 4

#define NEXT_PIECE_FIELD_TOP_BIAS 2
#define INFO_BLOCK_TOP_BIAS 1
#define INFO_BLOCK_LEFT_BIAS 1

#define NUM_OF_PIECES_TO_ADD_IN_QUEUE 5

using namespace std;

const double BUMPINESS_WEIGHT = 1;
const double BLOCKS_ABOVE_HOLES_WEIGHT = 8;
const double MINIMISE_HEIGHT_WEIGHT = 1.5;
const double HOLES_UNDER_BLOCKS_WEIGHT = 4.5;
const double CLEARING_NOT_FOUR_LINES_WEIGHT = 0.5;
const double BLOCKS_IN_RIGHTMOST_LANE_WEIGHT = -1.5;
const double TETRIS_WEIGHT = 2;
const double NUM_OF_PILLARS_WEIGHT = 4;

//double scoreParams[NUM_OF_SCORE_PARAMS] = {BUMPINESS_WEIGHT, BLOCKS_ABOVE_HOLES_WEIGHT, MINIMISE_HEIGHT_WEIGHT, HOLES_UNDER_BLOCKS_WEIGHT, CLEARING_NOT_FOUR_LINES_WEIGHT, BLOCKS_IN_RIGHTMOST_LANE_WEIGHT, TETRIS_WEIGHT, NUM_OF_PILLARS_WEIGHT};
//double scoreParams[NUM_OF_SCORE_PARAMS] = {1, 8, 1.5, 4.5, 0.5, -1.5, 2, 4};
const double scoreParams[NUM_OF_SCORE_PARAMS] = {0.25, 4.25, 1, -0.25, 3, 1.25, 0, 0};

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

const int FIELD_MIDDLE = FIELD_WIDTH / 2 - 1;

#endif //TETRIS_CONSTANTS_H
