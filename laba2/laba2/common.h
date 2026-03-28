#ifndef COMMON_H
#define COMMON_H

// Защита от макросов min/max из windows.h
#define NOMINMAX 

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <queue>
#include <stack>
#include <iomanip>

// Настройки для Windows
#ifdef _WIN32
#include <windows.h>

// Функция установки кодировки CP1251 (стандартная для русской Windows)
inline void setupConsoleEncoding() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}
#define SET_CONSOLE_ENCODING() setupConsoleEncoding()
#else
    // Для Linux/macOS
#include <locale>
inline void setupConsoleEncoding() {
    try { std::locale::global(std::locale("")); }
    catch (...) {}
}
#define SET_CONSOLE_ENCODING() setupConsoleEncoding()
#endif

using namespace std;
using namespace std::chrono;

// Объявления функций (чтобы main видел их)
void runBFS_DFS();
void runKosaraju();
void runKruskal_Prim();
void runBellman_Moore();

int getSafeInt();

#endif // COMMON_H