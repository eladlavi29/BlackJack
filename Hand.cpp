#include "Hand.h"
#include "graphicsFuncs.h"
using namespace std;

Hand::Hand() :money(0), ace(0), blackjack(false) {}

void Hand::setMoney(int money) { this->money = money; }
int Hand::getMoney() { return money; };

vector<Card*> Hand::getCards() {
	return this->hand;
}

Card* temp;
void Hand::draw(Deck& deck) {
	temp = deck.draw();
	hand.push_back(temp);

	sumNbet.top()[0] += temp->getValue();
	if (temp->isAce()) ace++;

	if (ace > 0 && sumNbet.top()[0] > 21) {
		ace--;
		sumNbet.top()[0] -= 10;
	}
}

char Hand::move(int bet) {

	bool firstMove = hand.size() == 2;
	bool canDouble = firstMove && money >= bet;
	bool canSplit = firstMove && hand[0]->getValue() == hand[1]->getValue() && money >= bet;

	return playerMove(canDouble, canSplit);
}

void Hand::turn(Deck& deck, int bet) {
	array<int, 2> temp = { 0, bet };
	sumNbet.push(temp); money -= bet;

	ace = 0;
	draw(deck);	draw(deck);

	if (sumNbet.top()[0] == 21) {
		playerHand(hand, sumNbet);

		blackjack = true;
		deck.throwCards(hand);
		ace = 0;

		playerResult(bet, 2);

		return;
	}

	char choice;
	stack<Card*> splitted;
	splitted.push(new Card(-1, '0'));

	//While there is a hand to play
	while (!splitted.empty()) {
		splitted.pop();

		playerHand(hand, sumNbet);

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

				playerHand(hand, sumNbet);

				break;
			}
			draw(deck);

			playerHand(hand, sumNbet);

			if (sumNbet.top()[0] < 21) choice = move(bet); //Text
		}

		deck.throwCards(hand);

		ace = 0;
		if (!splitted.empty()) {
			ace = splitted.top()->isAce();
			hand.push_back(splitted.top());
			array<int, 2> temp = { splitted.top()->getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);
			draw(deck);
		}
	}
}

void Hand::results(Dealer& d, Deck& deck, int dealerSum, bool isPlayer) {
	if (blackjack) {
		blackjack = false;
		money += sumNbet.top()[1] * 15 / 10;

		sumNbet.pop();

		return;
	}

	int profits; int sum; int bet; bool moveToNextHand = false;

	while (!sumNbet.empty()) {
		if (moveToNextHand) {
			if (isPlayer) playerNextHandResult();
			else AINextHandResult();
		}
		else moveToNextHand = true;

		sum = sumNbet.top()[0]; bet = sumNbet.top()[1];
		if (sum > 21) {
			if (isPlayer) playerResult(bet, 0);
			else AIResult(bet, 0);
			sumNbet.pop();
			continue;
		}

		if (sum < dealerSum) {
			if (isPlayer) playerResult(bet, 0);
			else AIResult(bet, 0);
		}
		else if (sum > dealerSum) {
			money += 2 * bet;
			if (isPlayer) playerResult(bet, 2);
			else AIResult(bet, 2);
		}
		else {
			money += bet;
			if (isPlayer) playerResult(bet, 1);
			else AIResult(bet, 1);
		}

		sumNbet.pop();
	}
}


