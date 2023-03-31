#include "geneticAlgorithm.h"

Species::Species(bool fillGens) {
    gens = new double[GEN_LENGTH];
    if (fillGens) {
        for (int i = 0; i < GEN_LENGTH; ++i) {
            gens[i] = getRandDouble(GEN_MIN_START_VALUE, GEN_MAX_START_VALUE, 1);
        }
    }
}
Species::Species(const double gen[GEN_LENGTH]) {
    gens = new double[GEN_LENGTH];
    for (int i = 0; i < GEN_LENGTH; ++i) {
        gens[i] = gen[i];
    }
}
ostream &operator<<(ostream &out, Species &species) {
    for (int i = 0; i < GEN_LENGTH; ++i) {
        cout << species.gens[i] << ", ";
    }
    cout << endl;
    return out;
}

Species* getClone(const Species& species) {
    return new Species(species.gens);
}

double fitness(Species species) {
    array<int, 4> result = AI(false, species.gens, 0);
    // result = {score, totalClearedLines, numOfTetris, numOfClearingLines}
    if (result[3] != 0) {
        return result[1] * (double)result[2] / result[3];
    }
    return 0;
}

void getFitnessFromTo(pair<Species, double> population[POPULATION_SIZE], int start, int end) {
    for (int i = start; i < end; ++i) {
        population[i].second = fitness(population[i].first);
    }
}

void getPopulationFitness(pair<Species, double> population[POPULATION_SIZE]) {
    vector <thread> thVec;
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.emplace_back(getFitnessFromTo, ref(population), i * POPULATION_SIZE / NUM_OF_THREADS, (i + 1) * (POPULATION_SIZE / NUM_OF_THREADS));
    }
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.at(i).join();
    }
}

void fillPopulation(pair<Species, double> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].first = Species();
    }
    getPopulationFitness(population);
}

int getBestSpecies(const pair<Species, double> population[POPULATION_SIZE], int s1, int s2, int s3) {
    if (population[s1].second >= population[s2].second && population[s1].second >= population[s3].second) {
        return s1;
    }
    if (population[s2].second >= population[s1].second && population[s2].second >= population[s3].second) {
        return s2;
    }
    return s3;
}

void selectionPopulation(pair<Species, double> population[POPULATION_SIZE]) {
    Species selected[POPULATION_SIZE];
    Species *clone;
    int s1, s2, s3;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        do {
            s1 = getRandInt(0, POPULATION_SIZE);
            s2 = getRandInt(0, POPULATION_SIZE);
            s3 = getRandInt(0, POPULATION_SIZE);
        } while (equals(3, s1, s2, s2));
        clone = getClone(population[getBestSpecies(population, s1, s2, s3)].first);
        selected[i] = *clone;
    }
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].first = selected[i];
    }
}

void crossover(Species &s1, Species &s2) {
    if (!getTrueWithProbability(CROSSOVER_PROBABILITY)) return;
    int genCatPos = getRandInt(0, GEN_LENGTH);
    int temp;
    for (int i = genCatPos; i < GEN_LENGTH; ++i) {
        temp = s1.gens[i];
        s1.gens[i] = s2.gens[i];
        s2.gens[i] = temp;
    }
}

void crossoverPopulation(pair<Species, double> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
        crossover(population[2 * i].first, population[2 * i + 1].first);
    }
}

void genMutation(pair<Species, double> population[POPULATION_SIZE], int arrPos, int genPos) {
    if (getRandInt(0, 2)) {
        population[arrPos].first.gens[genPos] += GEN_MUTATION_STEP;
    }
    else {
        population[arrPos].first.gens[genPos] -= GEN_MUTATION_STEP;
    }
    roundToZero(population[arrPos].first.gens[genPos]);
}

void mutationPopulation(pair<Species, double> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < GEN_LENGTH; ++j) {
            if (getTrueWithProbability(MUTATION_PROBABILITY)) {
                genMutation(population, i, j);
            }
        }
    }
}

double getAvgPopulationFitness(const pair<Species, double> population[POPULATION_SIZE]) {
    double avgFitness = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        avgFitness += population[i].second;
    }
    return avgFitness / POPULATION_SIZE;
}

