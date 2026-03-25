#pragma once
#include <vector>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>

using namespace std;
using namespace chrono;

struct Clause {
    vector<int> literals;
};

struct CNFFormula {
    int numVars = 0;
    vector<Clause> clauses;
};

extern int dpllSteps; // extern для разных файлов
void simplify(CNFFormula& f, int var, int value);
bool unitPropagation(CNFFormula& f, vector<int>& assignment);
bool DPLLRecursive(CNFFormula f, vector<int>& assignment);
bool runDPLL(CNFFormula f, vector<int>& assignment);
bool DPLL_DP(CNFFormula f, vector<int>& assignment);
size_t hashCNF(const CNFFormula& f);
bool runDPLLDP(CNFFormula f, vector<int>& assignment);
