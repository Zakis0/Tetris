#include "matrix.h"

Matrix::Matrix() {
    this->height = 16;
    this->width = 16;
    this->emptyPixel = BASE_EMPTY_PIXEL;

    initMatrix(emptyPixel);
}

Matrix::Matrix(unsigned int height, unsigned int width, char emptyPixel, char initPixel) {
    this->height = height;
    this->width = width;
    this->emptyPixel = emptyPixel;

    initMatrix(initPixel);
}

Matrix::Matrix(Matrix &matrix) {
    height = matrix.height;
    width = matrix.width;
    emptyPixel = matrix.emptyPixel;

    field = new char* [height];
    for (int i = 0; i < height; ++i) {
        field[i] = new char[width];
        for (int j = 0; j < width; ++j) {
            field[i][j] = matrix.field[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < height; ++i) {
        delete[] field[i];
    }
    delete[] field;
}

void Matrix::initMatrix(char initPixel) {
    field = new char* [height];
    for (int i = 0; i < height; ++i) {
        field[i] = new char[width];
        for (int j = 0; j < width; ++j) {
            field[i][j] = initPixel;
        }
    }
}

void Matrix::fillMatrixWith(char pixel) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = pixel;
        }
    }
}

Matrix Matrix::connectMatrixHorizontal(const Matrix& connectableMatrix, unsigned int connectableMatrixLeftBias, unsigned int connectableMatrixTopBias) {
    int connectedMatrixHeight = max(height, connectableMatrix.height + connectableMatrixTopBias);
    int connectedMatrixWidth = width + connectableMatrix.width + connectableMatrixLeftBias;

    Matrix connectedMatrix(connectedMatrixHeight, connectedMatrixWidth, emptyPixel, emptyPixel);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width + connectableMatrix.width + connectableMatrixLeftBias; ++j) {
            connectedMatrix.field[i][j] = emptyPixel;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            connectedMatrix.field[i][j] = field[i][j];
        }
    }
    for (int i = 0; i < connectableMatrix.height; ++i) {
        for (int j = 0; j < connectableMatrix.width; ++j) {
            connectedMatrix.field[i + connectableMatrixTopBias][j + width + connectableMatrixLeftBias] = connectableMatrix.field[i][j];
        }
    }
    return connectedMatrix;
}

Matrix Matrix::connectMatrixVertical(const Matrix& connectableMatrix, unsigned int connectableMatrixTopBias, unsigned int connectableMatrixLeftBias) {
    int connectedMatrixHeight = height + connectableMatrix.height + connectableMatrixTopBias;
    int connectedMatrixWidth = max(width, connectableMatrix.width + connectableMatrixLeftBias);

    Matrix connectedMatrix(connectedMatrixHeight, connectedMatrixWidth, emptyPixel, emptyPixel);

    for (int i = 0; i < height + connectableMatrix.height + connectableMatrixTopBias; ++i) {
        for (int j = 0; j < width + connectableMatrixLeftBias; ++j) {
            connectedMatrix.field[i][j] = emptyPixel;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            connectedMatrix.field[i][j] = field[i][j];
        }
    }
    for (int i = 0; i < connectableMatrix.height; ++i) {
        for (int j = 0; j < connectableMatrix.width; ++j) {
            connectedMatrix.field[i + height + connectableMatrixTopBias][j + connectableMatrixLeftBias] = connectableMatrix.field[i][j];
        }
    }
    for (int i = 0; i < connectableMatrix.height + height + connectableMatrixTopBias; ++i) {
        for (int j = 0; j < connectableMatrix.width + connectableMatrixLeftBias; ++j) {
        }
    }
    return connectedMatrix;
}

void Matrix::printMatrix() {
    for (int i = 0; i < height; ++i) {
        cout << field[i][0];
        for (int j = 1; j < width; ++j) {
            cout << " " << field[i][j];
        }
        cout << endl;
    }
}

void Matrix::clearMatrix() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = emptyPixel;
        }
    }
}

void Matrix::copyField(const Matrix &matrix) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = matrix.field[i][j];
        }
    }
}

Matrix Matrix::scaleMatrix(unsigned int scale) {
    if (scale < 1) {
        error("Scale can `t be 0");
    }
    Matrix scaledMatrix = Matrix(scale * height, scale * width, emptyPixel);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < scale; ++k) {
                for (int l = 0; l < scale; ++l) {
                    scaledMatrix.field[i * scale + k][j * scale + l] = field[i][j];
                }
            }
        }
    }
    return scaledMatrix;
}

void Matrix::fillMatrixByVector(const vector<vector<char>> &vec) {
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec[i].size(); ++j) {
            field[i][j] = vec[i][j];
        }
    }
}

char **Matrix::getField() {
    return field;
}

Matrix Matrix::shiftMatrix(bool changeSize, int leftBias, int topBias) {
    int shiftedMatrixHeight, shiftedMatrixWidth;
    if (changeSize) {
        shiftedMatrixHeight = height + topBias;
        shiftedMatrixWidth = width + leftBias;
    }
    else {
        shiftedMatrixHeight = height;
        shiftedMatrixWidth = width;
    }
    Matrix shiftedMatrix(shiftedMatrixHeight, shiftedMatrixWidth, emptyPixel, emptyPixel);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int y = i + topBias, x = j + leftBias;
            if (y >= 0 && y < shiftedMatrixHeight && x >= 0 && x < shiftedMatrixWidth) {
                shiftedMatrix.field[y][x] = field[i][j];
            }
        }
    }
    return shiftedMatrix;
}

Matrix Matrix::overlayMatrix(const Matrix &overlaybleMatrix, bool overlaybleMatrixTransparency) {
    Matrix overlayedMatrix(height, width, emptyPixel);
    overlayedMatrix.copyField(*this);

    for (int i = 0; i < overlaybleMatrix.height; ++i) {
        for (int j = 0; j < overlaybleMatrix.width; ++j) {
            if (overlaybleMatrixTransparency && overlaybleMatrix.field[i][j] == overlaybleMatrix.emptyPixel) {
                continue;
            }
            overlayedMatrix.field[i][j] = overlaybleMatrix.field[i][j];
        }
    }
    return overlayedMatrix;
}

Matrix Matrix::roundMatrix(unsigned int radius) {
    Matrix roundedMatrix(height, width, emptyPixel);
    roundedMatrix.copyField(*this);

    int x, y;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            x = i - width / 2;
            y = j - height / 2;
            if (x * x + y * y > radius * radius) {
                roundedMatrix.field[i][j] = emptyPixel;
            }
        }
    }
    return roundedMatrix;
}

unsigned int Matrix::getHeight() {
    return height;
}

unsigned int Matrix::getWidth() {
    return width;
}
