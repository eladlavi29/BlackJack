#include "Generation.h"
#include "Game.h"
#include "MachineLearning.h"
#include "practicalFuncs.h"
#include <algorithm>

using namespace std;

Generation::Generation() : generationNumber(2), avgWinningExp(0), bestChromoInd(-1){
	cout << "Generating the first generation \n";
	generation = new DM*[generationSize];
	for (int i = 0; i < generationSize; ++i) {
		generation[i] = new DM;
		generation[i]->generate();
	}
	
	rankingProcess();

	system("cls"); //Clean the outputs
	//print the generation stats
	cout << "First generation stats: \n";
	cout << "The best chromo winning exp :  " << generation[bestChromoInd]->getWinningExpectation() << " \n\n";
	cout << "Avg Winning Exp :  " << avgWinningExp << " \n";
	printData();
}

void Generation::updateData() {
	avgWinningExpData.push_back(avgWinningExp);
	bestWinningExpData.push_back(generation[bestChromoInd]->getWinningExpectation());
}
void Generation::printData() {
	cout << "The machine learning's data : \n";

	cout << "\nbestWinningExp :  \n";
	for (auto x : bestWinningExpData)
		cout << x << " ";
	cout << "\n\n";

	cout << "avgWinningExp :  \n";
	for (auto x : avgWinningExpData)
		cout << x << " ";
	cout << "\n\n";

	bestChromo()->printData();
	cout << "\n";
}

void Generation::erasePreGeneration(int startingInd) {
	for (int i = startingInd; i < generationSize; ++i) {
		delete generation[i];
	}
}

void Generation::lifetime() {
	cout << "Generation number " << generationNumber << " is on progress \n";

	breedingProcess();

	rankingProcess();

	system("cls"); //Clean the outputs
	//print the generation stats
	cout << "Generation number " << generationNumber << " stats: \n";
	cout << "The best chromo winning exp :  " << generation[bestChromoInd]->getWinningExpectation() << " \n\n";
	cout << "Avg Winning Exp :  " << avgWinningExp << " \n";
	printData();
	++generationNumber;
}

void Generation::rankingProcess() {
	Game* game = new Game(0);

	float preAvgWinningExp = avgWinningExp; avgWinningExp = 0;
	int bestFitness = INT_MIN; int currentFitness;
	for (int i = 0; i < generationSize; ++i) {
		//if the chromo haven't been ranked, rank it
		if (!generation[i]->getFitness() != 0) 
			game->rankDM(*generation[i]);

		currentFitness = generation[i]->getFitness();
		if (currentFitness > bestFitness) {
			bestFitness = currentFitness;
			bestChromoInd = i;
		}

		avgWinningExp += generation[i]->getWinningExpectation();
	}
	
	//Update the relevant data (other then the generation itself)
	avgWinningExp = avgWinningExp / generationSize;

	updateData();

	delete game;
}

int elitistsAmount;
bool comp(DM* dm1, DM* dm2) {
	return (dm1->getFitness() > dm2->getFitness());
}
void Generation::elitism() {
	//Sort elitism precentage of the generation
	if (elitisimPrecRate != 0) {
		elitistsAmount = (int)((float)generationSize * elitisimPrecRate / 100);
		nth_element(generation, generation + elitistsAmount - 1, generation + generationSize, comp);
	}
}

vector<int> selectionOptions, selected;
int Generation::selectionProcess() {
	if (selectionOptions.empty()) {
		for (int i = 0; i < generationSize; ++i) selectionOptions.push_back(i);
	}

	int rand, chosen = -1, bestFitness = INT_MIN, curFitness;
	for (int i = 0; i < tourney; ++i) {
		rand = (int)random() % selectionOptions.size();
		selected.push_back(rand);
		selectionOptions.erase(selectionOptions.begin() + rand);

		curFitness = generation[rand]->getFitness();
		if (curFitness > bestFitness) {
			bestFitness = curFitness;
			chosen = rand;
		}
	}

	for (auto x : selected) selectionOptions.push_back(x);
	//deleteVector
	//vector<int>().swap(selected);
	selected.clear(); //selected.shrink_to_fit();

	return chosen;
}

void Generation::breedingProcess() {

	elitism();

	DM** newGeneration = new DM* [generationSize];
	//Generate the new generation's survivors
	for (int i = 0; i < survivorsAmount; ++i) {
		newGeneration[i] = generation[i];
	}

	//Generate the new generation's offsprings
	int selection1, selection2;
	int save;
	for (int i = survivorsAmount; i < generationSize; ++i) {
		//Select 2 fathers
		selection1 = selectionProcess();
		//Make sure selection1 doesn't repeat
		save = generation[selection1]->getFitness();
		generation[selection1]->setFitness(INT_MIN); //It will always lose selection
		selection2 = selectionProcess();
		generation[selection1]->setFitness(save);
		newGeneration[i] = generation[selection1]->crossover(generation[selection2]);
	}

	erasePreGeneration(survivorsAmount);
	generation = newGeneration;
}

int notImproving = -1; float avgImprovement;
bool Generation::stopEvoloution() {
	avgImprovement = avgWinningExpData[avgWinningExpData.size() - 1] - avgWinningExpData[avgWinningExpData.size() - 2];
	if (avgImprovement < 0.001) ++notImproving;
	else notImproving = 0;

	if (notImproving == 3 || generationNumber == 48) return true;

	return false;
}

DM* Generation::bestChromo() {
	return generation[bestChromoInd];
}
