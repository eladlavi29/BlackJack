#pragma once
#include <iostream>
#include <vector>
using namespace std;

//The object Card
class Card{
private:
	int num;
	char type;

public:
	Card(int num, char type);

	int getValue();
	bool isAce();

	string toString();
};

//The object Deck
class Deck {
private:
	vector<Card*> deck;
public:
	Deck();
	void shuffle();
	Card* draw();
};

//The object Dealer
class Dealer {
private:
	vector<Card*> dealer;
	int handSum;
	bool ace;
public:
	Dealer();
	void newGame(Deck& deck);
	int play(Deck& deck);
};

