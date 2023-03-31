#include <stack>
#include "tetrisFunctions.h"

void fillArrayOfRandPieceTypes(int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES]) {
    for (int i = 0; i < LENGTH_OF_ARRAY_WITH_RAND_PIECES; ++i) {
        arrOfRandPieceTypes[i] = i % NUM_OF_PIECES;
    }
}
void initField(Matrix& field) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field.getField()[i][j] = EMPTY_PIXEL;
        }
    }
}
void init(Matrix& field, int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines, int& posInArrOfRandPiece, int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES]) {
    initRand();
    initField(field);

    score = 0;
    totalClearedLines = 0;
    numOfTetris = 0;
    numOfClearingLines = 0;

    posInArrOfRandPiece = 0;

    fillArrayOfRandPieceTypes(arrOfRandPieceTypes);
}

void arrayBiasToLeftTop(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    while (min_(NUM_OF_BLOCK_IN_PIECE, arr[0].first, arr[1].first, arr[2].first, arr[3].first) > 0) {
        --arr[0].first;
        --arr[1].first;
        --arr[2].first;
        --arr[3].first;
    }
    while (min_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second) > 0) {
        --arr[0].second;
        --arr[1].second;
        --arr[2].second;
        --arr[3].second;
    }
}
bool arrContains(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> arr, pair<int, int> cords) {
    for (int i = 0; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first == cords.first && arr[i].second == cords.second) {
            return true;
        }
    }
    return false;
}

void fillMatrixWith(char **matrix, int height, int width, char c) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = c;
        }
    }
}

void printField(Matrix& field, Piece *heldPiece, array<int, 4> statArr, Queue &queueOfRandPieceTypes) {
    array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> pieceCord;

    Matrix gameField = field;
    gameField.emptyPixel = SCREEN_EMPTY_PIXEL;

    Matrix heldPieceField(PIECE_FIELD_HEIGHT, PIECE_FIELD_WIDTH, SCREEN_EMPTY_PIXEL, SCREEN_EMPTY_PIXEL);

    pieceCord = heldPiece->getCords();
    arrayBiasToLeftTop(pieceCord);

    for (auto cord : pieceCord) {
        heldPieceField.getField()[cord.second][cord.first] = heldPiece->getPieceChar();
    }

    Matrix nextPieceField(NUM_OF_SHOWN_NEXT_PIECES * (PIECE_FIELD_HEIGHT + 1) - 1, PIECE_FIELD_WIDTH, SCREEN_EMPTY_PIXEL, SCREEN_EMPTY_PIXEL);

    int* queue = queueOfRandPieceTypes.getQueue();
    Piece *piece;

    int dY = 0;

    for (int i = 0; i < NUM_OF_SHOWN_NEXT_PIECES; ++i) {
        try {
            piece = createPieceByCode(queue[i]);
        }
        catch (int code) {
            cerr << "Piece with code " << code << " do not exist" << endl;
        }
        pieceCord = piece->getCords();
        arrayBiasToLeftTop(pieceCord);
        for (auto cord : pieceCord) {
            nextPieceField.getField()[cord.second + dY][cord.first] = piece->getPieceChar();
        }
        dY += PIECE_FIELD_HEIGHT + 1;
    }
    Matrix infoBlock = heldPieceField.connectMatrixVertical(nextPieceField, NEXT_PIECE_FIELD_TOP_BIAS);

    Matrix screen = gameField.connectMatrixHorizontal(infoBlock, INFO_BLOCK_LEFT_BIAS, INFO_BLOCK_TOP_BIAS);

    screen.printMatrix();

    cout << "Score: " << statArr[0] << "; NCL: " << statArr[1] << "; Tetris rate: " << getPercent(statArr[2], statArr[3]) << "%" << endl;
}

void swapPiece(Piece *&a, Piece *&b) {
    Piece *temp = a;
    a = b;
    b = temp;
}

list<string> getInputList(bool notO_Block) {
    list<string> result;
    for (int numOfHorMov = FIELD_WIDTH / 2 - 1; numOfHorMov >= -(FIELD_WIDTH / 2 - 1); --numOfHorMov) {
        for (int numOfRot = NUM_OF_ROTATION - 1; numOfRot >= -(NUM_OF_ROTATION - 1); --numOfRot) {
            string str = "";
            if (notO_Block) {
                if (numOfRot > 0) str += string(numOfRot, CONTROL_SPIN_CLOCKWISE);
                else str += string(abs(numOfRot), CONTROL_SPIN_COUNTERCLOCKWISE);
            }
            if (numOfHorMov > 0) str += string(numOfHorMov, CONTROL_MOVE_LEFT);
            else str += string(abs(numOfHorMov), CONTROL_MOVE_RIGHT);
            str += string(1, CONTROL_HARD_DROP);
            result.push_back(str);
        }
    }
    return result;
}

