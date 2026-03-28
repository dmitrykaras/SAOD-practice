#include "common.h"
#include <fstream>
#include <sstream>

// Загрузка графа из файла
vector<vector<int>> loadGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << filename << endl;
        return {};
    }

    int n, m;
    if (!(file >> n >> m)) {
        cout << "Ошибка формата файла: не удалось прочитать количество вершин и ребер." << endl;
        return {};
    }

    if (n <= 0) {
        cout << "Ошибка: количество вершин должно быть положительным." << endl;
        return {};
    }

    vector<vector<int>> graph(n);
    int u, v;
    for (int i = 0; i < m; ++i) {
        if (file >> u >> v) {
            if (u >= 0 && u < n && v >= 0 && v < n) {
                graph[u].push_back(v);
                graph[v].push_back(u);
            }
            else {
                cout << "Предупреждение: пропущено ребро (" << u << ", " << v << ") - неверные индексы." << endl;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        sort(graph[i].begin(), graph[i].end());
    }

    cout << "Граф успешно загружен из файла \"" << filename << "\"." << endl;
    cout << "Вершин: " << n << ", Ребер: " << m << endl;
    return graph;
}

// Ручной ввод графа
vector<vector<int>> inputGraphManually() {
    int n, m;
    cout << "Введите количество вершин: ";
    n = getSafeInt();

    if (n <= 0) {
        cout << "Количество вершин должно быть больше нуля." << endl;
        return {};
    }

    cout << "Введите количество ребер: ";
    m = getSafeInt();

    vector<vector<int>> graph(n);
    cout << "Введите ребра (по два числа в строке: номер_вершины_1 номер_вершины_2):" << endl;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cout << "Ребро " << (i + 1) << ": ";
        cin >> u >> v;
        if (u >= 0 && u < n && v >= 0 && v < n) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        else {
            cout << "Ошибка: вершины должны быть в диапазоне от 0 до " << (n - 1) << ". Ребро пропущено." << endl;
            --i;
        }
    }

    for (int i = 0; i < n; ++i) {
        sort(graph[i].begin(), graph[i].end());
    }

    return graph;
}

// Вывод графа в консоль
void printGraph(const vector<vector<int>>& graph) {
    cout << "\nПредставление графа (списки смежности):" << endl;
    for (size_t i = 0; i < graph.size(); ++i) {
        cout << "Вершина " << i << ": ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Выбор графа (меню)
vector<vector<int>> selectGraph() {
    vector<vector<int>> graph;

    while (true) {
        cout << "\nВыбор источника графа" << endl;
        cout << "1. Маленький граф (5 вершин)" << endl;
        cout << "2. Средний граф (10 вершин)" << endl;
        cout << "3. Большой граф (20 вершин)" << endl;
        cout << "4. Ввести граф вручную" << endl;
        cout << "0. Отмена" << endl;
        cout << "Ваш выбор: ";

        int choice = getSafeInt();

        if (choice == 0) return {};

        string filename;
        if (choice == 1) filename = "data/graph_small.txt";
        else if (choice == 2) filename = "data/graph_medium.txt";
        else if (choice == 3) filename = "data/graph_large.txt";
        else if (choice == 4) {
            graph = inputGraphManually();
            if (!graph.empty()) return graph;
            continue;
        }
        else {
            cout << "Неверный выбор. Попробуйте снова." << endl;
            continue;
        }

        if (choice >= 1 && choice <= 3) {
            graph = loadGraphFromFile(filename);
            if (!graph.empty()) return graph;
        }

        cout << "Попробуйте выбрать другой вариант." << endl;
    }
}

// Структура для ребра DFS-леса
struct DFSEdge {
    int from;
    int to;
    string type;
};

// 1. BFS (Обход в ширину)
vector<int> bfs(const vector<vector<int>>& graph, int start) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<int> order;
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        for (int to : graph[v]) {
            if (!visited[to]) {
                visited[to] = true;
                q.push(to);
            }
        }
    }
    return order;
}

// 2. DFS (Обход в глубину) - вспомогательная рекурсивная функция
void dfsRecursive(const vector<vector<int>>& graph, int v,
    vector<bool>& visited, vector<int>& order) {
    visited[v] = true;
    order.push_back(v);

    for (int to : graph[v]) {
        if (!visited[to]) {
            dfsRecursive(graph, to, visited, order);
        }
    }
}

vector<int> dfs(const vector<vector<int>>& graph, int start) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<int> order;
    dfsRecursive(graph, start, visited, order);
    return order;
}

