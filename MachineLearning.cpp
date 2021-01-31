#include "MachineLearning.h"
#include "Generation.h"
#include "practicalFuncs.h"
#include <iostream>

using namespace std;

void machineLearning() {
	bool flag = true;
	Generation generation;
	while (flag) {

		generation.lifetime();

		if (generation.stopEvoloution()) {
			flag = false;
		}
	}

	cout << "\nMachine Learning process ended \n";
	generation.printData();
}