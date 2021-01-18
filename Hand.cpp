#include <iostream>
#include <sstream>

#include "Hand.h"
using namespace std;

Hand::Hand() :money(0), ace(0), blackjack(false) {}

void Hand::setMoney(int money) { this->money = money; }
int Hand::getMoney() { return money; };

void Hand::printHand() {
	cout << "Current hand: ";
	for (auto x : hand) {
		cout << x->toString() << " ";
	}
	cout << "\n";
	cout << "This hand sum is: " << sumNbet.top()[0] << "\n";
}

Card* temp;
void Hand::draw(Deck& deck) {
	temp = deck.draw();
	hand.push_back(temp);

	//cout << temp->toString() << "\n";

	sumNbet.top()[0] += temp->getValue();
	if (temp->isAce()) ace++;

	if (ace > 0 && sumNbet.top()[0] > 21) {
		ace--;
		sumNbet.top()[0] -= 10;
	}
}

char Hand::move(int bet) {
	stringstream output;
	output << "Press s for stand or h for hit ";

	bool firstMove = hand.size() == 2;
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

void Hand::turn(Deck& deck, int bet) {
	//Draw strating hand
	money -= bet; ace = 0;

	array<int, 2> temp = { 0, bet };
	sumNbet.push(temp);

	draw(deck);	draw(deck);

	if (sumNbet.top()[0] == 21) {
		printHand();

		blackjack = true;
		hand.clear(); ace = 0;

		return;
	}

	char choice;
	stack<Card*> splitted;
	splitted.push(new Card(-1, '0'));

	//While there is a hand to play
	while (!splitted.empty()) {
		splitted.pop();

		printHand();

		//The first move is seperated to optimize the code (to know if the player has standed)
		if (sumNbet.top()[0] < 21) choice = move(bet);
		while (sumNbet.top()[0] < 21 && choice != 's') {
			if (choice == 'p') {
				splitted.push(hand[1]);
				hand.erase(hand.begin() + 1);
				sumNbet.top()[0] -= splitted.top()->getValue();
				//When splits a pair of A substruct 1 instead of 11
				if (splitted.top()->getValue() == 11) sumNbet.top()[0] += 10;
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

			if (sumNbet.top()[0] < 21) choice = move(bet);
		}

		hand.clear(); ace = 0;
		if (!splitted.empty()) {
			ace = splitted.top()->isAce();
			hand.push_back(splitted.top());
			array<int, 2> temp = { splitted.top()->getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);
			cout << "\n";
			draw(deck);
		}
	}
}

void Hand::results(Dealer& d, Deck& deck) {
	if (blackjack) {
		cout << "Balckjack!!! \n";

		blackjack = false;
		money += 2 * sumNbet.top()[1];

		cout << "Hand number 1 has won " << sumNbet.top()[1] << " \n";
		sumNbet.pop();

		return;
	}

	int profits; int count = 1; int sum; int bet;
	int dealer = -1;

	while (!sumNbet.empty()) {
		sum = sumNbet.top()[0]; bet = sumNbet.top()[1];
		if (sum > 21) {
			cout << "Hand number " << count << " has lost " << bet << "\n";
			count++;
			sumNbet.pop();
			continue;
		}

		if (dealer == -1) {
			dealer = d.play(deck, true);
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


void Hand::turn(Deck& deck, DM& dm, Card* dealer, bool output) {
	//Draw strating hand
	money -= betDM; ace = 0;

	array<int, 2> temp = { 0, betDM };
	sumNbet.push(temp);

	draw(deck); draw(deck);

	//Check for Blackjack
	if (sumNbet.top()[0] == 21) {

		if (output) {
			printHand();
		}

		blackjack = true;
		hand.clear(); ace = 0;

		return;
	}

	char choice;
	stack<Card*> splitted;
	splitted.push(new Card(-1, '0'));
	bool canSplit; bool firstMove;
	int wantSplit = 0;//0 means unintialized, -1 means don't want to split, any other number means the couple splitted

	//While there is a hand to play
	while (!splitted.empty()) {
		splitted.pop();
		
		if (output) {
			printHand();
			cout << "\n";
		}

		//If already declared it doesn't want to split then don't check this possibility again
		if (wantSplit != -1) {
			canSplit = hand[0]->getValue() == hand[1]->getValue();
			//If can split check if it wants to
			if (canSplit && wantSplit == 0) {
				if (dm.decideSplit(sumNbet.top()[0], dealer->getValue(), ace > 0, !output))
					wantSplit = hand[0]->getValue();
				else wantSplit = -1;
			}
			if (wantSplit > 0) {
				while (canSplit) {
					//Initiate split
					if (output) {
						cout << "The computer has chosen to p \n";
					}
					money -= betDM;

					splitted.push(hand[1]);
					hand.erase(hand.begin() + 1);
					sumNbet.top()[0] -= splitted.top()->getValue();
					//When splits a pair of A substruct 1 instead of 11
					if (splitted.top()->getValue() == 11) sumNbet.top()[0] += 10;

					draw(deck); 
					if (output) {
						printHand();
					}

					canSplit = hand[0]->getValue() == hand[1]->getValue();
				}
			}
		}

		//If gotten a perfect hand after splitting (not blackjack) make a ranking hand of 'p' only
		if (sumNbet.top()[0] == 21 && !output) {
			dm.perfectAfterSplit(dealer->getValue(), wantSplit);
		}

		//The first move is seperated to optimize the code
		if (sumNbet.top()[0] < 21) {
			choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), true, ace > 0, wantSplit, !output);
			if (output) cout << "The computer has chosen to " << choice << "\n";
		}
		//Double scenario
		if (sumNbet.top()[0] < 21 && choice == 'd') {

			money -= betDM;

			sumNbet.top()[1] += betDM;
			draw(deck);

			if (output) {
				printHand();
				cout << "\n";
			}
		}
		//Hitting loop
		else {
			while (sumNbet.top()[0] < 21 && choice != 's') {
				draw(deck);

				if (output) {
					printHand();
					cout << "\n";
				}

				if (sumNbet.top()[0] < 21) {
					choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), false, ace > 0, wantSplit, !output);
					if (output) cout << "The computer has chosen to " << choice << "\n";
				}
			}
		}

		hand.clear(); ace = 0;
		if (!splitted.empty()) {
			ace = splitted.top()->isAce();
			hand.push_back(splitted.top());
			array<int, 2> temp = { splitted.top()->getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);
			draw(deck);
		}
	}
}

void Hand::results(Dealer& d, Deck& deck, DM& dm, bool ranking, bool output) {
	if (blackjack) {
		if(output) cout << "Balckjack!!! " << "\n";
		if (!ranking) money += betDM * 2;
		
		sumNbet.pop();
		blackjack = false;

		return;
	}
	int profits; int sum; int bet;
	int dealer = -1; int count = 0;
	while (!sumNbet.empty()) {
		++count;
		sum = sumNbet.top()[0]; bet = sumNbet.top()[1];
		if (sum > 21) {
			if (output) cout << "Hand number " << count << " lost " << bet << "\n";
			if(ranking) dm.rank(false, false);

			sumNbet.pop();
			continue;
		}

		if (dealer == -1)
			dealer = d.play(deck, !ranking);

		if (sum < dealer) {
			if (output) cout << "Hand number " << count << " lost " << bet << "\n";
			if (ranking) dm.rank(false, false);
		}

		else if (sum > dealer) {
			if (output) cout << "Hand number " << count << " won " << bet << "\n";
			if (ranking) dm.rank(true, false);

			money += 2 * bet;
		}
		else {
			if (output) cout << "Hand number " << count << " didn't win or lose \n";
			if (ranking) dm.rank(NULL, true);
			money += bet;
		}

		sumNbet.pop();
	}
}
