#pragma once

//Data
//for 10000 the standard deviation is 0.015, which is precise enough and yet the runtime is not too long
const int timesWithMinCoeffcient = 10000;

//Ideal generationSize is 
const int generationSize = 100;

//Ideal tourney is
//tourney can't be 1 or larger then the generationSize
const int tourney = 5;

//Ideal crossover rate is 
const float crossoverPrecRate = 100;
//The survivors between every generation
const int survivorsAmount = (int)((float)generationSize * (100 - crossoverPrecRate) / 100);

//Ideal mutation rate is
const float mutationPrecRate = 0;

//Cant be bigger then the 100 - crossoverPrecRate
const float elitisimPrecRate = 0; 

//Functions
void machineLearning();
