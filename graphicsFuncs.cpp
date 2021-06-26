#include <iostream>
#include <sstream>

#include "graphicsFuncs.h"

//Technical
//Data
//Images size and placements
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 1080;

const int CARD_WIDTH = 160;
const int CARD_HEIGHT = 220;

const int PLAYER_BUTTON_X = SCREEN_WIDTH * 145 / 200;
const int PLAYER_BUTTON_Y = SCREEN_HEIGHT * 28 / 100;
const int PLAYER_BUTTON_WIDTH = SCREEN_WIDTH * 21 / 100;
const int PLAYER_BUTTON_HEIGHT = SCREEN_HEIGHT * 17 / 200;

const int delayOfPrint = 2000;

SDL_Rect dealerHandGrid; //()
SDL_Rect playerHandGrid; //()
SDL_Rect AIHandGrid; //()

//The window we'll be rendering to
SDL_Window* gWindow;

//The surface contained by the window
SDL_Surface* gScreenSurface;

//Event handler
SDL_Event e;

//The placement of each image printed
SDL_Rect dest;

//The text tools
SDL_Surface* text;
TTF_Font* font;
string st;

//The cmd text
string cmd_text = "";
SDL_Rect dest_cmd_text;

//Images
SDL_Surface* start_screen_image;
SDL_Surface* background_image;
SDL_Surface* card_back;
SDL_Surface* cardsImages[52];

enum playerButton {
	stand_button,
	hit_button,
	double_button,
	split_button,
	totalPlayerButtons
};
SDL_Surface* playerButtons[totalPlayerButtons];
bool playerButtonsActive[totalPlayerButtons];

