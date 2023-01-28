#pragma once
#ifndef TETRIS_GENETIC_ALGORITHM_H
#define TETRIS_GENETIC_ALGORITHM_H

#include "constants.h"
#include "functions.h"
#include "AI.h"
#include "geneticAlgorithmConfig.h"

#include <iostream>
#include <fstream>

using namespace std;

class Species {
public:
    double *gens;

    explicit Species(bool fillGens = true);
    explicit Species(const double gen[GEN_LENGTH]);
    void printGens() const;
};
Species* getClone(const Species& species);
int fitness(Species species);
void getFitnessFromTo(pair<Species, int> population[POPULATION_SIZE], int start = 0, int end = POPULATION_SIZE);
void getPopulationFitness(pair<Species, int> population[POPULATION_SIZE]);
void fillPopulation(pair<Species, int> population[POPULATION_SIZE]);
int getBestSpecies(const pair<Species, int> population[POPULATION_SIZE], int s1, int s2, int s3);
void selectionPopulation(pair<Species, int> population[POPULATION_SIZE]);
void crossover(Species &s1, Species &s2);
void crossoverPopulation(pair<Species, int> population[POPULATION_SIZE]);
void genMutation(pair<Species, int> population[POPULATION_SIZE], int arrPos, int genPos);
void mutationPopulation(pair<Species, int> population[POPULATION_SIZE]);
float getAvgPopulationFitness(const pair<Species, int> population[POPULATION_SIZE]);
int getBestSpeciesPos(pair<Species, int> population[POPULATION_SIZE]);
void swapInArr(pair<Species, int> arr[POPULATION_SIZE], int pos1, int pos2);
void sortArrByFitness(pair<Species, int> population[POPULATION_SIZE]);
void savePopulation(pair<Species, int> population[POPULATION_SIZE]);
void getPopulationFromFile(pair<Species, int> population[POPULATION_SIZE]);
void trainAI();

#endif //TETRIS_GENETIC_ALGORITHM_H
