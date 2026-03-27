#include "sat_solution.h"

// Функция безопасного ввода целого числа
int getSafeInt() {
    int value;
    while (true) {
        if (cin >> value) {
            return value;
        }
        else {
            cin.clear();
            cin.ignore(32767, '\n');
            cout << "Ошибка! Введите целое число: ";
        }
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    while (true) {
        cout << "SAT Solver: DPLL\n";
        cout << "1. Загрузить из файла\n";
        cout << "2. Ввести формулу вручную\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";

        int mode = getSafeInt();
        if (mode == 0) break;
        if (mode >= 3) {
            cout << "Ошибка: Неверный пункт меню!\n\n";
            continue;
        }

        CNFFormula f;

        if (mode == 1) {
            while (true) {
                cout << "Выбор файла:\n";
                cout << "1. Простой\n";
                cout << "2. Средний\n";
                cout << "3. Сложный\n";
                cout << "0. Назад\n";
                cout << "Ваш выбор: ";

                int fChoice;
                fChoice = getSafeInt();

                if (fChoice == 0) {
                    cout << "Возвращение к выбору метода ввода...\n\n";
                    break;
                }
                string filename;
                if (fChoice == 1) filename = "sat_simple.txt";
                else if (fChoice == 2) filename = "sat_medium.txt";
                else if (fChoice == 3) filename = "sat_large.txt";
                else {
                    cout << "Ошибка: Неверный выбор файла!\n";
                    continue;
                }

                ifstream file(filename);
                if (!file.is_open()) {
                    cout << "Ошибка открытия файла!\n";
                    continue;
                }

                string p, cnf;
                int clauseCount;
                file >> p >> cnf >> f.numVars >> clauseCount;

                for (int i = 0; i < clauseCount; i++) {
                    Clause cl;
                    int literal;

                    while (file >> literal && literal != 0) {
                        cl.literals.push_back(literal);
                    }

                    f.clauses.push_back(cl);
                }

                cout << "Файл " << filename << " успешно загружен!\n";
                break;
            }

            // Проверка, что при нажатии "0" и f.clauses пустая, то не запускаем алгоритм
            if (f.clauses.empty()) continue;
        }

        if (mode == 2) {
            cout << "\nВведите количество переменных: ";
            f.numVars = getSafeInt();

            if (f.numVars <= 0) { cout << "Ошибка: должно быть > 0 переменных!" << endl; continue; }

            cout << "Введите количество клауз (скобок): ";
            int numClauses = getSafeInt();

            for (int i = 0; i < numClauses; ++i) {
                Clause cl;
                cout << "Клауза " << i + 1 << " (номера переменных через пробел, 0 в конце): ";
                // Считываем литералы до ввода нуля
                while (true) {
                    int lit = getSafeInt();
                    if (lit == 0) break;
                    if (abs(lit) > f.numVars)
                        cout << "Пропуск: переменной x" << abs(lit) << " нет в базе!" << endl;
                    else cl.literals.push_back(lit);
                }
                if (!cl.literals.empty()) f.clauses.push_back(cl);
            }
        }

        if (f.clauses.empty()) {
            cout << "Формула пуста! Она всегда выполнима (SAT)." << endl;
            continue;
        }

        vector<int> assign;

        // Тест DPLL
        cout << "\nЗапуск DPLL..." << endl;
        auto s1 = high_resolution_clock::now();
        bool resDPLL = runDPLL(f, assign); // обычный DPLL
        auto e1 = high_resolution_clock::now();
        cout << "Результат: " << (resDPLL ? "ВЫПОЛНИМА" : "НЕВЫПОЛНИМА")
            << " | Время: " << duration<double, milli>(e1 - s1).count() << " мс"
            << " | Шагов: " << dpllSteps << endl;


        if (resDPLL) {
            cout << "Набор: ";
            for (int i = 0; i < (int)assign.size(); i++)
                cout << "x" << i + 1 << "=" << (assign[i] == 1 ? "T " : "F ");
            cout << endl;
        }
        cout << "\n";
    }
    return 0;
}