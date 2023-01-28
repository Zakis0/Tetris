#include "../headers/AI.h"

void executeListOfActions(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *&piece, const string& actionsList, Piece *&heldPiece, array<int, 4> statArr, int delay, bool pField) {
    for (char action: actionsList) {
        doAction(field, piece, getAction(action), heldPiece);
        if (pField) {
            printField(field, heldPiece, statArr);
        }
        usleep(delay);
    }
}

pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
getAllActionsAndTheirPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], Piece *piece, Piece *heldPiece, array<int, 4> statArr) {
    set<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positionsSet;
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> cordsArr;

    vector<string> actions;
    vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positions;

    Piece *tempPiece = piece;

    list<string> actionsList = getInputList((tempPiece->pieceName != O_BLOCK_NAME));

    for (const string &str: actionsList) {
        tempPiece = createPiece(piece);
        executeListOfActions(field, tempPiece, str, heldPiece, statArr);

        if (!tempPiece->canMoveDown(field) && tempPiece->isPixelsInField()) {
            cordsArr = tempPiece->getCords();
            sortXY(cordsArr);
            if (positionsSet.insert(cordsArr).second) {
                actions.push_back(str);
                positions.push_back(cordsArr);
            }
        }
        tempPiece->clearPixels(field);
        delete tempPiece;
    }
    return make_pair(actions, positions);
}

int getColumnHeight(char field[FIELD_HEIGHT][FIELD_WIDTH], int column) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        if (field[i][column] != EMPTY_PIXEL) {
            return FIELD_HEIGHT - i;
        }
    }
    return 0;
}

int bumpinessCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn, int endColumn) {
    // calculate different between columns height
    int sum = 0;

    if (startColumn < 0) {
        startColumn;
    }
    if (endColumn > FIELD_WIDTH - 1) {
        endColumn - 1;
    }

    for (int i = startColumn; i < endColumn; ++i) {
        sum += abs(getColumnHeight(field, i) - getColumnHeight(field, i + 1));
    }
    return sum;
}

int numOfBlocksInRightmostLaneCheck(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    int num = 0;
    for (pair<int, int> cords : arr) {
        if (cords.first == FIELD_WIDTH - 1) {
            ++num;
        }
    }
    return num;
}

int numOfBlocksAboveHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn, int endColumn) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = FIELD_HEIGHT - 1; i >= 0; --i) {
            if (!flag && field[i][j] != EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field[i][j] != EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfClearingLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int num = 0;
    bool filled;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        filled = true;
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == EMPTY_PIXEL) {
                filled = false;
                break;
            }
        }
        if (filled) {
            ++num;
        }
    }
    return num;
}

int tetrisCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    if (numOfClearingLinesCheck(field) == 4) {
        return -1;
    }
    return 0;
}

int clearingNotFourLinesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH]) {
    int num = numOfClearingLinesCheck(field);
    if (num == 4) {
        return -1;
    }
    else {
        return 1;
    }
}

int getMaxHeight(int minY) {
    return FIELD_HEIGHT - minY;
}

int numOfGlobalHolesCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn, int endColumn) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = 0; i < FIELD_HEIGHT; ++i) {
            if (!flag && field[i][j] == EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field[i][j] == EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfPillarsCheck(char field[FIELD_HEIGHT][FIELD_WIDTH], int startColumn, int endColumn) {
    int numOfPillars = 0;
    for (int j = startColumn; j < endColumn; ++j) {
        if (j == 0) {
            if (getColumnHeight(field, j + 1) - getColumnHeight(field, j) >= 3) {
                ++numOfPillars;
            }
        }
        else if (j == FIELD_WIDTH - 1) {
            if (getColumnHeight(field, j - 1) - getColumnHeight(field, j) >= 3) {
                ++numOfPillars;
            }
        }
        else {
            if ((getColumnHeight(field, j - 1) - getColumnHeight(field, j) >= 3) &&
                (getColumnHeight(field, j + 1) - getColumnHeight(field, j) >= 3)) {
                ++numOfPillars;
            }
        }
    }
    return numOfPillars;
}

double getScore(char field[FIELD_HEIGHT][FIELD_WIDTH], const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params) {
    double posScore = 0;

    int maxX = getMaxX(arr);
    int minX = getMinX(arr);
    int maxY = getMaxY(arr);
    int minY = getMinY(arr);

    posScore += params[0] * bumpinessCheck(field, minX - 1, maxX + 1);

    posScore += params[1] * numOfBlocksAboveHolesCheck(field, minX, maxX + 1);
    posScore += params[2] * getMaxHeight(minY);
    posScore += params[3] * numOfGlobalHolesCheck(field, minX, maxX + 1);
    posScore += params[4] * clearingNotFourLinesCheck(field);
    if (maxX == FIELD_WIDTH - 1) {
        posScore += params[5] * numOfBlocksInRightmostLaneCheck(arr);
    }
    posScore += params[6] * tetrisCheck(field);
    posScore += params[7] * numOfPillarsCheck(field, minX, maxX + 1);
    return posScore;
}

pair<string, double> getBestPosition(char field[FIELD_HEIGHT][FIELD_WIDTH], const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params) {
    int bestPosIndex = 0;
    double curPosScore, bestPosScore = 10000000;

    for (int i = 0; i < posList.first.size(); ++i) {
        setArraysPixels(field, posList.second[i], '@');
        curPosScore = getScore(field, posList.second[i], params);
        setArraysPixels(field, posList.second[i], EMPTY_PIXEL);
        if (curPosScore < bestPosScore) {
            bestPosScore = curPosScore;
            bestPosIndex = i;
        }
    }
    return make_pair(posList.first[bestPosIndex], bestPosScore);
}

// {score, totalClearedLines, numOfTetris, numOfClearingLines}
array<int, 4> AI(bool printGame, const double *params) {
    char field[FIELD_HEIGHT][FIELD_WIDTH];
    int score, totalClearedLines, numOfTetris, numOfClearingLines;
    init(field, score, totalClearedLines, numOfTetris, numOfClearingLines);

    Piece *piece = getRandPiece(), *heldPiece = new I_Block();

    string actionsToBestPosition;

    while (!checkLose(field)) {
        piece = getRandPiece();
        piece->setPixels(field);
        if (printGame) {
            printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines});
        }
        auto allActionsAndTheirPosition = getAllActionsAndTheirPosition(field, piece, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines});
        auto allActionsAndTheirPositionHeldPiece = getAllActionsAndTheirPosition(field, heldPiece, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines});

        pair<string, double> bestPosition = getBestPosition(field, allActionsAndTheirPosition, params);
        pair<string, double> bestPositionHeldPiece = getBestPosition(field, allActionsAndTheirPositionHeldPiece, params);

        if (bestPosition.second <= bestPositionHeldPiece.second) {
            actionsToBestPosition = bestPosition.first;
        }
        else {
            actionsToBestPosition = CONTROL_HELD_PIECE + bestPositionHeldPiece.first;
        }

        executeListOfActions(field, piece, actionsToBestPosition, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, AI_MOVES_DELAY, printGame);

        delete piece;
        checkLines(field, score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    if (printGame) {
        printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines});
        endGame(score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    return {score, totalClearedLines, numOfTetris, numOfClearingLines};
}

void testAI(int numOfGame) {
    float avgScore = 0, avgClearedLines = 0, avgTetrisRate = 0;
    array<int, 4> result;
    vector <thread> thVec;
    auto totalStartTime = chrono::high_resolution_clock::now();
    auto totalEndTime = chrono::high_resolution_clock::now();
    cout << "Score; Cleared lines; Tetris rate; Duration" << endl;

    for (int i = 0; i < numOfGame; ++i) {
        thVec.emplace_back(
                [&result, &avgScore, &avgClearedLines, &avgTetrisRate, &totalStartTime, &totalEndTime](){
                    result = AI(false);
                    avgScore += result[0];
                    avgClearedLines += result[1];
                    avgTetrisRate += getPercent(result[2], result[3]);

                    totalEndTime = chrono::high_resolution_clock::now();
                    chrono::duration<float> totalDuration = totalEndTime - totalStartTime;

                    cout << result[0] << " " << result[1] << " " <<
                         getPercent(result[2], result[3]) << "% " << totalDuration.count() << endl;
                });
    }
    for (int i = 0; i < numOfGame; ++i) {
        thVec.at(i).join();
    }
    avgScore /= numOfGame;
    avgClearedLines /= numOfGame;
    avgTetrisRate = floor(avgTetrisRate / numOfGame * 100) / 100;

    cout << "avgScore: " << avgScore << endl;
    cout << "avgClearedLines: " << avgClearedLines << endl;
    cout << "avgTetrisRate: " << avgTetrisRate << "%" << endl;
}