//Technical Funcs
void loadMedia() {
	//Create window
	gWindow = SDL_CreateWindow("Blackjack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	//Images
	start_screen_image = SDL_LoadBMP("Images/start_screen.bmp");
	background_image = SDL_LoadBMP("Images/blackjack_background.bmp");
	card_back = SDL_LoadBMP("Images/card_back.bmp");

	//Optimize images' upload by formating them
	background_image = SDL_ConvertSurface(background_image, gScreenSurface->format, 0);
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

	//Load and optimization of the player buttons
	playerButtons[stand_button] = SDL_LoadBMP("Images/buttons/stand_button.bmp");
	playerButtons[hit_button] = SDL_LoadBMP("Images/buttons/hit_button.bmp");
	playerButtons[double_button] = SDL_LoadBMP("Images/buttons/double_button.bmp");
	playerButtons[split_button] = SDL_LoadBMP("Images/buttons/split_button.bmp");

	//Font
	font = TTF_OpenFont("font.ttf", 36);

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

	//Init SDL ttf
	TTF_Init();

	loadMedia();
}
void closeGraphics() {
	//Deallocate surface and images
	SDL_FreeSurface(background_image);
	SDL_FreeSurface(card_back);
	background_image = NULL;
	card_back = NULL;

	for (int i = 0; i < 52; ++i) {
		SDL_FreeSurface(cardsImages[i]);
		cardsImages[i] = NULL;
	}

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Destroy text tools
	TTF_CloseFont(font);

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

SDL_Color color = { 0, 0, 0 };
void printText(int x, int y, string input) {
	text = TTF_RenderText_Solid(font, input.c_str(), color);
	dest.x = x; dest.y = y;
	SDL_BlitSurface(text, NULL, gScreenSurface, &dest);
}
void printCard(int x, int y, Card* card) {
	if (card->getValue() == -1) return;
	dest.x = x; dest.y = y; dest.w = CARD_WIDTH; dest.h = CARD_HEIGHT;
	int cardIndex = card->cardIndex();
	SDL_BlitScaled(cardsImages[cardIndex], NULL, gScreenSurface, &dest);
}

//Practical
//DATA
int playerMoney;
int AIMoney;
vector<Card*> dealer_hand; int dealerSum;
vector<Card*> player_hand; int indPlayer; int playerSum;
vector<Card*> AI_hand; int indAI; int AISum;

//Funcs
//General
void startScreen() {
	//Apply the background image
	dest.x = 0; dest.y = 0;
	dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
	SDL_BlitScaled(start_screen_image, NULL, gScreenSurface, &dest);
	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
	bool flag = true;
	while (flag) {
		SDL_PollEvent(&e) != 0;
		if (e.type == SDL_KEYDOWN) flag = false;
	}
}

void printBoard() {
	//Apply the background image
	dest.x = 0; dest.y = 0;
	dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
	SDL_BlitScaled(background_image, NULL, gScreenSurface, &dest);

	//Apply the dealerHand
	int x = dealerHandGrid.x - CARD_WIDTH / 2; int y = dealerHandGrid.y;
	int change;
	if (dealer_hand.size() == 1) {
		change = dealerHandGrid.w / (2 + 1);
		x += change;
		printCard(x, y, dealer_hand[0]);

		x += change;
		dest.x = x; dest.y = y; dest.h = CARD_HEIGHT; dest.w = CARD_WIDTH;
		SDL_BlitScaled(card_back, NULL, gScreenSurface, &dest);
	}
	else {
		change = dealerHandGrid.w / (dealer_hand.size() + 1);
		for (int i = 0; i < dealer_hand.size(); ++i) {
			x += change;
			printCard(x, y, dealer_hand[i]);
		}
	}
	//Apply the dealer's hand sum
	if (!dealerSum == 0) {
		st = "Total: " + to_string(dealerSum);
		printText(dealerHandGrid.x + dealerHandGrid.w * 40 / 100, dealerHandGrid.y + dealerHandGrid.h + 10, st);
	}

	//Apply the player Hand
	x = playerHandGrid.x - CARD_WIDTH / 2; y = playerHandGrid.y;
	change = playerHandGrid.w / (player_hand.size() + 1);
	for (int i = 0; i < player_hand.size(); ++i) {
		x += change;
		printCard(x, y, player_hand[i]);
	}
	//Apply the player's hand sum
	if (!playerSum == 0) {
		st = "Total: " + to_string(playerSum);
		printText(playerHandGrid.x + playerHandGrid.w * 40 / 100, playerHandGrid.y + playerHandGrid.h + 10, st);
	}
	//Apply the player's money
	st = to_string(playerMoney);
	printText(SCREEN_WIDTH * 84 / 100, SCREEN_HEIGHT * 15 / 100, st);

	//Apply the AI Hand
	x = AIHandGrid.x - CARD_WIDTH / 2; y = AIHandGrid.y;
	change = AIHandGrid.w / (AI_hand.size() + 1);
	for (int i = 0; i < AI_hand.size(); ++i) {
		x += change;
		printCard(x, y, AI_hand[i]);
	}
	//Apply the AI's hand sum
	if (!AISum == 0) {
		st = "Total: " + to_string(AISum);
		printText(AIHandGrid.x + AIHandGrid.w * 40 / 100, AIHandGrid.y + AIHandGrid.h + 10, st);
	}
	//Apply the AI's money
	st = to_string(AIMoney);
	printText(SCREEN_WIDTH * 15 / 100, SCREEN_HEIGHT * 15 / 100, st);

	//Apply the active player's buttons
	//The buttonsPlacement
	dest.x = PLAYER_BUTTON_X; dest.y = PLAYER_BUTTON_Y;
	dest.w = PLAYER_BUTTON_WIDTH; dest.h = PLAYER_BUTTON_HEIGHT;
	if(playerButtonsActive[stand_button]) SDL_BlitScaled(playerButtons[stand_button], NULL, gScreenSurface, &dest);
	dest.y += PLAYER_BUTTON_HEIGHT;
	if (playerButtonsActive[hit_button]) SDL_BlitScaled(playerButtons[hit_button], NULL, gScreenSurface, &dest);
	dest.y += PLAYER_BUTTON_HEIGHT;
	if (playerButtonsActive[double_button]) SDL_BlitScaled(playerButtons[double_button], NULL, gScreenSurface, &dest);
	dest.y += PLAYER_BUTTON_HEIGHT;
	if (playerButtonsActive[split_button]) SDL_BlitScaled(playerButtons[split_button], NULL, gScreenSurface, &dest);

	//print the cmd text
	if (cmd_text.compare("")) printText(dest_cmd_text.x, dest_cmd_text.y, cmd_text);
	cmd_text = "";

	//Update the surface
	SDL_UpdateWindowSurface(gWindow);
}
void newRound(int pMoney, int aiMoney) {
	playerMoney = pMoney;
	AIMoney = aiMoney;

	dealer_hand.clear(); dealerSum = 0;
	player_hand.clear(); indPlayer = 0; playerSum = 0;
	AI_hand.clear(); indAI = 0; AISum = 0;

	printBoard();
}
void endGame() {
	//Game ended window
}

//Dealer
void dealerNewGame(Card* reaveled) {
	dealerSum = reaveled->getValue();

	dealer_hand.push_back(reaveled);
	printBoard();
	SDL_Delay(delayOfPrint);
}

void dealerHand(vector<Card*>& dealer, int sum) {
	int ind = 1; //The first card is already inside
	for (; ind < dealer.size(); ++ind) {
		dealer_hand.push_back(dealer[ind]);
		dealerSum += dealer[ind]->getValue();
		printBoard();
		SDL_Delay(delayOfPrint);
	}
}

//Player
void playerHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet, int money) {
	for (; indPlayer < hand.size(); ++indPlayer)
		player_hand.push_back(hand[indPlayer]);

	playerMoney = money;
	playerSum = sumNbet.top()[0];

	printBoard();
}
void playerSplit() {
	indPlayer = 1;
	player_hand.erase(player_hand.begin() + player_hand.size() - 1);
}

void playerNextHand() {
	indPlayer = 0;
	player_hand.push_back(new Card(-1, '0'));
}

char playerMove(bool canDouble, bool canSplit) {
	//Activate the relavent player buttons
	playerButtonsActive[stand_button] = true;
	playerButtonsActive[hit_button] = true;
	if(canDouble) playerButtonsActive[double_button] = true;
	if (canSplit) playerButtonsActive[split_button] = true;
	printBoard();

	char choice = NULL;
	//Empty the event that happend before the loop
	while (SDL_PollEvent(&e) != 0) SDL_PollEvent(&e);
	while (choice == NULL){
		//Handle events on queue
		if (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_KEYDOWN) {
				//Select surfaces based on key press
				switch (e.key.keysym.sym) {
					//Stand
				case SDLK_s:
					choice = 's';
					break;

					//Hit
				case SDLK_h:
					choice = 'h';
					break;

					//Double
				case SDLK_d:
					if (canDouble) {
						choice = 'd'; break;
					}

					//Split
				case SDLK_p:
					if (canSplit) {
						choice = 'p';
						break;
					}
				}
			}
		}
	}

	//Deactivate the player buttons on the screen
	for (int i = 0; i < totalPlayerButtons; ++i) playerButtonsActive[i] = false;
	printBoard();

	return choice;
}

