#include "AI.h"

void executeListOfActions(Matrix& field, Piece *&piece, const string& actionsList, Piece *&heldPiece, array<int, 4> statArr, Queue& queueOfRandPieceTypes, int delay, bool pField) {
    for (char action: actionsList) {
        doAction(field, piece, getAction(action), heldPiece);
        if (pField) {
            printField(field, heldPiece, statArr, queueOfRandPieceTypes);
        }
        usleep(delay);
    }
}

pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>
getAllActionsAndTheirPosition(Matrix& field, Piece *piece, Piece *heldPiece, array<int, 4> statArr, Queue& queueOfRandPieceTypes) {
    set<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positionsSet;
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> cordsArr;

    vector<string> actions;
    vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>> positions;

    Piece *tempPiece = piece;

    list<string> actionsList = getInputList((tempPiece->pieceName != O_BLOCK_NAME));

    for (const string &str: actionsList) {
        tempPiece = createPiece(piece);
        executeListOfActions(field, tempPiece, str, heldPiece, statArr, queueOfRandPieceTypes);

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

int getColumnHeight(Matrix& field, int column) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        if (field.getField()[i][column] != EMPTY_PIXEL) {
            return FIELD_HEIGHT - i;
        }
    }
    return 0;
}

int bumpinessCheck(Matrix& field, int startColumn, int endColumn) {
    // calculate different between columns height
    int sum = 0;

    if (startColumn < 0) {
        startColumn;
    }
    if (endColumn > FIELD_WIDTH - 1) {
        endColumn = FIELD_WIDTH - 1;
    }

    for (int i = startColumn; i < endColumn; ++i) {
        sum += abs(getColumnHeight(field, i) - getColumnHeight(field, i + 1));
    }
    return sum;
}

int numOfBlocksInRightmostLaneCheck(Matrix& field) {
    int num = 0;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        if (field.getField()[i][FIELD_WIDTH - 1] != EMPTY_PIXEL) {
            ++num;
        }
    }
    return num;
}

int numOfBlocksAboveHolesCheck(Matrix& field, int startColumn, int endColumn) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = FIELD_HEIGHT - 1; i >= 0; --i) {
            if (!flag && field.getField()[i][j] != EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field.getField()[i][j] != EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfClearingLinesCheck(Matrix& field) {
    int num = 0;
    bool filled;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        filled = true;
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field.getField()[i][j] == EMPTY_PIXEL) {
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

int tetrisCheck(Matrix& field) {
    if (numOfClearingLinesCheck(field) == 4) {
        return -1;
    }
    return 0;
}

int clearingNotFourLinesCheck(Matrix& field) {
    int num = numOfClearingLinesCheck(field);
    if (num == 4) {
        return 0;
    }
    else {
        return 1;
    }
}

int getMaxHeight(int minY) {
    return FIELD_HEIGHT - minY;
}

int numOfGlobalHolesCheck(Matrix& field, int startColumn, int endColumn) {
    int num = 0;
    bool flag;
    for (int j = startColumn; j < endColumn; ++j) {
        flag = false;
        for (int i = 0; i < FIELD_HEIGHT; ++i) {
            if (!flag && field.getField()[i][j] == EMPTY_PIXEL) {
                continue;
            }
            else {
                flag = true;
            }
            if (flag && field.getField()[i][j] == EMPTY_PIXEL) {
                ++num;
            }
        }
    }
    return num;
}

int numOfPillarsCheck(Matrix& field, int startColumn, int endColumn) {
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

double getScore(Matrix& field, const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, const double *params) {
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
        posScore += params[5] * numOfBlocksInRightmostLaneCheck(field);
    }
    posScore += params[6] * tetrisCheck(field);
    posScore += params[7] * numOfPillarsCheck(field, minX, maxX + 1);
    return posScore;
}

pair<string, double> getBestPosition(Matrix& field, const pair<vector<string>, vector<array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>>>& posList, const double *params) {
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
array<int, 4> AI(bool printGame, const double *params, int AIDelay) {
    Matrix field(FIELD_HEIGHT, FIELD_WIDTH, EMPTY_PIXEL, EMPTY_PIXEL);
    int score, totalClearedLines, numOfTetris, numOfClearingLines;

    int posInArrOfRandPiece;
    int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES];

    Queue queueOfRandPieceTypes;

    init(field, score, totalClearedLines, numOfTetris, numOfClearingLines, posInArrOfRandPiece, arrOfRandPieceTypes);

    Piece *piece = getRandPiece(queueOfRandPieceTypes, arrOfRandPieceTypes, posInArrOfRandPiece);
    Piece *heldPiece = new I_Block();

    string actionsToBestPosition;

    while (!checkLose(field)) {
        piece = getRandPiece(queueOfRandPieceTypes, arrOfRandPieceTypes, posInArrOfRandPiece);
        piece->setPixels(field);
        if (printGame) {
            printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);
        }
        auto allActionsAndTheirPosition = getAllActionsAndTheirPosition(field, piece, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);
        auto allActionsAndTheirPositionHeldPiece = getAllActionsAndTheirPosition(field, heldPiece, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);

        pair<string, double> bestPosition = getBestPosition(field, allActionsAndTheirPosition, params);
        pair<string, double> bestPositionHeldPiece = getBestPosition(field, allActionsAndTheirPositionHeldPiece, params);

        if (bestPosition.second <= bestPositionHeldPiece.second) {
            actionsToBestPosition = bestPosition.first;
        }
        else {
            actionsToBestPosition = CONTROL_HELD_PIECE + bestPositionHeldPiece.first;
        }

        executeListOfActions(field, piece, actionsToBestPosition, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes, AIDelay, printGame);

        delete piece;
        checkLines(field, score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    if (printGame) {
        printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);
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
                    result = AI(false, scoreParams, 0);
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