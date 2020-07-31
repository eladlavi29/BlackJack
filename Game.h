#pragma once
#include <vector>
#include "Hand.h"
using namespace std;

class Game{
private:
	Deck deck;
	Hand hand;
	Dealer dealer;
public:
	Game(int money);
	void run();
};

