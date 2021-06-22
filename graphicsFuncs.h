#pragma once
#include <SDL.h>

#include "Hand.h"
using namespace std;

//Technical Funcs
void loadMedia();
void initGraphics();
void closeGraphics();

void printCard(int x, int y, Card* card);

//Practical Funcs
void printStats(Hand& player, Hand& AI);
void endGame(int playerMoney, int AIMoney);

void dealerNewGame(Card* reaveled);
void dealerHand(vector<Card*>& dealer, int dealerSum);

void playerHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet);
char playerMove(bool canDouble, bool canSplit);
void playerResult(int bet, int result);
void playerNextHandResult();

void AIHand(vector<Card*>& hand, stack<array<int, 2>>& sumNbet);
void AIMove(char choice);
void AIResult(int bet, int result);
void AINextHandResult();