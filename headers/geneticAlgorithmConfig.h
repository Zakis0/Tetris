#pragma once
#ifndef TETRIS_GENETIC_ALGORITHM_CONFIG_H
#define TETRIS_GENETIC_ALGORITHM_CONFIG_H


#define GEN_LENGTH NUM_OF_SCORE_PARAMS
#define GEN_MIN_START_VALUE 0
#define GEN_MAX_START_VALUE 10.0
#define GEN_MUTATION_STEP 0.25 // шаг в мутации гена
#define TETRIS_COOLNESS 10 // множитель для процента тетрисов

#define POPULATION_SIZE 2 // количество индивидуумов в популяции
#define CROSSOVER_PROBABILITY 70 // вероятность скрещивания
#define MUTATION_PROBABILITY 15 // вероятность мутации гена индивидуума
#define MAX_GENERATIONS 2 // максимальное количество поколений

#define NUM_OF_THREADS 20

#define USE_LAST_POPULATION true

#define SAVE_POPULATION_TO_FILE true

#define SAVED_POPULATION_FILE_PATH "../lastPopulation.txt"

#endif // TETRIS_GENETIC_ALGORITHM_CONFIG_H
