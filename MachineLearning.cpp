#include "MachineLearning.h"
#include "Generation.h"
#include "practicalFuncs.h"
#include <iostream>

using namespace std;

void machineLearning() {

	/*Generation generation;
	generation.check();*/

	bool flag = true;
	Generation generation;
	while (flag) {

		generation.lifetime();

		if (generation.stopEvoloution()) {
			flag = false;
		}
	}

	system("cls");
	cout << "\nMachine Learning process ended \n";
	generation.printData();
}