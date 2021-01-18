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

bool DM::decideSplit(int sum, int dealerReaveled, bool hasAce, bool ranking) {
	dealerReaveled -= 2;

	if (hasAce) {
		//If decided to split hasSplitted will be updated and any new hand will regard 'p' as a move
		if (dm[2][dealerReaveled][9] == 'p') {
			return true;
		}
		//Else 'n' will be regarded once
		else {
			if (ranking) addMove(2, dealerReaveled, 9);
			return false;
		}
	}
	else {
		//If decided to split hasSplitted will be updated and any new hand will regard 'p' as a move
		if (dm[2][dealerReaveled][sum / 2 - 2] == 'p') return true;
		//Else 'n' will be regarded once
		else {
			if (ranking) addMove(2, dealerReaveled, sum / 2 - 2);
			return false;
		}
	}
}

inline char fix(bool firstMove, char ch) {
	if (ch == 'd' && !firstMove) ch = 'h';

	return ch;
}
char DM::decideReg(int sum, int dealerReaveled, bool firstMove, bool hasAce, int hasSplitted, bool ranking) {
	dealerReaveled -= 2; char ch;
	
	//As explained in decideSplit
	if (hasSplitted > 0 && firstMove && ranking) {
		addMove(2, dealerReaveled, hasSplitted - 2);
	}

	if (hasAce) {
		if (ranking) addMove(1, dealerReaveled, sum - 11 - 1);
		return fix(firstMove, dm[1][dealerReaveled][sum - 11 - 1]);
	}

	if (ranking) addMove(0, dealerReaveled, sum - 4);
	return fix(firstMove, dm[0][dealerReaveled][sum - 4]);
}

void DM::perfectAfterSplit(int dealerReaveled, int hasSplitted) {
	//If splitted and then got a perfect hand then the only move in the hand is split
	//Therefore there has to be a special function that addMove p specificaly for this scenario
	addMove(2, dealerReaveled - 2, hasSplitted - 2);
}

int index = 0; bool aaa = true;
void DM::rank(bool result, bool tie) {

	if (aaa) {
		for (int i = 0; i < plays.size(); ++i) {
			cout << dm[plays[i][0]][plays[i][1]][plays[i][2]] << " ";
		}
		cout << "\n";
		aaa = false;
	}

	const int plays_size = plays.size();
	//If it's a tie skip this hand
	if (tie) {
		for (; index < plays_size && dm[plays[index][0]][plays[index][1]][plays[index][2]] != 'p'; ++index) {}

		if (index == plays_size) {
			plays.clear();
			index = 0;
		}

		return;
	}

	//Count how many decision were in this hand
	int decisionCount = 0;
	if (dm[plays[index][0]][plays[index][1]][plays[index][2]] == 'p') {
		++index; ++decisionCount;
	}
	for (int i = index; i < plays_size && dm[plays[i][0]][plays[i][1]][plays[i][2]] != 'p'; ++i) {
		++decisionCount;
	}

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
	print(2, dealer, pairs);

	/*if (plays.empty()) {
	cout << "empty \n";
	return;
	}
	for (int i = 0; i < plays.size(); ++i) {
		cout << dm[plays[i][0]][plays[i][1]][plays[i][2]] << " ";
	}
	plays.clear();
	cout << "\n\n";*/
}
