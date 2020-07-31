#include "Game.h"
using namespace std;

void useLiveTextInConsoleExample() {
	int count = 0;
	while (true) {
		++count;
		cout << count;
		system("cls");
	}
}


float calculateSD(float*& data, int dataSize, float sum) {
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

	return (float)game.rankDMforCalc(dm, rep);

	/*float* data = new float[num];
	float sum = 0.0;
	for (int i = 0; i < num; ++i) {
		data[i] = game.rankDMforCalc(dm, rep);
		sum += data[i];
	}

	float value = calculateSD(data, num, sum) / (float)rep;
	delete data;

	return value;*/
}
