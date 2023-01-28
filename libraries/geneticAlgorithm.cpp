#include "../headers/geneticAlgorithm.h"

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
void Species::printGens() const {
    for (int i = 0; i < GEN_LENGTH; ++i) {
        cout << gens[i] << ", ";
    }
    cout << endl;
}

Species* getClone(const Species& species) {
    return new Species(species.gens);
}

int fitness(Species species) {
    array<int, 4> result = AI(false, species.gens);
    if (result[3] != 0) {
        return (int)(result[0] * (1 + TETRIS_COOLNESS * (double)result[2] / result[3]));
    }
    return result[0];
}

void getFitnessFromTo(pair<Species, int> population[POPULATION_SIZE], int start, int end) {
    for (int i = start; i < end; ++i) {
        population[i].second = fitness(population[i].first);
    }
}

void getPopulationFitness(pair<Species, int> population[POPULATION_SIZE]) {
    vector <thread> thVec;
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.emplace_back(getFitnessFromTo, ref(population), i * POPULATION_SIZE / NUM_OF_THREADS, (i + 1) * (POPULATION_SIZE / NUM_OF_THREADS));
    }
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        thVec.at(i).join();
    }
}

void getPopulationFromFile(pair<Species, int> population[POPULATION_SIZE]) {
    ifstream fin;
    fin.open(SAVED_POPULATION_FILE_PATH);

    int lastPopulationSize, lastNumOfGens;

    if (fin.is_open()) {
        fin >> lastPopulationSize;
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
}

void fillPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].first = Species();
    }
    getPopulationFitness(population);
}

int getBestSpecies(const pair<Species, int> population[POPULATION_SIZE], int s1, int s2, int s3) {
    if (population[s1].second >= population[s2].second && population[s1].second >= population[s3].second) {
        return s1;
    }
    if (population[s2].second >= population[s1].second && population[s2].second >= population[s3].second) {
        return s2;
    }
    return s3;
}

void selectionPopulation(pair<Species, int> population[POPULATION_SIZE]) {
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

void crossoverPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
        crossover(population[2 * i].first, population[2 * i + 1].first);
    }
}

void genMutation(pair<Species, int> population[POPULATION_SIZE], int arrPos, int genPos) {
    if (getRandInt(0, 2)) {
        population[arrPos].first.gens[genPos] += GEN_MUTATION_STEP;
    }
    else {
        population[arrPos].first.gens[genPos] -= GEN_MUTATION_STEP;
    }
}

void mutationPopulation(pair<Species, int> population[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < GEN_LENGTH; ++j) {
            if (getTrueWithProbability(MUTATION_PROBABILITY)) {
                genMutation(population, i, j);
            }
        }
    }
}

float getAvgPopulationFitness(const pair<Species, int> population[POPULATION_SIZE]) {
    int avgFitness = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        avgFitness += population[i].second;
    }
    return (float)avgFitness / POPULATION_SIZE;
}

int getBestSpeciesPos(pair<Species, int> population[POPULATION_SIZE]) {
    int maxFitness = 0, posOfBestSpecies = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        if (population[i].second > maxFitness) {
            maxFitness = population[i].second;
            posOfBestSpecies = i;
        }
    }
    return posOfBestSpecies;
}

void swapInArr(pair<Species, int> arr[POPULATION_SIZE], int pos1, int pos2) {
    pair<Species, int> temp = arr[pos1];
    arr[pos1] = arr[pos2];
    arr[pos2] = temp;
}

void sortArrByFitness(pair<Species, int> population[POPULATION_SIZE]) {
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
    pair<Species, int> population[POPULATION_SIZE];
    int bestSpeciesPos;

    if (USE_LAST_POPULATION) {
        getPopulationFromFile(population);
    }
    else {
        fillPopulation(population);
    }

    auto totalStartTime = chrono::high_resolution_clock::now();
    auto generationStartTime = chrono::high_resolution_clock::now();
    auto generationEndTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX_GENERATIONS; ++i) {
        generationStartTime = chrono::high_resolution_clock::now();
        selectionPopulation(population);
        crossoverPopulation(population);
        mutationPopulation(population);

        getPopulationFitness(population);

        if (SAVE_POPULATION_TO_FILE) {
            savePopulation(population);
        }

        bestSpeciesPos = getBestSpeciesPos(population);

        cout << "Generation: " << i + 1 << endl;

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
    }
}

void savePopulation(pair<Species, int> population[POPULATION_SIZE]) {
    ofstream fout;
    fout.open(SAVED_POPULATION_FILE_PATH);

    if(fout.is_open()) {
        fout << POPULATION_SIZE << " ";
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