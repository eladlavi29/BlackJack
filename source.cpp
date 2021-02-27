#include <iostream>
#include "MachineLearning.h"

using namespace std;

#include <chrono>
#include "Game.h"

int main() {
	
	Game game(0); DM dm; dm.generate();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	game.rankDM(dm);	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

	machineLearning();

	return 0;
}
