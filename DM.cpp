#include "DM.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>  

using namespace std;

int DM::getFitness() {
	return fitness;
}
void DM::setFitness(int fitness) {
	this->fitness = fitness;
}

#define cd (double)
double DM::getWinningExpectation() {
	return cd fitness / cd betDM / cd timesWithMinCoeffcient;
}

void DM::build(int ind, const int& num1, const int& num2) {
	dm[ind] = new char* [num1];
	dmRanking[ind] = new int* [num1];
	for (int i = 0; i < num1; ++i) {
		dm[ind][i] = new char[num2];
		dmRanking[ind][i] = new int[num2];
		for (int j = 0; j < num2; ++j)
			dmRanking[ind][i][j] = 0;
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

void DM::print(int ind, const int& num1, const int& num2) {
	for (int i = 0; i < num1; ++i) {
		for (int j = 0; j < num2; ++j) {
			cout << dmRanking[ind][i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void DM::generate() {
	srand(time(NULL));
	vector<char> options = { 's', 'h', 'd' };
	char random;

	//Hard hands' table
	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < hard; ++j) {
			random = options[rand() % options.size()];
			dm[0][i][j] = random;
		}
	}

	//Soft hands' table;
	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < soft; ++j) {
			random = options[rand() % options.size()];
			dm[1][i][j] = random;
		}
	}

	//Pair hands' table
	options.clear();
	options.push_back('p');
	options.push_back('n');

	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < pairs; ++j) {
			random = options[rand() % options.size()];
			dm[2][i][j] = random;
		}
	}
}

void DM::addMove(int ind, int num1, int num2) {
	array<int, 3> arr = { ind, num1, num2 };
	plays.push_back(arr);
}

char fix(bool firstMove, char ch) {
	if (ch == 'd' && !firstMove) ch = 'h';
	return ch;
}
char DM::decide(int sum, int dealerReaveled, bool firstMove, bool hasAce, bool canSplit, bool hasSplitted, bool ranking) {
	dealerReaveled -= 2; char ch;
	if (canSplit) {
		if (hasAce) {
			//Can also rank the decision not to p
			if (ranking) addMove(2, dealerReaveled, 9);
			ch = dm[2][dealerReaveled][0];
		}
		else {
			//Can also rank the decision not to p
			if (ranking) addMove(2, dealerReaveled, sum / 2 - 2);
			ch = dm[2][dealerReaveled][sum / 2 - 2];
		}

		if (ch == 'p')
			return 'p';
	}
	//If reached this point then didn't split
	//If has splitted but didn't split then add p
	if(hasSplitted)
		if (ranking) addMove(2, dealerReaveled, sum / 2 - 2);


	if (hasAce) {
		if (ranking) addMove(1, dealerReaveled, sum - 11 - 1);
		return fix(firstMove, dm[1][dealerReaveled][sum - 11 - 1]);
	}

	if (ranking) addMove(0, dealerReaveled, sum - 4);
	return fix(firstMove, dm[0][dealerReaveled][sum - 4]);
}

int index = 0;
void DM::rank(bool result) {
	//arrange the p before the decisions

	if (plays.empty()) return;
	for (int i = 0; i < plays.size(); ++i) cout << dm[plays[i][0]][plays[i][1]][plays[i][2]] << " ";
	plays.clear();
	cout << "\n";
	/*const int plays_size = plays.size();
	int decisionCount = 0;
	int i;
	for (i = index; i < plays_size && dm[plays[i][0]][plays[i][1]][plays[i][2]] != 'p'; ++i)
		++decisionCount;
	if (i != plays_size) ++decisionCount;

	int doubleBet = 1;
	if (dm[plays[index][0]][plays[index][1]][plays[index][2]] == 'd') doubleBet = doubleBet * 2;
	
	for (; index < plays_size && dm[plays[index][0]][plays[index][1]][plays[index][2]] != 'p'; ++index) {
		if (result)
			dmRanking[plays[index][0]][plays[index][1]][plays[index][2]] += betDM * doubleBet / decisionCount;
		else
			dmRanking[plays[index][0]][plays[index][1]][plays[index][2]] -= betDM * doubleBet / decisionCount;
	}

	if (!index == plays_size) {
		if (result)
			dmRanking[plays[index][0]][plays[index][1]][plays[index][2]] += betDM * doubleBet / decisionCount;
		else
			dmRanking[plays[index][0]][plays[index][1]][plays[index][2]] -= betDM * doubleBet / decisionCount;
		++index;
	}

	cout << index << " " << plays_size << "\n\n";

	if (index == plays_size) {
		plays.clear();
		index = 0;
	}

	print(0, dealer, hard);
	print(1, dealer, soft);
	print(2, dealer, pairs);*/
}
