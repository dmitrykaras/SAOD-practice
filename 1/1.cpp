#include "knap.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    bool run = true;
    while (run) {
        cout << "Задача о рюкзаке (Knapsack Problem)\n";
        cout << "1. Загрузить тест из файла\n";
        cout << "2. Ввести данные вручную\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";

        int mode;
        if (!(cin >> mode)) {
            cin.clear();
            cin.ignore(MAXSHORT, '\n');
            cout << "Ошибка ввода! Введите число.\n";
            continue;
        }

        if (mode == 0) {
            run = false;
            cout << "Завершение работы...\n";
            continue;
        }

        int n = 0, W = 0;
        vector<int> w, v;
        bool showLog = false;

        if (mode == 1) {
            cout << "\nВыберите файл:\n1. Малый (10 предм.)\n2. Средний (100 предм.)\n3. Большой (500 предм.)\nВыбор: ";
            int fChoice;
            if (!(cin >> fChoice) || fChoice > 3 || fChoice < 1) {
                cout << "Ошибка ввода. Введите верное значение (от 1 до 3)!\n";
                cin.clear();
                cin.ignore(MAXSHORT, '\n');
            }
            string filename = (fChoice == 2) ? "bag_medium.txt" : (fChoice == 3) ? "bag_large.txt" : "bag_small.txt";

            if (fChoice == 1) showLog = true;

            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Ошибка: Файл " << filename << " не найден!\n";
                continue; // Возврат в начало меню
            }
            file >> n;
            w.resize(n);
            for (int i = 0; i < n; ++i) file >> w[i];
            v.resize(n);
            for (int i = 0; i < n; ++i) file >> v[i];
            file >> W;
            file.close();
            cout << "Данные загружены: " << n << " предметов, вместимость " << W << endl;
        }
        else if (mode == 2) {
            showLog = true;
            cout << "Введите количество элементов: ";
            cin >> n;
            w.resize(n); v.resize(n);
            cout << "Введите веса (" << n << " шт.): ";
            for (int i = 0; i < n; ++i) cin >> w[i];
            cout << "Введите стоимости (" << n << " шт.): ";
            for (int i = 0; i < n; ++i) cin >> v[i];
            cout << "Введите вместимость рюкзака: ";
            cin >> W;
        }
        else {
            cout << "Неверный пункт меню!\n";
            continue;
        }

        cout << "\nРезультаты расчетов:\n";

        // 2D Knapsack
        auto start = high_resolution_clock::now();
        auto dp = knapsack01_2D(W, w, v, showLog);
        auto end = high_resolution_clock::now();

        if (showLog) {
            cout << "\n--- 2D DP Подробно ---";
            printDPTable(dp, n, W);
        }

        printComplexity("2D DP", n, W, "O(n * W)");
        int n1 = w.size();
        cout << "2D DP результат: " << dp[n1][W] << endl;

        cout << "Время knapsack01_2D: " << duration<double, milli>(end - start).count() << " мс\n";

        // 1D Knapsack
        auto start1 = high_resolution_clock::now();
        auto result1D = knapsack01_1D(W, w, v, showLog);
        auto end1 = high_resolution_clock::now();

        if (showLog) {
            cout << "\n--- 1D DP Таблица прогресса ---" << endl;
            cout << setw(6) << "w\\i";
            for (int i = 0; i < n; ++i) cout << setw(5) << i + 1;
            cout << endl;
            for (int weight = 0; weight <= W; ++weight) {
                cout << setw(6) << weight;
                for (int i = 0; i < n; ++i) cout << setw(5) << result1D.table[i][weight];
                cout << endl;
            }
        }

        printComplexity("1D DP", n, W, "O(n * W)");
        cout << "1D DP результат: " << result1D.maxValue << endl;

        cout << "Время knapsack01_1D: " << duration<double, milli>(end1 - start1).count() << " мс\n";

        // Unbounded Knapsack
        auto start2 = high_resolution_clock::now();
        auto resultUnbounded = unboundedKnapsack1D(W, w, v, showLog);
        auto end2 = high_resolution_clock::now();

        if (showLog) {
            cout << "\n--- Unbounded DP Таблица прогресса ---" << endl;
            cout << setw(6) << "w\\i";
            for (int i = 0; i < n; ++i) cout << setw(5) << i + 1;
            cout << endl;
            for (int weight = 0; weight <= W; ++weight) {
                cout << setw(6) << weight;
                for (int i = 0; i < n; ++i) cout << setw(5) << resultUnbounded.table[i][weight];
                cout << endl;
            }
        }

        printComplexity("Unbounded DP", n, W, "O(W)");
        int n3 = w.size();
        cout << "Unbounded DP результат: " << resultUnbounded.maxValue << endl;

        cout << "Время unboundedKnapsack1D: " << duration<double, milli>(end2 - start2).count() << " мс\n\n\n";
    }

    return 0;
}