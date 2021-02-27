#include "Game.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

void useLiveTextInConsoleExample() {
	int count = 0;
	while (true) {
		++count;
		cout << count;
		system("cls");
	}
}

//Random Function
bool flagRandom = true;
unsigned long random() {
	if (flagRandom) srand(time(NULL));
	flagRandom = false;
	return rand();
}
//Choose one of two obj randomly coresponding to values
int dependentPick(char firstValue, int firstPower, char secondValue, int secondPower) {
	//Two of the powers are negative so to fix it:
	firstPower = abs(firstPower); secondPower = abs(secondPower);
	int temp = firstPower; firstPower = secondPower; secondPower = temp;

	int rand = random() % (firstPower + secondPower);
	if (rand < firstPower) return firstValue;
	return secondValue;
}

float calculateSD(float*& data, int dataSize, float sum) {
	//חשב סטיית תקן
	float standardDeviation = 0.0;

	float mean = sum / dataSize;

	for (int i = 0; i < dataSize; ++i)
		standardDeviation += pow(data[i] - mean, 2);

	return sqrt(standardDeviation / dataSize);
}

float calcCoefficientOfVariation(int rep, int num) {
	DM dm;
	dm.generate();

	Game game(0);

	float* data = new float[num];
	float sum = 0.0;
	for (int i = 0; i < num; ++i) {
		data[i] = game.rankDMforCalc(dm, rep);
		sum += data[i];

		dm.setFitness(0);
	}

	float value = calculateSD(data, num, sum);
	delete []data;

	return value;
}

//for 10000 the sd is 0.01, which is precise enough yet not too long
void findIdealRepsAmount() {
	int repCheck[] = {100, 1000, 10000, 100000};
	int rep; int checkSD;
	for (int i = 0; i < sizeof(repCheck) / sizeof(repCheck[0]); ++i) {
		rep = repCheck[i]; checkSD = 1e6 / rep;
		cout << "For rep = " << rep << " (checkSD = " << checkSD << "): ";
		cout << calcCoefficientOfVariation(rep, checkSD) << "\n";
	}
}

void checkCrossover() {
	DM dm1, dm2;
	dm1.generate(); dm2.generate();
	dm1.setFitness(-1); dm2.setFitness(-4);
	DM dm3 = *(dm1.crossover(&dm2));

	int count1 = 0, count2 = 0, common = 0;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 17; ++j) {
			if (dm1.getMove(0, i, j) == dm2.getMove(0, i, j)) ++common;
			if (dm1.getMove(0, i, j) == dm3.getMove(0, i, j)) ++count1;
			else {
				if (dm2.getMove(0, i, j) == dm3.getMove(0, i, j)) ++count2;
				else {
					cout << 0 << " " << i << " " << j;
					cout << dm1.getMove(0, i, j) << " " << dm2.getMove(0, i, j) << " " << dm3.getMove(0, i, j);
				}
			}
		}
	}
	cout << count1 << " " << count2 << " " << common << "\n";
	dm1.printData(); cout << "\n"; dm2.printData(); cout << "\n"; dm3.printData();
}

//template <typename T>
//inline void deleteVector(vector<T*> vec) {
//	for (auto x : vec) delete x;
//	vec.shrink_to_fit();
//}



