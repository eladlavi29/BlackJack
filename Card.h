#pragma once
#include <iostream>
#include <vector>
using namespace std;

//The object Card
class Card {
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
class Deck {
private:
	vector<Card*> deck;
	vector<Card*> throwenCards;

public:
	Deck();
	//Generate the deck for the new round
	void throwCards(vector<Card*>& vec);
	void shuffle();
	//Draw a card from the deck
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
	//Generate a new dealer's hand
	void newGame(Deck& deck, bool output);
	//The dealer's turn method
	int play(Deck& deck, bool output);

	//return a pointer to the dealer reaveled card
	Card* reaveled();
};