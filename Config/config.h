#pragma once
#ifndef TETRIS_CONFIG_H
#define TETRIS_CONFIG_H

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 15

#define MAX_SCREEN_HEIGHT 30
#define MAX_SCREEN_WIDTH 16

#define NUM_OF_SHOWN_NEXT_PIECES 3

#define LENGTH_OF_ARRAY_WITH_RAND_PIECES 7

const int NUM_OF_SWAP_IN_RAND_SORT = 20;

//const int AI_MOVES_DELAY = 500000;
const int AI_MOVES_DELAY = 0;

const char I_BlockChar = '@';
const char J_BlockChar = '#';
const char L_BlockChar = '$';
const char O_BlockChar = '%';
const char S_BlockChar = '&';
const char T_BlockChar = 'G';
const char Z_BlockChar = 'W';

const char EMPTY_PIXEL = '.';

const char SCREEN_EMPTY_PIXEL = ' ';

const char CONTROL_MOVE_DOWN = 's';
const char CONTROL_MOVE_LEFT = 'a';
const char CONTROL_MOVE_RIGHT = 'd';
const char CONTROL_HARD_DROP = 'w';
const char CONTROL_SPIN_CLOCKWISE = 'e';
const char CONTROL_SPIN_COUNTERCLOCKWISE = 'q';
const char CONTROL_HELD_PIECE = 'r';

const int SCORE_1_LINE = 100;
const int SCORE_2_LINE = 300;
const int SCORE_3_LINE = 700;
const int SCORE_4_LINE = 1500;

#endif //TETRIS_CONFIG_H
