#ifndef BFS_DFS_H
#define BFS_DFS_H

#include "common.h"

// Структура для классификации рёбер DFS-леса
struct DFSEdge {
    int from;
    int to;
    string type; // TREE, BACK, FORWARD, CROSS
};

// Функция создания тестового графа
vector<vector<int>> createTestGraph();

// Алгоритм BFS (возвращает порядок обхода)
vector<int> bfs(const vector<vector<int>>& graph, int start);

// Алгоритм DFS (возвращает порядок обхода)
vector<int> dfs(const vector<vector<int>>& graph, int start);

// Построение DFS-леса (возвращает список рёбер и кол-во компонент)
pair<vector<DFSEdge>, int> buildDFSForeset(const vector<vector<int>>& graph);

void runBFS_DFS();

#endif