#include <iostream>
#include "MachineLearning.h"
#include "Game.h"
#include "graphicsFuncs.h"

using namespace std;

int main(int argc, char* args[]) {

	//The machine learning method
	//DM* bestDM = machineLearning();

	//Set the Game
	DM* ultimateDM = new DM(); Game game(100); 
	ultimateDM->setAI();

	//Set the graphics
	initGraphics();

	//Starting screen
	startScreen();

	game.runGame(*ultimateDM);

	//Close Game
	closeGraphics();

	return 0;
}