int minPosX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos) {
    int mPos = startPos;
    for (int i = startPos; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first < arr[mPos].first) {
            mPos = i;
        }
    }
    return mPos;
}
int minPosY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr, int startPos, int endPos) {
    int mPos = startPos;
    for (int i = startPos; i < endPos; ++i) {
        if (arr[i].second < arr[mPos].second) {
            mPos = i;
        }
    }
    return mPos;
}
void sortXY(array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE> &arr) {
    int mPos;
    pair<int, int> temp;

    for (int i = 0; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        mPos = minPosX(arr, i);
        temp = arr[mPos];
        arr[mPos] = arr[i];
        arr[i] = temp;
    }

    int curX = arr[0].first, curXFirstPos = 0;
    for (int i = 1; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        if (arr[i].first != curX) {
            for (int j = curXFirstPos; j < i; ++j) {
                mPos = minPosY(arr, j, i);
                temp = arr[mPos];
                arr[mPos] = arr[j];
                arr[j] = temp;
            }
            curXFirstPos = i;
            curX = arr[i].first;
        }
    }
    for (int i = curXFirstPos; i < NUM_OF_BLOCK_IN_PIECE; ++i) {
        mPos = minPosY(arr, i, NUM_OF_BLOCK_IN_PIECE);
        temp = arr[mPos];
        arr[mPos] = arr[i];
        arr[i] = temp;
    }
}

void setArraysPixels(Matrix& field, const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr, char pixel) {
    for (pair<int, int> cord : arr) {
        field.getField()[cord.second][cord.first] = pixel;
    }
}
void downgradeMatrix(Matrix& field, int row) {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        field.getField()[row][i] = EMPTY_PIXEL;
    }
    for (int i = row - 1; i >= 0; --i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            field.getField()[i + 1][j] = field.getField()[i][j];
            field.getField()[i][j] = EMPTY_PIXEL;
        }
    }
}
int clearLines(Matrix& field, int& numOfClearingLines) {
    int numOfClearLines = 0;
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
            downgradeMatrix(field, i);
            ++numOfClearLines;
        }
    }
    if (numOfClearLines > 0) {
        ++numOfClearingLines;
    }
    return numOfClearLines;
}
void checkLines(Matrix& field, int& score, int& totalClearedLines, int& numOfTetris, int& numOfClearingLines) {
    int numOfClearedLines = clearLines(field, numOfClearingLines);
    totalClearedLines += numOfClearedLines;
    switch (numOfClearedLines) {
        case 1: {
            score += SCORE_1_LINE;
            break;
        }
        case 2: {
            score += SCORE_2_LINE;
            break;
        }
        case 3: {
            score += SCORE_3_LINE;
            break;
        }
        case 4: {
            score += SCORE_4_LINE;
            ++numOfTetris;
            break;
        }
    }
}
bool checkLose(Matrix& field) {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        if (field.getField()[0][i] != EMPTY_PIXEL) return true;
    }
    return false;
}

int getMinX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return arr[0].first;
}
int getMaxX(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return arr[NUM_OF_BLOCK_IN_PIECE - 1].first;
}
int getMinY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return min_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second);
}
int getMaxY(const array<pair<int, int>, NUM_OF_BLOCK_IN_PIECE>& arr) {
    return max_(NUM_OF_BLOCK_IN_PIECE, arr[0].second, arr[1].second, arr[2].second, arr[3].second);
}

