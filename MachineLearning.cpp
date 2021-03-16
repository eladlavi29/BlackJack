#include "MachineLearning.h"
#include "Generation.h"
#include "DM.h"

#include "practicalFuncs.h"
#include <iostream>

using namespace std;

DM* machineLearning() {
	DM** bestDMs = new DM*[48]; int count = 1; 
	for (int i = 0; i < 48; ++i) {
		bestDMs[i] = new DM();
	}
	
	bool flag = true;
	Generation generation;
	bestDMs[count] = generation.bestChromo(); ++count;
	while (flag) {
		generation.lifetime();

		bestDMs[count]->copy(generation.bestChromo()); ++count;

		if (generation.stopEvoloution()) {
			flag = false;
		}
	}

	system("cls");
	cout << "\nMachine Learning process ended \n";
	generation.printData();
	
	int x; cin >> x;
	while (x != 1) {
		cout << bestDMs[x]->getWinningExpectation() << "\n\n";
		bestDMs[x]->printData(); cout << "\n\n";
		cin >> x;
	}

	DM* ultimateDM = generation.bestChromo();
	return ultimateDM;
}