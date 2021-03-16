#pragma once

//Data
//for 10000 the standard deviation is about 0.012, which is precise enough and yet the runtime is not too long
const int timesWithMinCoeffcient = 10000;

//Ideal generationSize is 400
const int generationSize = 400;

//Ideal tourney is 4
//tourney can't be 1 or larger then the generationSize
const int tourney = 4;

//Ideal crossover rate is 99
const float crossoverPrecRate = 99;
//The survivors between every generation
const int survivorsAmount = (int)((float)generationSize * (100 - crossoverPrecRate) / 100);

//Ideal mutation rate is 0
const float mutationPrecRate = 0;

//Cant be bigger then the 100 - crossoverPrecRate
const float elitisimPrecRate = 100 - crossoverPrecRate;

//Functions
#include "DM.h"
DM* machineLearning();
