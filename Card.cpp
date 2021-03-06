#include <iostream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include "Card.h"
#include "practicalFuncs.h"
#include "graphicsFuncs.h"
using namespace std;

Card::Card(int num, char type) : num(num), type(type) {}

int Card::getValue() {
	if (num == 1) return 11;
	if (num >= 10) return 10;
	return num;
}

bool Card::isAce() { return num == 1; }

string Card::toString() {
	stringstream ss;
	char rank;
	switch (num) {
	case(1): rank = 'A';
		break;
	case(13): rank = 'K';
		break;
	case(12): rank = 'Q';
		break;
	case(11): rank = 'J';
		break;
	case(10): rank = '0'; ss << 1;
		break;
	default: rank = num + '0';
	}
	ss << rank << type;

	return ss.str();
}

int Card::cardIndex() {
	return (find(cardTypes, cardTypes + 4, this->type) - cardTypes) * 13 + this->num - 1;
}

Deck::Deck() {
	//Generate a deck
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 13; j++)
			deck.push_back(new Card(j, cardTypes[i]));
	}
}

void Deck::throwCards(vector<Card*>& vec) {
	for (auto x : vec) throwenCards.push_back(x);
	//deleteVector
	//vector<Card*>().swap(vec);
	vec.clear(); //vec.shrink_to_fit();
}
void Deck::shuffle() {
	for (auto x : throwenCards) deck.push_back(x);
	//deleteVector
	//vector<Card*>().swap(throwenCards);
	throwenCards.clear(); //throwenCards.shrink_to_fit();
}

Card* Deck::draw() {
	int r = (int)random() % deck.size();
	Card* card = deck[r];

	deck.erase(deck.begin() + r);

	return card;
}


Dealer::Dealer() :handSum(0), ace(false) {}

void Dealer::newGame(Deck& deck, bool output) {
	deck.throwCards(dealer);
	handSum = 0; ace = false;
	dealer.push_back(deck.draw());
	handSum += dealer[0]->getValue();
	if (!ace && dealer[0]->isAce()) ace = true;
	//Text
	if (output)
		dealerNewGame(dealer[0]);
}

int Dealer::play(Deck& deck, bool output) {
	int count = 1;
	while (handSum < 17) {
		dealer.push_back(deck.draw());
		handSum += dealer[count]->getValue();
		if (!ace && dealer[count]->isAce()) ace = true;

		count++;
		if (ace && handSum > 21) {
			ace = false;
			handSum -= 10;
		}
	}
	if (output)
		dealerHand(dealer, handSum);
	if (handSum > 21) return 0;
	return handSum;
}

Card* Dealer::reaveled() {
	return dealer[0];
}
