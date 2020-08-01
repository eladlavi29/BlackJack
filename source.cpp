#include <iostream>
#include <vector>
#include "Game.h"

using namespace std;

int main() {
	DM dm;
	dm.generate();

	Game game(0);
	game.rankDM(dm);

	//cout << dm.getWinningExpectation() << "\n";

	//cout << calcCoefficientOfVariation(100000, 100);

	return 0;
}