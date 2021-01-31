#pragma once

//Data
//for 10000 the standard deviation is 0.015, which is precise enough and yet the runtime is not too long
const int timesWithMinCoeffcient = 10000;

//Ideal generationSize is 
const int generationSize = 100;

//Ideal tourney is
const int tourney = 3;

//Ideal crossover rate is 
//(99 is for saving only the best chromo)
const float crossoverPrecRate = 99;
//The survivors between every generation
const int survivorsAmount = (int)((float)generationSize * (100 - crossoverPrecRate) / 100);
//Caculate how many chromo will be generated next generation for each father (tourney)
const int fatherBreedingSize = (generationSize - survivorsAmount) / tourney;

//Ideal mutation rate is
const float mutationPrecRate = 0;


//Functions
void machineLearning();
