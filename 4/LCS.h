#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <clocale> 


using namespace std;
using namespace chrono;


// pair для вывода нескольких значений в return
pair<int, string> longest_common_subsequence(const string& str1, const string& str2);
pair<int, string> lcs_recursive_helper(const string& str1, const string& str2, int i, int j);
pair<int, string> common_lcs(const string& str1, const string& str2);

int LCS();