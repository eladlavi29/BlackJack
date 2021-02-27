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
	avgImprovement = avgWinningExp - preAvgWinningExp;
	//Don't updata in the first time when the vector's empty
	if (!bestWinningExpData.empty()) {
		float preBestWinningExp = bestWinningExpData[bestWinningExpData.size() - 1];
		bestImprovement = generation[bestChromoInd]->getWinningExpectation() - preBestWinningExp;
	}

	updateData();

	delete game;
}

int elitistsAmout;
bool comp(DM* dm1, DM* dm2) {
	return (dm1->getFitness() < dm2->getFitness());
}
void Generation::elitism() {
	//Sort elitism precentage of the generation
	//This functions helps to keep only the best chromo alive
	//I assume it's ideal not to use it, so for now I'll only make sure I save the best chromo
	if (elitisimPrecRate != 0) {
		swap(generation[0], generation[bestChromoInd]);
		bestChromoInd = 0;
	}

	/*if (elitisimPrecRate != 0) {
		elitistsAmout = generationSize * elitisimPrecRate;
		nth_element(generation, generation + elitistsAmout - 1, generation + generationSize, comp);
	}*/
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

int notImproving = 0;
bool Generation::stopEvoloution() {
	if (bestImprovement < 0.001) ++notImproving;
	else notImproving = 0;

	if (notImproving == 3) return true;

	return false;
}

void Generation::check() {
	Game game(0);
	DM* parent1 = new DM; parent1->generate();
	game.rankDM(*parent1);

	DM* parent2 = new DM; parent2->generate();
	game.rankDM(*parent2); 

	int count0 = 0, count1 = 0, count2 = 0;
	int strong = parent1->getFitness(), weak = parent2->getFitness(); 
	if (weak > strong) swap(weak, strong);
	int curFitness;
	for (int i = 0; i < generationSize; ++i) {
		generation[i] = parent1->crossover(parent2);
		game.rankDM(*generation[i]);
		curFitness = generation[i]->getFitness();
		if (curFitness < weak) ++count0;
		else if (curFitness < strong) ++count1;
		else ++count2;
	}
	cout << count1 << "chromoes are better than 1 father and " << count2 << "are better then both of them \n";
}
