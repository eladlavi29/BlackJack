#pragma once
#include <vector>
#include "DM.h"

using namespace std;

class Generation
{
private:
	DM** generation;
	int generationNumber;

	int bestChromoInd;
	float avgWinningExp; 
	float avgImprovement;
	float bestImprovement;

	//Data tracking
	vector<float> avgWinningExpData;
	vector<float> bestWinningExpData;
	vector<float> avgImprovementData;
	vector<float> bestImprovementData;
	
	void erasePreGeneration(int startingInd);

	void rankingProcess();
	void elitism(); //More about it in the function

	int selectionProcess();
	void breedingProcess();

	void updateData();

public:

	Generation();

	void lifetime();
	bool stopEvoloution();

	void printData();

	void check();
};

