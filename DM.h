#pragma once
#include <vector>
#include <array>

using namespace std;

const int betDM = 100;
const int timesWithMinCoeffcient = 1;
class DM
{
private:
	//The three tables following optimal decision for each situation
	//0 hard hands  1 soft hands  2 pair hands
	char** dm[3];

	//The rank of each of the decisions
	int** dmRanking[3];

	//The general chromosome fitness
	int fitness;

	//The decisions made in the round
	vector<array<int, 3>> plays;

	//Auxiliary functions
	void build(int ind, const int& num1, const int& num2);

	//Add a move to the vector
	void addMove(int ind, int num1, int num2);
public:
	//Diffult constructor
	DM(); //insert 0 everywhere in dmRanking
	//Ransomize dm
	void generate();

	//Make a decision according to the object's data (When split relevant)
	bool decideSplit(int sum, int dealerReaveled, bool hasAce, bool ranking);
	//Make a decision according to the object's data (When split irrelevant)
	char decideReg(int sum, int dealerReaveled, bool firstMove, bool hasAce, int hasSplitted, bool ranking);
	//Special scenario, got a perfect hand (21 2 card) and not black jack
	void perfectAfterSplit(int dealerReaveled, int hasSplitted);
	//Update dmRanking according to the results
	void rank(bool result, bool tie);

	//Return and change the object's fitness score
	int getFitness();
	void setFitness(int fitness);

	double getWinningExpectation();

	void print(int ind, const int& num1, const int& num2);
};
