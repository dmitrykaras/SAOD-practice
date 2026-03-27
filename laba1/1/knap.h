#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

// Структуры нужны только для вывода данных
struct Knapsack1DResult {
    int maxValue;
    vector<int> dp;
    vector<vector<int>> table;
};

struct KnapsackUnboundedResult {
    int maxValue;
    vector<int> dp;
    vector<vector<int>> table;
};

// Прототипы функций
vector<vector<int>> knapsack01_2D(int W, const vector<int>& w, const vector<int>& v, bool showTable);
Knapsack1DResult knapsack01_1D(int W, const vector<int>& w, const vector<int>& v, bool showTable);
KnapsackUnboundedResult unboundedKnapsack1D(int W, const vector<int>& w, const vector<int>& v, bool showTable);
void printDPTable(const vector<vector<int>>& dp, int n, int W);
void printComplexity(string name, int n, int W, string space);