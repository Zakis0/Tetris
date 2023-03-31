#pragma once
#ifndef TETRIS_GENETIC_ALGORITHM_H
#define TETRIS_GENETIC_ALGORITHM_H

#include "../../Config/constants.h"
#include "../Functions/functions.h"
#include "../AI/AI.h"
#include "../../Config/geneticAlgorithmConfig.h"

#include <iostream>
#include <fstream>

using namespace std;

class Species {
public:
    double *gens;

    explicit Species(bool fillGens = true);
    explicit Species(const double gen[GEN_LENGTH]);

    friend ostream& operator<<(ostream& out, Species& species);
};
Species* getClone(const Species& species);
double fitness(Species species);
void getFitnessFromTo(pair<Species, double> population[POPULATION_SIZE], int start = 0, int end = POPULATION_SIZE);
void getPopulationFitness(pair<Species, double> population[POPULATION_SIZE]);
void fillPopulation(pair<Species, double> population[POPULATION_SIZE]);
int getBestSpecies(const pair<Species, double> population[POPULATION_SIZE], int s1, int s2, int s3);
void selectionPopulation(pair<Species, double> population[POPULATION_SIZE]);
void crossover(Species &s1, Species &s2);
void crossoverPopulation(pair<Species, double> population[POPULATION_SIZE]);
void genMutation(pair<Species, double> population[POPULATION_SIZE], int arrPos, int genPos);
void mutationPopulation(pair<Species, double> population[POPULATION_SIZE]);
double getAvgPopulationFitness(const pair<Species, double> population[POPULATION_SIZE]);
int getBestSpeciesPos(pair<Species, double> population[POPULATION_SIZE]);
void swapInArr(pair<Species, double> arr[POPULATION_SIZE], int pos1, int pos2);
void sortArrByFitness(pair<Species, double> population[POPULATION_SIZE]);
void savePopulation(pair<Species, double> population[POPULATION_SIZE], int genNum);
int getPopulationFromFile(pair<Species, double> population[POPULATION_SIZE]);
void trainAI();

#endif //TETRIS_GENETIC_ALGORITHM_H
