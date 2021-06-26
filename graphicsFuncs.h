#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "Hand.h"
using namespace std;

//Technical Funcs
void loadMedia();
void initGraphics();
void closeGraphics();

void printText(int x, int y, string input);
void printCard(int x, int y, Card* card);

//Practical Funcs
void startScreen();

void printBoard();
void newRound(int pMoney, int aiMoney);
void endGame();

void dealerNewGame(Card* reaveled);
void dealerHand(vector<Card*>& dealer, int dealerSum);

void playerHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet, int money);
void playerSplit();
void playerNextHand();
char playerMove(bool canDouble, bool canSplit);
void playerBlackjack(int money);
void playerResult(int bet, int result, int money);

void AIHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet, int money);
void AISplit();
void AINextHand();
void AIMove(char choice);
void AIBlackjack(int money);
void AIResult(int bet, int result, int money);

