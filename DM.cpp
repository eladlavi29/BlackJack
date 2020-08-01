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
const int hard = 20 - 4; //5 to 20
const int soft = 9 - 1; //2 to 9
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
	options.push_back('p');

	for (int i = 0; i < dealer; ++i) {
		for (int j = 0; j < pairs; ++j) {
			random = options[rand() % options.size()];
			dm[2][i][j] = random;
		}
	}
}

void DM::addMove(int ind, int& num1, int num2) {
	array<int, 3> arr = { ind, num1, num2 };
	plays.push_back(arr);
}

char canDouble(bool firstMove, char ch) {
	if (ch == 'd' && !firstMove) ch = 'h';
	cout << ch << "\n";
	return ch;
}
char DM::decide(int sum, int dealerReaveled, bool firstMove, bool hasAce, bool canSplit, bool ranking) {
	dealerReaveled -= 2;
	if (canSplit) {
		if (hasAce) {
			if (ranking) addMove(2, dealerReaveled, 9);
			return canDouble(firstMove, dm[2][dealerReaveled][9]);
		}
		if (ranking) addMove(2, dealerReaveled, sum / 2 - 2);
		return canDouble(firstMove, dm[2][dealerReaveled][sum / 2 - 2]);
	}

	if (hasAce) {
		if (ranking) addMove(1, dealerReaveled, sum - 11 - 2);
		return canDouble(firstMove, dm[1][dealerReaveled][sum - 11 - 2]);
	}

	if (ranking) addMove(0, dealerReaveled, sum - 5);
	return canDouble(firstMove, dm[0][dealerReaveled][sum - 5]);
}

vector<array<int, 3>> playsCopy;
void DM::rank(bool result, int split) {
	//לדבג אחושרמוטה לדבג ואז להפוך את מערך פיצולים לבוליאני
	int i = 0;
	for (int j = 0; j < split; ++j) {
		++i;
		while (dm[plays[i][0]][plays[i][1]][plays[i][2]] == 'h')
			++i;
	}

	playsCopy.clear();
	playsCopy.push_back(plays[i]); ++i;
	for (; i < plays.size() && dm[plays[i][0]][plays[i][1]][plays[i][2]] == 'h'; ++i) {
		playsCopy.push_back(plays[i]);
	}
	if (i == plays.size())
		plays.clear();
	
	const int plays_size = playsCopy.size();
	int doubleBet = 1;

	if (dm[plays[0][0]][plays[0][1]][plays[0][2]] == 'd') doubleBet = doubleBet * 2;

	if (result)
		dmRanking[plays[i][0]][plays[i][1]][plays[i][2]] += betDM * doubleBet / plays_size;
	else
		dmRanking[plays[i][0]][plays[i][1]][plays[i][2]] -= betDM * doubleBet / plays_size;
	++i;

	for (; i < plays_size && dm[plays[i][0]][plays[i][1]][plays[i][2]] != 'p'; ++i) {
		if (result)
			dmRanking[plays[i][0]][plays[i][1]][plays[i][2]] += betDM * doubleBet / plays_size;
		else
			dmRanking[plays[i][0]][plays[i][1]][plays[i][2]] -= betDM * doubleBet / plays_size;
	}

	cout << "\n" << i << " " << plays_size << "\n\n";


	print(0, dealer, hard);
	print(1, dealer, soft);
	print(2, dealer, pairs);
}
