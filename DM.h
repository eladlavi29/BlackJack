#pragma once
#include <vector>
#include <array>

using namespace std;

const int betDM = 1;

class DM
{
private:
	//The three tables following optimal decision for each situation
	//0 hard hands  1 soft hands  2 pair hands
	char** dm[3];

	//The general chromosome fitness
	int fitness;

	//Auxiliary functions
	void build(int ind, const int& num1, const int& num2);

	//Specific table functions
	void copyTable(int ind, const int& num1, const int& num2, DM* copy);

	void printTable(int ind, const int& num1, const int& num2);

	void crossoverTable(int a, int b, int c, DM* chromo, DM* offspring);

public:
	//Diffult constructor & Copy function
	DM(); void copy(DM* copy);
	//Ransomize dm
	void generate();

	//Make a decision according to the object's data (When split relevant)
	bool decideSplit(int sum, int dealerReaveled, bool hasAce);
	//Make a decision according to the object's data (When split irrelevant)
	char decideReg(int sum, int dealerReaveled, bool firstMove, bool hasAce);

	//Return and change the object's fitness score
	int getFitness();
	void setFitness(int fitness);

	//Set and get a decision at the dm
	char getMove(int a, int b, int c);
	void setMove(int a, int b, int c, char move);

	//Ideal is -0.006
	double getWinningExpectation();

	void printData();

	DM* crossover(DM* chromo);

	void checkTableAlikness(int a, int b, int c, DM* father1, DM* father2);
	float checkOffspringAlikeness(DM* father1, DM* father2);
};
