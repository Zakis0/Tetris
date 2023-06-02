#pragma once
#ifndef TETRIS_GENETIC_ALGORITHM_CONFIG_H
#define TETRIS_GENETIC_ALGORITHM_CONFIG_H

#define GEN_LENGTH NUM_OF_SCORE_PARAMS
#define GEN_MIN_START_VALUE 0
#define GEN_MAX_START_VALUE 10.0
#define GEN_MUTATION_STEP 0.25 // step in gen mutation

#define POPULATION_SIZE 200 // number of species in population
#define CROSSOVER_PROBABILITY 95 // probability of crossover
#define MUTATION_PROBABILITY 1 // probability of gen mutation
#define MAX_GENERATIONS 1000000 // max number of generations

#define NUM_OF_THREADS 200

#define USE_LAST_POPULATION true

#define SAVE_POPULATION_TO_FILE false

#define SAVED_POPULATION_FILE_PATH "../lastPopulation.txt"

#define TIME_ROUND_DIGITS 2

#endif // TETRIS_GENETIC_ALGORITHM_CONFIG_H