void playerBlackjack(int money) {
	//Print with message
	cmd_text = "Blackjack!!";
	dest_cmd_text.x = playerHandGrid.x + playerHandGrid.w * 15 / 100; dest_cmd_text.y = playerHandGrid.y - 50;
	printBoard();
	SDL_Delay(delayOfPrint);
	//Print without message
	printBoard();
	SDL_Delay(delayOfPrint);
}

void playerResult(int bet, int result, int money) {
	playerMoney = money;
	switch (result) {
	case 0:
		st = "Player doesn't profit";
		break;
	
	case 2:
		st = "Player wins " + to_string(bet * 2);
		break;

	case 1:
		st = "Tie! player gets his bet back (" + to_string(bet) + ")";
	}
	//Update the cmd text accordingly
	cmd_text = st;
	dest_cmd_text.x = playerHandGrid.x + playerHandGrid.w * 15 / 100; dest_cmd_text.y = playerHandGrid.y - 50;
	printBoard();
	SDL_Delay(delayOfPrint);
	printBoard();
	SDL_Delay(delayOfPrint);

	//result 0 lost, 1 tied, 2 won
}

//AI
void AIHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet, int money) {
	AIMoney = money;
	SDL_Delay(delayOfPrint);

	AISum = sumNbet.top()[0];

	for (; indAI < hand.size(); ++indAI) {
		AI_hand.push_back(hand[indAI]);
		printBoard();
		SDL_Delay(delayOfPrint);
	}
}

void AISplit() {
	indAI = 1;
	AI_hand.erase(AI_hand.begin() + AI_hand.size() - 1);
}

void AINextHand() {
	indAI = 0;
	AI_hand.push_back(new Card(-1, '0'));
}

void AIMove(char choice) {

}

void AIBlackjack(int money) {
	//Print with message
	cmd_text = "Blackjack!!";
	dest_cmd_text.x = AIHandGrid.x + AIHandGrid.w * 15 / 100; dest_cmd_text.y = AIHandGrid.y - 50;
	printBoard();
	SDL_Delay(delayOfPrint);
	//Print without message
	printBoard();
	SDL_Delay(delayOfPrint);
}

void AIResult(int bet, int result, int money) {
	AIMoney = money;
	switch (result) {
	case 0:
		st = "AI doesn't profit";
		break;

	case 2:
		st = "AI wins " + to_string(bet * 2);
		break;

	case 1:
		st = "Tie! AI gets his bet back (" + to_string(bet) + ")";
	}
	//Update the cmd text accordingly
	cmd_text = st;
	dest_cmd_text.x = AIHandGrid.x + AIHandGrid.w * 15 / 100; dest_cmd_text.y = AIHandGrid.y - 50;
	printBoard();
	SDL_Delay(delayOfPrint);
	printBoard();
	SDL_Delay(delayOfPrint);
	printBoard();
	SDL_Delay(2000);

	//result 0 lost, 1 tied, 2 won
}

