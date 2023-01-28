cmake_minimum_required(VERSION 3.24)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

add_executable(Tetris main.cpp headers/Piece.h headers/constants.h libraries/functions.cpp headers/functions.h libraries/Piece.cpp headers/config.h libraries/tetrisFunctions.cpp headers/tetrisFunctions.h libraries/geneticAlgorithm.cpp headers/geneticAlgorithm.h libraries/AI.cpp headers/AI.h headers/geneticAlgorithmConfig.h libraries/myQueue.cpp headers/myQueue.h)
