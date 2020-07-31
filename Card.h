#pragma once
#include <iostream>
#include <vector>
#include <set>
using namespace std;

//The object Card
class Card{
private:
	int num;
	char type;

public:
	Card(int num, char type);

	//Get the card value according to blackjack
	int getValue();

	//Return if a card is an ace
	bool isAce();

	string toString();
};

//The object Deck
const int minNeeded = 6 * 4 + 3; //The min cards needed in a deck for a round

class Deck {
private:
	vector<Card*> deck;

	set<Card*> s;

public:
	Deck();
	//Shuffle a new deck
	void shuffle();
	//Draw a card from the deck
	Card* draw();

	void print(); //Print the deck (debugging function)
};

//The object Dealer
class Dealer {
private:
	vector<Card*> dealer;
	int handSum;
	bool ace;
public:
	Dealer();
	//Generate a new dealer's hand
	void newGame(Deck& deck, bool output);
	//The dealer's turn method
	int play(Deck& deck, bool output);

	//return a pointer to the dealer reaveled card
	Card* reaveled();
};

