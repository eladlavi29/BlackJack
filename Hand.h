#pragma once
#include <vector>
#include <stack>
#include <array>
#include <sstream>
#include "Card.h"
#include "DM.h"

using namespace std;

//The player's gameplay class
class Hand {
private:
	vector<Card*> hand;
	int ace;
	bool blackjack;
	stack<array<int, 2>> sumNbet;

	int money;
public:
	Hand(); //Generate a Hand
	vector<Card*> getCards();

	void setMoney(int money); //Set the player's money
	int getMoney(); //Return the player's money

	void draw(Deck& deck);
	char move(int bet); //Move acc to input
	void turn(Deck& deck, int bet); //The turn method

	void results(Dealer& d, Deck& deck, int dealerSum, bool isPlayer); //Update the Hand data acc to sumNbet

	//The turn and results with dm when ranking
	void turn(Deck& deck, DM& dm, Card* dealer, int bet);

	//The turn and results with dm when printing output
	void turnRanking(Deck& deck, DM& dm, Card* dealer);
	void resultsRanking(Dealer& d, Deck& deck, DM& dm);
};
