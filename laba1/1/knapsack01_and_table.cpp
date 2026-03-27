#include "knap.h"

// Вспомогательная функция вывода таблицы сложности (всегда полезна)
void printComplexity(string name, int n, int W, string space) {
    cout << "\nСложность алгоритма " << name << ":\n";
    cout << "Временная: O(n * W) = O(" << n << " * " << W << ")\n";
    cout << "Память: " << space << "\n";
}

// Вывод DP-таблицы
void printDPTable(const vector<vector<int>>& dp, int n, int W) {
    cout << "\nDP-таблица:\n";
    cout << setw(5) << "w\\i";
    for (int i = 0; i <= n; ++i) cout << setw(5) << i;
    cout << endl;

    for (int weight = 0; weight <= W; ++weight) {
        cout << setw(5) << weight;
        for (int i = 0; i <= n; ++i) {
            cout << setw(5) << dp[i][weight];
        }
        cout << endl;
    }
}

// Классическая 2D-таблица
vector<vector<int>> knapsack01_2D(int W, const vector<int>& w, const vector<int>& v, bool showTable) {
    int n = w.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int weight = 0; weight <= W; ++weight) {
            // Влезает ли предмет весом w[i - 1] в ячейку с вместимостью weight
            if (w[i - 1] <= weight) {
                dp[i][weight] = max(dp[i - 1][weight], dp[i - 1][weight - w[i - 1]] + v[i - 1]);
            }
            else {
                dp[i][weight] = dp[i - 1][weight]; // Просто копируем значение из строчки выше
            }
        }
    }

    return dp;
}

// Оптимизированная 1D-таблица
Knapsack1DResult knapsack01_1D(int W, const vector<int>& w, const vector<int>& v, bool showTable) {
    int n = w.size();
    vector<int> dp(W + 1, 0); // Всего W + 1 ячейка, а не N * W (как в 2D реализации)
    vector<vector<int>> table;

    if (showTable) table.resize(n, vector<int>(W + 1, 0));

    for (int i = 0; i < n; ++i) {
        // Идём задом напрёд чтобы исключить использование уже обновлённой ячейки (наслоение) 
        for (int weight = W; weight >= w[i]; --weight) {
            // Перезписываем ячейку, если взять предмет выгоднее, чем не брать его (и не храним ненужной информации как в 2D)
            dp[weight] = max(dp[weight], dp[weight - w[i]] + v[i]);
        }
        if (showTable) table[i] = dp;
    }

    return { dp[W], dp, table };
}

// Неограниченный рюкзак
KnapsackUnboundedResult unboundedKnapsack1D(int W, const vector<int>& w, const vector<int>& v, bool showTable) {
    int n = w.size();
    vector<int> dp(W + 1, 0);
    vector<vector<int>> table;

    if (showTable) table.resize(n, vector<int>(W + 1, 0));

    for (int i = 0; i < n; ++i) {
        // Идем ОТ начала К концу
        for (int weight = w[i]; weight <= W; ++weight) {
            dp[weight] = max(dp[weight], dp[weight - w[i]] + v[i]); // значение dp[weight - w[i]] уже обновлено на этой же итерации
        }
        if (showTable) table[i] = dp;
    }

    return { dp[W], dp, table };
}