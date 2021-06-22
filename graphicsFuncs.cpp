#include <iostream>
#include <sstream>

#include "graphicsFuncs.h"

//Technical
//Data
//Images size and placements
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 880;

const int CARD_WIDTH = 200;
const int CARD_HEIGHT = 280;

SDL_Rect dealerHandGrid; //()
SDL_Rect playerHandGrid; //()
SDL_Rect AIHandGrid; //()

//The window we'll be rendering to
SDL_Window* gWindow;

//The surface contained by the window
SDL_Surface* gScreenSurface;

//The placement of each image printed
SDL_Rect dest;
int change;

//Images
SDL_Surface* background_image;
SDL_Surface* cover_grid;
SDL_Surface* card_back;
SDL_Surface* cardsImages[52];

//Technical Funcs
void loadMedia() {
	//Create window
	gWindow = SDL_CreateWindow("Blackjack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	//Images
	background_image = SDL_LoadBMP("Images/blackjack_background.bmp");
	cover_grid = SDL_LoadBMP("Images/cover.bmp");
	card_back = SDL_LoadBMP("Images/card_back.bmp");

	//Optimize images' upload by formating them
	background_image = SDL_ConvertSurface(background_image, gScreenSurface->format, 0);
	cover_grid = SDL_ConvertSurface(cover_grid, gScreenSurface->format, 0);
	card_back = SDL_ConvertSurface(card_back, gScreenSurface->format, 0);

	//Load and optimization of all cards
	string cardNumbers[] = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13" };
	string path;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 13; ++j) {
			path = "Images/cards/"; path += cardTypes[i]; path += cardNumbers[j] + ".bmp";
			cardsImages[j + i * 13] = SDL_LoadBMP(path.c_str());
			cardsImages[j + i * 13] = SDL_ConvertSurface(cardsImages[j + i * 13], gScreenSurface->format, 0);
		}
	}

	//Grid definition
	dealerHandGrid.h = CARD_HEIGHT; dealerHandGrid.w = CARD_WIDTH * 3; dealerHandGrid.x = SCREEN_WIDTH / 2 - dealerHandGrid.w / 2; dealerHandGrid.y = SCREEN_HEIGHT / 5 - dealerHandGrid.h / 2;//()
	playerHandGrid.h = CARD_HEIGHT; playerHandGrid.w = CARD_WIDTH * 3; playerHandGrid.x = SCREEN_WIDTH * 3 / 4 - playerHandGrid.w / 2; playerHandGrid.y = SCREEN_HEIGHT * 4 / 5 - playerHandGrid.h / 2;//()
	AIHandGrid.h = CARD_HEIGHT; AIHandGrid.w = CARD_WIDTH * 3; AIHandGrid.x = SCREEN_WIDTH / 4 - AIHandGrid.w / 2; AIHandGrid.y = SCREEN_HEIGHT * 4 / 5 - AIHandGrid.h / 2;//()
}
void initGraphics(){
	//Create window
	gWindow = SDL_CreateWindow("Blackjack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	loadMedia();
}
void closeGraphics() {
	//Deallocate surface and images
	SDL_FreeSurface(background_image);
	SDL_FreeSurface(cover_grid);
	SDL_FreeSurface(card_back);
	background_image = NULL;
	cover_grid = NULL;
	card_back = NULL;

	for (int i = 0; i < 52; ++i) {
		SDL_FreeSurface(cardsImages[i]);
		cardsImages[i] = NULL;
	}

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void printCard(int x, int y, Card* card) {
	dest.x = x; dest.y = y; dest.w = CARD_WIDTH; dest.h = CARD_HEIGHT;
	SDL_BlitScaled(cardsImages[card->cardIndex()], NULL, gScreenSurface, &dest);
}

//Practical
//General
void printStats(Hand& player, Hand& AI) {
	//Apply the background image
	dest.x = 0; dest.y = 0;
	dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
	SDL_BlitScaled(background_image, NULL, gScreenSurface, &dest);
	
	//money
	
	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(2000);

	//Text
	cout << "The player's money: " << player.getMoney() << "\n";
	cout << "The AI's money: " << AI.getMoney() << "\n";
	cout << "\n";
}

void endGame(int playerMoney, int AIMoney) {
	//Game ended window
	
	//Text
	cout << "Game ended\n" << "Your money: " << playerMoney << "\nThe AI's money: " << AIMoney;
}

//Dealer
void dealerNewGame(Card* reaveled) {
	SDL_BlitScaled(cover_grid, NULL, gScreenSurface, &dealerHandGrid);
	
	int x = dealerHandGrid.x - CARD_WIDTH / 2; int y = dealerHandGrid.y;
	change = dealerHandGrid.w / (2 + 1);
	x += change;
	printCard(x, y, reaveled);

	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(2000);

	x += change;
	dest.x = x; dest.y = y; dest.h = CARD_HEIGHT; dest.w = CARD_WIDTH;
	SDL_BlitScaled(card_back, NULL, gScreenSurface, &dest);

	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(2000);

	/*dest.x = dealerHandGrid.x; dest.y = dealerHandGrid.y; dest.h = CARD_HEIGHT;
	change = dealerHandGrid.w / (2 + 1);
	for (int i = 0; i < 2; ++i) {

	}*/
	//Text
	cout << "The dealer's revealed card is " << reaveled->toString() << "\n";
}

void dealerHand(vector<Card*>& dealer, int dealerSum) {
	SDL_BlitScaled(cover_grid, NULL, gScreenSurface, &dealerHandGrid);

	int x = dealerHandGrid.x - CARD_WIDTH / 2; int y = dealerHandGrid.y;
	change = dealerHandGrid.w / (dealer.size() + 1);
	for (int i = 0; i < dealer.size(); ++i) {
		x += change;
		printCard(x, y, dealer[i]);
		//Update the surface
		SDL_UpdateWindowSurface(gWindow);
		SDL_Delay(2000);
	}

	//Text
	cout << "The dealer's cards: ";
	for (auto x : dealer) {
		cout << x->toString() << " ";
	}
	cout << "\n";
	cout << "This hand sum is: " << dealerSum << "\n\n";
}


//Player
void playerHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet) {
	SDL_BlitScaled(cover_grid, NULL, gScreenSurface, &playerHandGrid);

	int x = playerHandGrid.x - CARD_WIDTH / 2; int y = playerHandGrid.y;
	change = playerHandGrid.w / (hand.size() + 1);
	for (int i = 0; i < hand.size(); ++i) {
		x += change;
		printCard(x, y, hand[i]);
	}

	//Update the surface
	SDL_UpdateWindowSurface(gWindow);

	//Text
	cout << "Current hand: ";
	for (auto x : hand) {
		cout << x->toString() << " ";
	}
	cout << "\n";
	cout << "This hand sum is: " << sumNbet.top()[0] << "\n\n";
}

char playerMove(bool canDouble, bool canSplit) {
	//Text
	stringstream output;
	output << "Press s for stand or h for hit ";

	//If can Double add option
	if (canDouble) {
		output << "or d for double ";
	}

	//If can split add option
	if (canSplit) output << "or p for split ";

	//Print options
	cout << output.str() << "\n";

	char choice;
	//Check a relvant option was selected (ToDo)
	cin >> choice;

	return choice;
}

void playerResult(int bet, int result) {

	SDL_Delay(2000);

	//result 0 lost, 1 tied, 2 won
	//Text
	if(result == 0)
		cout << "Player lost " << bet << " \n";
	if (result == 2)
		cout << "Player won " << bet << " \n";
	if (result == 1)
		cout << "Player didn't win or lose\n";
}

void playerNextHandResult() {
	//Move to the next Splitted hand
}

//AI
void AIHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet) {
	SDL_BlitScaled(cover_grid, NULL, gScreenSurface, &AIHandGrid);

	int x = AIHandGrid.x - CARD_WIDTH / 2; int y = AIHandGrid.y;
	change = AIHandGrid.w / (hand.size() + 1);
	for (int i = 0; i < hand.size(); ++i) {
		x += change;
		printCard(x, y, hand[i]);
	}

	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(2000);

	//Text
	cout << "Current hand: ";
	for (auto x : hand) {
		cout << x->toString() << " ";
	}
	cout << "\n";
	cout << "This hand sum is: " << sumNbet.top()[0] << "\n\n";
}

void AIMove(char choice) {
	//Text
	cout << "The computer has chosen to " << choice << "\n";
}

void AIResult(int bet, int result) {
	SDL_Delay(2000);

	//result 0 lost, 1 tied, 2 won
	//Text
	if (result == 0)
		cout << "AI lost " << bet << " \n";
	if (result == 2)
		cout << "AI won " << bet << " \n";
	if (result == 1)
		cout << "AI didn't win or lose\n";
}

void AINextHandResult() {
	//Move to the next Splitted hand
}
