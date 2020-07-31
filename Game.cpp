#include "Game.h"

Game::Game(int money) {
	hand.setMoney(money);
}

void Game::run() {

	int input;
	while (hand.getMoney() > 0){
		//The game
		cout << "Your money: " << hand.getMoney() << "\n";
		cout << "Insert any amount of money you have to bet \nInsert any other number to stop playing \n";
		cin >> input;
		if (input <= 0 || input > hand.getMoney())
			//End the game if the player didn't insert amont of money he has
			break;
		cout << "\n";

		deck.shuffle();
		dealer.newGame(deck);
		hand.turn(deck, input);
		cout << "\n";
		
		//The results
		hand.results(dealer, deck);

		cout << "\n";	
	}

	cout << "Game ended. Your money: " << hand.getMoney() << "\n";
}

