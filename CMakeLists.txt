cmake_minimum_required(VERSION 3.24)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

add_executable(Tetris main.cpp libraries/Piece/Piece.h Config/constants.h libraries/Functions/functions.cpp libraries/Functions/functions.h libraries/Piece/Piece.cpp Config/config.h libraries/TetrisFunctions/tetrisFunctions.cpp libraries/TetrisFunctions/tetrisFunctions.h libraries/GeneticAlgorithm/geneticAlgorithm.cpp libraries/GeneticAlgorithm/geneticAlgorithm.h libraries/AI/AI.cpp libraries/AI/AI.h Config/geneticAlgorithmConfig.h libraries/MyQueue/myQueue.cpp libraries/MyQueue/myQueue.h main.cpp main.cpp libraries/Piece/Piece.h libraries/Matrix/matrix.h libraries/Matrix/matrix.cpp libraries/Error/error.cpp libraries/Useless/Useless.h)