void Hand::turn(Deck& deck, DM& dm, Card* dealer, int bet) {
	array<int, 2> temp = { 0, bet };
	sumNbet.push(temp); money -= bet;

	ace = 0;
	draw(deck);	draw(deck);

	//Check for Blackjack
	if (sumNbet.top()[0] == 21) {

		AIHand(hand, sumNbet);

		blackjack = true;
		deck.throwCards(hand);

		ace = 0;

		AIResult(bet, 2);

		return;
	}

	char choice;
	stack<Card*> splitted;
	splitted.push(new Card(-1, '0'));
	bool canSplit; bool firstMove;
	bool wantSplit = NULL;//0 means unintialized, -1 means don't want to split, any other number means the couple splitted

	//While there is a hand to play
	while (!splitted.empty()) {
		splitted.pop();

		AIHand(hand, sumNbet);

		//If already declared it doesn't want to split then don't check this possibility again
		if (wantSplit != false) {
			canSplit = hand[0]->getValue() == hand[1]->getValue();
			//If can split check if it wants to
			if (canSplit && wantSplit == NULL) {
				if (dm.decideSplit(sumNbet.top()[0], dealer->getValue(), ace > 0))
					wantSplit = true;
				else wantSplit = false;
			}
			if (wantSplit == true) {
				while (canSplit) {
					//Initiate split
					AIMove('p');
					money -= bet;

					splitted.push(hand[1]);
					hand.erase(hand.begin() + 1);
					sumNbet.top()[0] -= splitted.top()->getValue();
					//When splits a pair of A substruct 1 instead of 11
					if (splitted.top()->getValue() == 11) sumNbet.top()[0] += 10;

					draw(deck);
					AIHand(hand, sumNbet);

					canSplit = hand[0]->getValue() == hand[1]->getValue();
				}
			}
		}

		//The first move is seperated to optimize the code
		if (sumNbet.top()[0] < 21) {
			choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), true, ace > 0);
			AIMove(choice);
		}
		//Double scenario
		if (sumNbet.top()[0] < 21 && choice == 'd') {

			money -= bet;

			sumNbet.top()[1] += bet;
			draw(deck);

			AIHand(hand, sumNbet);
		}
		//Hitting loop
		else {
			while (sumNbet.top()[0] < 21 && choice != 's') {
				draw(deck);

				AIHand(hand, sumNbet);

				if (sumNbet.top()[0] < 21) {
					choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), false, ace > 0);
					AIMove(choice);
				}
			}
		}

		deck.throwCards(hand);

		ace = 0;
		if (!splitted.empty()) {
			ace = splitted.top()->isAce();
			hand.push_back(splitted.top());
			array<int, 2> temp = { splitted.top()->getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);

			draw(deck);
		}
	}
}


void Hand::turnRanking(Deck& deck, DM& dm, Card* dealer) {
	//Draw strating hand
	money -= betDM; ace = 0;

	array<int, 2> temp = { 0, betDM };
	sumNbet.push(temp);

	draw(deck); draw(deck);

	//Check for Blackjack
	if (sumNbet.top()[0] == 21) {

		blackjack = true;
		deck.throwCards(hand);

		ace = 0;

		return;
	}

	char choice;
	stack<Card*> splitted;
	splitted.push(new Card(-1, '0'));
	bool canSplit; bool firstMove;
	bool wantSplit = NULL;//0 means unintialized, -1 means don't want to split, any other number means the couple splitted

	//While there is a hand to play
	while (!splitted.empty()) {
		splitted.pop();

		//If already declared it doesn't want to split then don't check this possibility again
		if (wantSplit != false) {
			canSplit = hand[0]->getValue() == hand[1]->getValue();
			//If can split check if it wants to
			if (canSplit && wantSplit == NULL) {
				if (dm.decideSplit(sumNbet.top()[0], dealer->getValue(), ace > 0))
					wantSplit = true;
				else wantSplit = false;
			}
			if (wantSplit == true) {
				while (canSplit) {
					//Initiate split
					money -= betDM;

					splitted.push(hand[1]);
					hand.erase(hand.begin() + 1);
					sumNbet.top()[0] -= splitted.top()->getValue();
					//When splits a pair of A substruct 1 instead of 11
					if (splitted.top()->getValue() == 11) sumNbet.top()[0] += 10;

					draw(deck);
					canSplit = hand[0]->getValue() == hand[1]->getValue();
				}
			}
		}

		//The first move is seperated to optimize the code
		if (sumNbet.top()[0] < 21) {
			choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), true, ace > 0);
		}
		//Double scenario
		if (sumNbet.top()[0] < 21 && choice == 'd') {

			money -= betDM;

			sumNbet.top()[1] += betDM;
			draw(deck);
		}
		//Hitting loop
		else {
			while (sumNbet.top()[0] < 21 && choice != 's') {
				draw(deck);

				if (sumNbet.top()[0] < 21) {
					choice = dm.decideReg(sumNbet.top()[0], dealer->getValue(), false, ace > 0);
				}
			}
		}

		deck.throwCards(hand);

		ace = 0;
		if (!splitted.empty()) {
			ace = splitted.top()->isAce();
			hand.push_back(splitted.top());
			array<int, 2> temp = { splitted.top()->getValue(), sumNbet.top()[1] };
			sumNbet.push(temp);
			draw(deck);
		}
	}
}

void Hand::resultsRanking(Dealer& d, Deck& deck, DM& dm) {
	if (blackjack) {
		money += betDM * 1.5;

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
			sumNbet.pop();
			continue;
		}

		if (dealer == -1)
			dealer = d.play(deck, false);

		if (sum > dealer) {
			money += 2 * bet;
		}
		if (sum == dealer) {
			money += bet;
		}

		sumNbet.pop();
	}
}