int getBestSpeciesPos(pair<Species, double> population[POPULATION_SIZE]) {
    double maxFitness = 0;
    int posOfBestSpecies = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        if (population[i].second > maxFitness) {
            maxFitness = population[i].second;
            posOfBestSpecies = i;
        }
    }
    return posOfBestSpecies;
}

void swapInArr(pair<Species, double> arr[POPULATION_SIZE], int pos1, int pos2) {
    pair<Species, double> temp = arr[pos1];
    arr[pos1] = arr[pos2];
    arr[pos2] = temp;
}

void sortArrByFitness(pair<Species, double> population[POPULATION_SIZE]) {
    int posOfMax;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        posOfMax = i;
        for (int j = i; j < POPULATION_SIZE; ++j) {
            if (population[j].second > population[posOfMax].second) {
                posOfMax = j;
            }
        }
        swapInArr(population, i, posOfMax);
    }
}

void trainAI() {
    initRand();
    pair<Species, double> population[POPULATION_SIZE];
    int bestSpeciesPos, genNum = 0;

    auto totalStartTime = chrono::high_resolution_clock::now();
    auto generationStartTime = chrono::high_resolution_clock::now();
    auto generationEndTime = chrono::high_resolution_clock::now();

    if (USE_LAST_POPULATION) {
        genNum = getPopulationFromFile(population);
    }
    else {
        fillPopulation(population);
    }

    for (int i = 0; i < MAX_GENERATIONS; ++i) {
        selectionPopulation(population);

        bestSpeciesPos = getBestSpeciesPos(population);

        cout << "Generation: " << genNum + i + 1 << endl;

        cout << "Best fitness: " << population[bestSpeciesPos].second << endl;
        cout << "Avg fitness: " << getAvgPopulationFitness(population) << endl;

        cout << "Gens of best species: " << population[bestSpeciesPos].first.gens[0];

        for (int j = 1; j < GEN_LENGTH; ++j) {
            cout << ", " << population[bestSpeciesPos].first.gens[j];
        }

        generationEndTime = chrono::high_resolution_clock::now();
        chrono::duration<float> generationDuration = generationEndTime - generationStartTime;
        chrono::duration<float> totalDuration = generationEndTime - totalStartTime;

        cout << endl << "Generation time: " << generationDuration.count() << endl;
        cout << "Total time: " << totalDuration.count() << endl << endl;

        if (SAVE_POPULATION_TO_FILE) {
            savePopulation(population, genNum + i + 1);
        }

        generationStartTime = chrono::high_resolution_clock::now();

        crossoverPopulation(population);
        mutationPopulation(population);
        getPopulationFitness(population);
    }
}

void savePopulation(pair<Species, double> population[POPULATION_SIZE], int genNum) {
    ofstream fout;
    fout.open(SAVED_POPULATION_FILE_PATH);

    sortArrByFitness(population);

    if(fout.is_open()) {
        fout << POPULATION_SIZE << " ";
        fout << genNum << " ";
        fout << GEN_LENGTH << endl;
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            fout << population[i].second << " ";
            fout << population[i].first.gens[0];
            for (int j = 1; j < GEN_LENGTH; ++j) {
                fout << " " << population[i].first.gens[j];
            }
            fout << endl;
        }
    }
    else {
        cout << "ERROR of saving population" << endl;
    }
    fout.close();
}

int getPopulationFromFile(pair<Species, double> population[POPULATION_SIZE]) {
    ifstream fin;
    fin.open(SAVED_POPULATION_FILE_PATH);

    int lastPopulationSize, lastNumOfGens, genNum = 0;

    if (fin.is_open()) {
        fin >> lastPopulationSize;
        fin >> genNum;
        fin >> lastNumOfGens;
        if (lastPopulationSize != POPULATION_SIZE ||
            lastNumOfGens != GEN_LENGTH) {
            cout << "Different population size or num of gens" << endl;
            cout << "Random population generation" << endl;
            fillPopulation(population);
        }
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            population[i].first = Species(false);
            fin >> population[i].second;
            for (int j = 0; j < GEN_LENGTH; ++j) {
                fin >> population[i].first.gens[j];
            }
        }
    }
    else {
        cout << "The file was not opened" << endl;
        fillPopulation(population);
    }
    fin.close();
    return genNum;
}