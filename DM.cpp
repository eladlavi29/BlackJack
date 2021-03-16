#include "DM.h"
#include "MachineLearning.h"
#include "practicalFuncs.h"
#include <iostream>

using namespace std;

int DM::getFitness() {
	return fitness;
}
void DM::setFitness(int fitness) {
	this->fitness = fitness;
}
char DM::getMove(int a, int b, int c) {
	
	return dm[a][b][c];
}
void DM::setMove(int a, int b, int c, char move) {
	dm[a][b][c] = move;
}

#define cf (float)
//Ideal is -0.006
double DM::getWinningExpectation() {
	return cf fitness / cf betDM / cf timesWithMinCoeffcient;
}

void DM::build(int ind, const int& num1, const int& num2) {
	dm[ind] = new char* [num1];
	for (int i = 0; i < num1; ++i) {
		dm[ind][i] = new char[num2];
	}
}
const int dealer = 11 - 1;//2 to A
const int hard = 20 - 3; //4 to 20
const int soft = 9; //A to 9
const int pairs = 11 - 1; //2 to A
DM::DM() :fitness(0) {
	//Hard hands' table
	build(0, dealer, hard);
	//Soft hands' table;
	build(1, dealer, soft);
	//Pair hands' table
	build(2, dealer, pairs);
}

void DM::copyTable(int ind, const int& num1, const int& num2, DM* copy) {
	for (int i = 0; i < num1; ++i) {
		for (int j = 0; j < num2; ++j) {
			dm[ind][i][j] = copy->getMove(ind, i, j);
		}
	}
}
void DM::copy(DM* copy) {
	//Hard hands' table
	copyTable(0, dealer, hard, copy);
	//Soft hands' table;
	copyTable(1, dealer, soft, copy);
	//Pair hands' table
	copyTable(2, dealer, pairs, copy);

	this->fitness = copy->getFitness();
}

void DM::printTable(int ind, const int& num1, const int& num2) {
	for (int j = num2 - 1; j >= 0; --j) {
		for (int i = num1 - 1; i >= 0; --i) {
			cout << dm[ind][i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void DM::printData() {
	printTable(0, dealer, hard);
	printTable(1, dealer, soft);
	printTable(2, dealer, pairs);
	cout << "\n";
}

void DM::generate() {
	vector<char> options = { 's', 'h', 'd' };
	char rand;

	//Hard hands' table
	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < hard; ++j) {
			rand = options[(int)(random() % options.size())];
			dm[0][i][j] = rand;
		}
	}

	//Soft hands' table;
	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < soft; ++j) {
			rand = options[(int)(random() % options.size())];
			dm[1][i][j] = rand;
		}
	}

	//Pair hands' table
	options.clear();
	options.push_back('p');
	options.push_back('n');

	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < pairs; ++j) {
			rand = options[(int)(random() % options.size())];
			dm[2][i][j] = rand;
		}
	} 
}

bool DM::decideSplit(int sum, int dealerReaveled, bool hasAce) {
	dealerReaveled -= 2;

	if (hasAce) {
		if (dm[2][dealerReaveled][9] == 'p') {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (dm[2][dealerReaveled][sum / 2 - 2] == 'p') return true;
		else {
			return false;
		}
	}
}

inline char fix(bool firstMove, char ch) {
	if (ch == 'd' && !firstMove) ch = 'h';

	return ch;
}
char DM::decideReg(int sum, int dealerReaveled, bool firstMove, bool hasAce) {
	dealerReaveled -= 2; char ch;
	
	if (hasAce) {
		return fix(firstMove, dm[1][dealerReaveled][sum - 11 - 1]);
	}

	return fix(firstMove, dm[0][dealerReaveled][sum - 4]);
}

vector<char> options; vector<char> optionsForMutation;
bool mutate() {
	return (random() % 100 < mutationPrecRate);
}
char mutation(char d1, char d2) {

	optionsForMutation = options;
	optionsForMutation.erase(remove(optionsForMutation.begin(), optionsForMutation.end(), d1), optionsForMutation.end());
	optionsForMutation.erase(remove(optionsForMutation.begin(), optionsForMutation.end(), d2), optionsForMutation.end());

	if (optionsForMutation.empty())
		return options[random() % options.size()];
	return optionsForMutation[random() % optionsForMutation.size()];
}
char ch;
void DM::crossoverTable(int a, int b, int c, DM* chromo, DM* offspring) {
	for (int i = 0; i < b; ++i) {
		for (int j = 0; j < c; ++j) {
			if (mutate())
				ch = mutation(getMove(a, i, j), chromo->getMove(a, i, j));
			else ch = dependentPick(getMove(a, i, j), getFitness(), chromo->getMove(a, i, j), chromo->getFitness());
			offspring->setMove(a, i, j, ch);
		}
	}
}
DM* DM::crossover(DM* chromo) {
	DM* offspring = new DM;

	options = { 'd', 'h', 's' };
	crossoverTable(0, dealer, hard, chromo, offspring);
	crossoverTable(1, dealer, soft, chromo, offspring);

	options = { 'n', 'p'};
	crossoverTable(2, dealer, pairs, chromo, offspring);

	return offspring;
}

float fitsFather1, fitsFather2;
void DM::checkTableAlikness(int a, int b, int c, DM* father1, DM* father2) {
	for (int i = 0; i < b; ++i) {
		for (int j = 0; j < c; ++j) {
			if (getMove(a, i, j) == father1->getMove(a, i, j)) ++fitsFather1;
			if (getMove(a, i, j) == father2->getMove(a, i, j)) ++fitsFather2;
		}
	}
}
float DM::checkOffspringAlikeness(DM* father1, DM* father2) {
	fitsFather1 = 0; fitsFather2 = 0;

	checkTableAlikness(0, dealer, hard, father1, father2);
	checkTableAlikness(1, dealer, soft, father1, father2);
	checkTableAlikness(2, dealer, pairs, father1, father2);

	float totalCells = (float)(dealer * (hard + soft + pairs));

	return ((fitsFather1 / totalCells) / (fitsFather2 / totalCells));
}
