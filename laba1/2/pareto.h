#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

struct Laptop {
    int id;
    string model;
    double price;
    double weight;
    double ram;
    double cpu_freq;
    int pareto_layer = 0;
};

bool isDominating(const Laptop& a, const Laptop& b);
void computeParetoLayers(vector<Laptop>& data);
void computeParetoLayersOptimized(vector<Laptop>& data);