#include "Game.h"

Game::Game(int money) {
	hand.setMoney(money);
}

void Game::run() {

	int input;
	while (hand.getMoney() > 0) {
		//The game
		cout << "Your money: " << hand.getMoney() << "\n";
		cout << "Insert any amount of money you have to bet \nInsert any other number to stop playing \n";
		cin >> input;
		if (input <= 0 || input > hand.getMoney())
			//End the game if the player didn't insert amont of money he has
			break;
		cout << "\n";

		deck.shuffle();
		dealer.newGame(deck, true);
		hand.turn(deck, input);
		cout << "\n";

		//The results
		hand.results(dealer, deck);

		cout << "\n";
	}

	cout << "Game ended. Your money: " << hand.getMoney() << "\n";
}


void Game::run(DM& dm) {
	int input = 0;
	while (input >= 0) {
		//The game
		cout << "The computer's money: " << hand.getMoney() << "\n";

		if (input == 0) {
			cout << "Insert how many more rounds you want the computer to play before asking again \nenter 0 to stop playing ";
			cin >> input;
			cout << "\n";

			if (input == 0) break;
		}
		--input;

		deck.shuffle();
		dealer.newGame(deck, true);
		hand.turn(deck, dm, dealer.reaveled(), true);
		cout << "\n";

		//The results
		hand.results(dealer, deck, dm, false, true);

		cout << "\n";
	}

	dm.setFitness(hand.getMoney());

	cout << "Game ended. The computer's money: " << dm.getFitness() << "\n";
}

void Game::rankDM(DM& dm) {
	for (int i = 0; i < timesWithMinCoeffcient; ++i) {
		//Prepare the game
		deck.shuffle();
		dealer.newGame(deck, false);

		hand.turn(deck, dm, dealer.reaveled(), false);

		//The results
		hand.results(dealer, deck, dm, true, false);
	}

	dm.setFitness(hand.getMoney());
	cout << "The object final fitness is " << dm.getFitness() << "\n";
	dm.print(0, 10, 16);
	dm.print(1, 10, 8);
	dm.print(2, 10, 10);
}

int Game::rankDMforCalc(DM& dm, int rep) {
	for (int i = 0; i < rep; ++i) {
		//The game
		deck.shuffle();
		dealer.newGame(deck, false);
		hand.turn(deck, dm, dealer.reaveled(), false);

		//The results
		hand.results(dealer, deck, dm, true, false);
	}

	return hand.getMoney();
}
