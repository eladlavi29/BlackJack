#include <iostream>
#include <sstream>

#include "Hand.h"
using namespace std;

Hand::Hand():money(0){}

void Hand::setMoney(int money) { this->money = money; }
int Hand::getMoney() { return money; };

void Hand::printHand() {
	cout << "Your current hand: ";
	for (auto x : hand) {
		cout << x->toString() << " ";
	}
	cout << "\n";
	cout << "This hand sum is: " << sumNbet.top()[0] << "\n";
}

void Hand::draw(Deck& deck) {
	hand.push_back(deck.draw());
	sumNbet.top()[0] += hand[hand.size() - 1]->getValue();
	if (!ace && hand[hand.size() - 1]->isAce()) ace = true;

	if (ace && sumNbet.top()[0] > 21) {
		ace = false;
		sumNbet.top()[0] -= 10;
	}
}

char Hand::move(bool firstMove, int bet) {
	stringstream output;
	output << "Press s for stand or h for hit ";

	//if can Double add option
	bool canDouble = firstMove && money >= bet;
	if (canDouble) {
		output << "or d for double ";
	}

	//If can split add option
	bool canSplit = firstMove && hand[0]->getValue() == hand[1]->getValue() && money >= bet;
	if (canSplit) output << "or p for split ";

	//Print options
	cout << output.str() << "\n";

	char choice;
	cin >> choice;

	return choice;
}

void Hand::turn(Deck &deck, int bet) {
	//Draw strating hand
	money -= bet; ace = false;

	array<int, 2> temp = {0, bet};
	sumNbet.push(temp);

	for (int i = 0; i < 2; i++) 
		draw(deck);
	
	char choice;
	Card splitted(-1, '0'); int playing = 1;

	//While there is a hand to play
	while (playing > 0) {
		playing--;

		printHand();

		if(sumNbet.top()[0] < 21) choice = move(true, bet);
		while (sumNbet.top()[0] < 21 && choice != 's') {
			if (choice == 'p') {
				playing++;
				splitted = *hand[1];
				hand.erase(hand.begin() + 1);
				sumNbet.top()[0] -= splitted.getValue();
			}
			else if (choice == 'd') {
				money -= bet;

				sumNbet.top()[1] += sumNbet.top()[1];
				draw(deck);

				printHand();

				break;
			}
			draw(deck);

			printHand();

			if(sumNbet.top()[0] < 21) choice = move(false, bet);
		}

		hand.clear(); ace = false;
		if (playing > 0) {
			hand.push_back(&splitted);
			array<int, 2> temp = { splitted.getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);
			cout << "\n";
			draw(deck);
		}
	}
}

void Hand::results(Dealer& d, Deck& deck) {
	int profits; int count = 1; int sum; int bet;
	int dealer = -1;;
	while (!sumNbet.empty()) {
		int sum = sumNbet.top()[0]; int bet = sumNbet.top()[1];
		if (sum > 21) {
			cout << "Hand number " << count << " has lost " << bet << "\n";
			count++;
			sumNbet.pop();
			continue;
		}

		if (dealer == -1) {
			dealer = d.play(deck);
		}

		cout << "Hand number " << count << " has ";
		if (sum < dealer) {
			cout << "lost " << bet << "\n";
		}
		else if (sum > dealer) {
			money += 2 * bet;
			cout << "won " << bet << "\n";
		}
		else {
			money += bet;
			cout << "not won or lost \n";
		}

		count++;
		sumNbet.pop();
	}
}
