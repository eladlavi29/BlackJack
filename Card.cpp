#include <iostream>
#include <sstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Card.h"
using namespace std;

Card::Card(int num, char type): num(num), type(type) {}

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


Deck::Deck() {
	srand(time(NULL));
}

void Deck::shuffle() {
	if (deck.size() >= minNeeded) return;

	deck.clear();
	char types[4] = { 'h', 'd', 'c', 's' };
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 13; j++)
			deck.push_back(new Card(j, types[i]));
	}
}

Card* Deck::draw() {
	int r = rand() % deck.size();
	Card* card = deck[r];
	
	deck.erase(deck.begin() + r);

	return card;
}

void Deck::print() {
	for (auto x : deck) {
		cout << x->toString() << " ";
	}
	cout << "\n";
}


Dealer::Dealer():handSum(0), ace(false){}

void Dealer::newGame(Deck& deck, bool output) {
	dealer.clear(); handSum = 0; ace = false;
	dealer.push_back(deck.draw());
	handSum += dealer[0]->getValue();
	if (!ace && dealer[0]->isAce()) ace = true;
	if(output)
		cout << "The dealer's revealed card is " << dealer[0]->toString() << "\n";
}

int Dealer::play(Deck& deck, bool output) {
	if(output)
		cout << "The dealer's cards: " << dealer[0]->toString() << " ";
	int count = 1;
	while (handSum < 17) {
		dealer.push_back(deck.draw());
		handSum += dealer[count]->getValue();
		if (!ace && dealer[count]->isAce()) ace = true;

		if(output) cout << dealer[count]->toString() << " ";
		count++;
		if (ace && handSum > 21) {
			ace = false;
			handSum -= 10;
		}
	}
	if(output)
		cout << "\nHis sum: " << handSum << "\n";
	if (handSum > 21) return 0;
	return handSum;
}

Card* Dealer::reaveled() {
	return dealer[0];
}
