#include "Generation.h"
#include "Game.h"
#include "MachineLearning.h"
#include "practicalFuncs.h"

using namespace std;

Generation::Generation() : generationNumber(2), avgWinningExp(0), avgImprovement(0), bestImprovement(0), bestChromoInd(-1){
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
	cout << "Avg Winning Exp :  " << avgWinningExp << " \n";
	cout << "The avg improvement from the prev generation :  " << avgImprovement << " \n\n";
	cout << "The best chromo winning exp :  " << generation[bestChromoInd]->getWinningExpectation() << " \n\n";
	printData();
}

void Generation::updateData() {
	avgWinningExpData.push_back(avgWinningExp);
	bestWinningExpData.push_back(generation[bestChromoInd]->getWinningExpectation());
	avgImprovementData.push_back(avgImprovement);
	bestImprovementData.push_back(bestImprovement);
}
void Generation::printData() {
	cout << "The machine learning's data :  \n\n";

	cout << "avgWinningExp :  \n";
	for (auto x : avgWinningExpData)
		cout << x << " ";

	cout << "\nbestWinningExp :  \n";
	for (auto x : bestWinningExpData)
		cout << x << " ";
	
	cout << "\navgImprovement :  \n";
	for (auto x : avgImprovementData)
		cout << x << " ";

	cout << "\nbestImprovement :  \n";
	for (auto x : bestImprovementData)
		cout << x << " ";
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
	cout << "Avg Winning Exp :  " << avgWinningExp << " \n";
	cout << "The avg improvement from the prev generation :  " << avgImprovement << " \n\n";
	cout << "The best chromo winning exp :  " << generation[bestChromoInd]->getWinningExpectation() << " \n\n";
	printData();
	++generationNumber;
}

void Generation::rankingProcess() {
	Game* game = new Game(0);

	float preAvgWinningExp = avgWinningExp; avgWinningExp = 0;
	int bestFitness = INT_MIN; int currentFitness;
	for (int i = 0; i < generationSize; ++i) {
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
	avgImprovement = avgWinningExp - preAvgWinningExp;
	//Don't updata in the first time when the vector's empty
	if (!bestWinningExpData.empty()) {
		float preBestWinningExp = bestWinningExpData[bestWinningExpData.size() - 1];
		bestImprovement = generation[bestChromoInd]->getWinningExpectation() - preBestWinningExp;
	}

	elitism();
	updateData();

	delete game;
}

void Generation::elitism() {
	//Sort elitism precentage of the generation
	//This functions helps to keep only the best chromo alive
	//I assume it's ideal not to use it, so for now I'll only make sure I save the best chromo
	swap(generation[0], generation[bestChromoInd]);
	bestChromoInd = 0;
}

void Generation::breedingProcess() {

	//Fathers pick process
	DM** fathers;
	fathers = new DM* [tourney + 1];
	for (int i = 0; i < tourney + 1; ++i) {
		fathers[i] = new DM();
	}
	//put the best chromo in the 0 cell for convenience
	fathers[0]->copy(generation[bestChromoInd]);

	int rand;
	for (int i = 1; i <= tourney; ++i) {
		rand = (int)(random() % (generationSize - 1) + 1);
		fathers[i]->copy(generation[rand]);
	}

	erasePreGeneration(survivorsAmount);

	//Generate the new generation
	int first; int last = survivorsAmount; //Skip the ones that aren't including acc to the crossover Rate
	for (int i = 1; i <= tourney; ++i) {
		first = last;
		if (i == tourney) last = generationSize;
		else last = fatherBreedingSize + last;

		for (int j = first; j < last; ++j) {
			generation[j] = fathers[0]->crossover(fathers[i]);
		}
	}

	delete []fathers;
}

int notImproving = 0;
bool Generation::stopEvoloution() {
	if (bestImprovement < 0.0001) ++notImproving;
	else notImproving = 0;

	if (notImproving == 3) return true;

	return false;
}

