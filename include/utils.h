#pragma once 

#include <ctime>
#include <random>

double const pi = 3.1415926535; 

class RandomGenerator {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

public:
    RandomGenerator(double low, double high);

    double operator()(); 
};