// 3. Построение DFS-леса с классификацией ребер
void dfsForestRecursive(const vector<vector<int>>& graph, int v,
    vector<bool>& visited, vector<int>& parent,
    vector<int>& tin, vector<int>& tout, int& timer,
    vector<DFSEdge>& edges) {
    visited[v] = true;
    tin[v] = ++timer;

    for (int to : graph[v]) {
        if (to == parent[v]) continue;

        if (visited[to]) {
            if (tout[to] == 0) {
                edges.push_back({ v, to, "BACK" });
            }
            else if (tin[v] < tin[to]) {
                edges.push_back({ v, to, "FORWARD" });
            }
            else {
                edges.push_back({ v, to, "CROSS" });
            }
        }
        else {
            parent[to] = v;
            edges.push_back({ v, to, "TREE" });
            dfsForestRecursive(graph, to, visited, parent, tin, tout, timer, edges);
        }
    }
    tout[v] = ++timer;
}

pair<vector<DFSEdge>, int> buildDFSForeset(const vector<vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    vector<int> tin(n, 0), tout(n, 0);
    int timer = 0;
    vector<DFSEdge> edges;
    int components = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            components++;
            cout << "\nКомпонента связности #" << components << " (начало с вершины " << i << ")" << endl;
            dfsForestRecursive(graph, i, visited, parent, tin, tout, timer, edges);
        }
    }
    return { edges, components };
}

// Основная функция задания
void runBFS_DFS() {
    SET_CONSOLE_ENCODING();

    cout << "\nОбходы графа: BFS, DFS, построение DFS-леса" << endl;

    // Выбор графа
    vector<vector<int>> graph = selectGraph();
    if (graph.empty()) {
        cout << "Возврат в главное меню." << endl;
        return;
    }

    printGraph(graph);

    // Выбор стартовой вершины
    int start = 0;
    int maxVertex = static_cast<int>(graph.size()) - 1;
    cout << "\nВведите стартовую вершину для обхода (0-" << maxVertex << "): ";
    int inputStart = getSafeInt();
    if (inputStart >= 0 && inputStart <= maxVertex) {
        start = inputStart;
    }
    else {
        cout << "Неверная вершина, используем 0." << endl;
    }

    // 1. Тестирование BFS
    cout << "\n1. BFS (Обход в ширину)" << endl;

    auto t1 = high_resolution_clock::now();
    vector<int> bfsOrder = bfs(graph, start);
    auto t2 = high_resolution_clock::now();

    cout << "Порядок обхода: ";
    for (size_t i = 0; i < bfsOrder.size(); ++i) {
        cout << bfsOrder[i];
        if (i < bfsOrder.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Время выполнения: " << duration_cast<microseconds>(t2 - t1).count() << " мкс" << endl;

    // 2. Тестирование DFS
    cout << "\n2. DFS (Обход в глубину)" << endl;

    t1 = high_resolution_clock::now();
    vector<int> dfsOrder = dfs(graph, start);
    t2 = high_resolution_clock::now();

    cout << "Порядок обхода: ";
    for (size_t i = 0; i < dfsOrder.size(); ++i) {
        cout << dfsOrder[i];
        if (i < dfsOrder.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Время выполнения: " << duration_cast<microseconds>(t2 - t1).count() << " мкс" << endl;

    // 3. Построение DFS-леса
    cout << "\n3. Построение DFS-леса и классификация ребер" << endl;

    t1 = high_resolution_clock::now();
    auto result = buildDFSForeset(graph);
    auto edges = result.first;
    int components = result.second;
    t2 = high_resolution_clock::now();

    cout << "Всего компонент связности: " << components << endl;
    cout << "\nКлассификация ребер:" << endl;

    int treeCount = 0, backCount = 0, fwdCount = 0, crossCount = 0;
    for (const auto& e : edges) {
        cout << "  " << e.from << " -> " << e.to << " (" << e.type << ")" << endl;
        if (e.type == "TREE") treeCount++;
        else if (e.type == "BACK") backCount++;
        else if (e.type == "FORWARD") fwdCount++;
        else if (e.type == "CROSS") crossCount++;
    }

    cout << "\nИтого:" << endl;
    cout << "  Дерево (TREE):      " << treeCount << endl;
    cout << "  Обратные (BACK):    " << backCount << endl;
    cout << "  Прямые (FORWARD):   " << fwdCount << endl;
    cout << "  Поперечные (CROSS): " << crossCount << endl;
    cout << "Время выполнения: " << duration_cast<microseconds>(t2 - t1).count() << " мкс" << endl;

    cout << "\nРабота завершена." << endl;
}