Piece *createPiece(Piece *piece) {
    if (smpStr(piece->pieceName, I_BLOCK_NAME)) {
        return new I_Block();
    }
    else if (smpStr(piece->pieceName, J_BLOCK_NAME)) {
        return new J_Block();
    }
    else if (smpStr(piece->pieceName, L_BLOCK_NAME)) {
        return new L_Block();
    }
    else if (smpStr(piece->pieceName, O_BLOCK_NAME)) {
        return new O_Block();
    }
    else if (smpStr(piece->pieceName, S_BLOCK_NAME)) {
        return new S_Block();
    }
    else if (smpStr(piece->pieceName, T_BLOCK_NAME)) {
        return new T_Block();
    }
    else if (smpStr(piece->pieceName, Z_BLOCK_NAME)) {
        return new Z_Block();
    }
    return new I_Block();
}
Piece *createPieceByCode(int code) {
    switch (code) {
        case 0: return new I_Block();
        case 1: return new J_Block();
        case 2: return new L_Block();
        case 3: return new O_Block();
        case 4: return new S_Block();
        case 5: return new T_Block();
        case 6: return new Z_Block();
        default: throw (code);
    }
}
Piece *getRandPiece(Queue &queueOfRandPieceTypes, int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES], int& posInArrOfRandPiece) {
    if (queueOfRandPieceTypes.getSize() < NUM_OF_SHOWN_NEXT_PIECES + 1) {
        for (int i = 0; i < NUM_OF_PIECES_TO_ADD_IN_QUEUE; ++i) {
            posInArrOfRandPiece %= LENGTH_OF_ARRAY_WITH_RAND_PIECES;
            if (posInArrOfRandPiece == 0) {
                randSort(arrOfRandPieceTypes, LENGTH_OF_ARRAY_WITH_RAND_PIECES);
            }
            queueOfRandPieceTypes.push(arrOfRandPieceTypes[posInArrOfRandPiece++]);
        }
    }
    switch (queueOfRandPieceTypes.getAndPopFront()) {
        case 0: return new I_Block();
        case 1: return new J_Block();
        case 2: return new L_Block();
        case 3: return new O_Block();
        case 4: return new S_Block();
        case 5: return new T_Block();
        case 6: return new Z_Block();
    }
    error("Non-existent piece code");
    return new I_Block();
}

void endGame(int score, int totalClearedLines, int numOfTetris, int numOfClearingLines) {
    cout << "Score: " << score << endl;
    cout << "Num of cleared lines: " << totalClearedLines << endl;
    cout << "Tetris rate: " << getPercent(numOfTetris, numOfClearingLines) << "%" << endl;
}

int getAction(char c) {
    switch (c) {
        case CONTROL_MOVE_DOWN: {
            return 0;
        }
        case CONTROL_MOVE_LEFT: {
            return 1;
        }
        case CONTROL_MOVE_RIGHT: {
            return 2;
        }
        case CONTROL_HARD_DROP: {
            return 3;
        }
        case CONTROL_SPIN_CLOCKWISE: {
            return 4;
        }
        case CONTROL_SPIN_COUNTERCLOCKWISE: {
            return 5;
        }
        case CONTROL_HELD_PIECE: {
            return 6;
        }
    }
    return '0';
}

void doAction(Matrix& field, Piece *&piece, int action, Piece *&heldPiece) {
    switch (action) {
        case MOVE_DOWN: {
            piece->moveDown(field);
            break;
        }
        case MOVE_LEFT: {
            piece->moveLeft(field);
            break;
        }
        case MOVE_RIGHT: {
            piece->moveRight(field);
            break;
        }
        case HARD_DROP: {
            piece->hardDrop(field);
            break;
        }
        case SPIN_CLOCKWISE: {
            piece->wallKickSpin(field, true);
            break;
        }
        case SPIN_COUNTERCLOCKWISE: {
            piece->wallKickSpin(field, false);
            break;
        }
        case HELD_PIECE: {
            swapPiece(piece, heldPiece);
            heldPiece->clearPixels(field);
            heldPiece->setInitialState();
            piece->setPixels(field);
            break;
        }
    }
}

void player() {
    Matrix field(FIELD_HEIGHT, FIELD_WIDTH, EMPTY_PIXEL, EMPTY_PIXEL);
    int score, totalClearedLines, numOfTetris, numOfClearingLines;

    int posInArrOfRandPiece;
    int arrOfRandPieceTypes[LENGTH_OF_ARRAY_WITH_RAND_PIECES];

    Queue queueOfRandPieceTypes;

    init(field, score, totalClearedLines, numOfTetris, numOfClearingLines, posInArrOfRandPiece, arrOfRandPieceTypes);

    Piece *piece = getRandPiece(queueOfRandPieceTypes, arrOfRandPieceTypes, posInArrOfRandPiece);
    Piece *heldPiece = new I_Block();

    char input;

    while (!checkLose(field)) {
        piece = getRandPiece(queueOfRandPieceTypes, arrOfRandPieceTypes, posInArrOfRandPiece);
        piece->setPixels(field);
        printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);
        while (piece->canMoveDown(field)) {
            cin >> input;
            doAction(field, piece, getAction(input), heldPiece);
            printField(field, heldPiece, {score, totalClearedLines, numOfTetris, numOfClearingLines}, queueOfRandPieceTypes);
        }
        delete piece;
        checkLines(field, score, totalClearedLines, numOfTetris, numOfClearingLines);
    }
    endGame(score, totalClearedLines, numOfTetris, numOfClearingLines);
}