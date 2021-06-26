#pragma once
#include <vector>
#include "Hand.h"
using namespace std;

class Game{
private:
	Deck deck;
	Hand hand;
	Dealer dealer;

public:
	Game(int money);
	void run(); //Run the game with a player
	void run(DM& dm); //Run the game with DM
	void runGame(DM& dm); //Run the game with DM and a player

	//run time for one loop : 0.000027 s
	void rankDM(DM& dm); //Rank a DM

	float rankDMforCalc(DM& dm, int rep); //Rank a DM
};

