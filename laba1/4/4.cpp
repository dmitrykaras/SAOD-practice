#include "LCS.h"

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
    setlocale(LC_ALL, ".UTF8");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    while (true) {
        cout << "Наибольшая общая подпоследовательность (LCS)\n";
        cout << "1. Загрузить из файла\n";
        cout << "2. Ввести строки вручную\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";

        int mode = getSafeInt();
        if (mode == 0) break;
        if (mode >= 3) {
            cout << "Ошибка: Неверный пункт меню!\n\n";
            continue;
        }

        string str1, str2;

        if (mode == 1) {
            bool fileLoaded = false;
            while (true) {
                cout << "\nВыбор файла:\n";
                cout << "1. Маленький (lcs_small.txt)\n";
                cout << "2. Средний (lcs_medium.txt)\n";
                cout << "3. Большой (lcs_large.txt)\n";
                cout << "0. Назад\n";
                cout << "Ваш выбор: ";

                int fChoice = getSafeInt();
                if (fChoice == 0) break;
                cout << "\n";

                string filename;
                if (fChoice == 1) filename = "lcs_small.txt";
                else if (fChoice == 2) filename = "lcs_medium.txt";
                else if (fChoice == 3) filename = "lcs_large.txt";
                else {
                    cout << "Ошибка: Неверный выбор файла!\n";
                    continue;
                }

                ifstream fin(filename);
                if (!fin.is_open()) {
                    cout << "Ошибка: Не удалось открыть файл " << filename << "!\n";
                    continue;
                }

                fin >> str1 >> str2;
                fin.close();

                cout << "Файл успешно загружен!\n";
                cout << "Длина строки 1: " << str1.size() << "\n";
                cout << "Длина строки 2: " << str2.size() << "\n";
                fileLoaded = true;
                break;
            }
            if (!fileLoaded) continue;
        }

        if (mode == 2) {
            cout << "Введите первую строку: ";
            cin >> str1;
            cout << "Введите вторую строку: ";
            cin >> str2;
        }


        cout << "\nНаивный рекурсивный метод:" << endl;
        if (str1.size() > 15 || str2.size() > 15) {
            cout << "Пропуск! Строки слишком длинные (>15 символов)\n\n";
        }
        else {
            auto start2 = high_resolution_clock::now();
            pair<int, string> result2 = common_lcs(str1, str2);
            auto end2 = high_resolution_clock::now();

            cout << "Время рекурсии: " << duration<double, milli>(end2 - start2).count() << " мс\n";
            cout << "Длина LCS: " << result2.first << "\n";
            cout << "LCS: " << result2.second << "\n";
        }

        cout << "\nМетод с использованием динамического программирования:" << endl;
        auto start1 = high_resolution_clock::now();
        pair<int, string> result1 = longest_common_subsequence(str1, str2);
        auto end1 = high_resolution_clock::now();

        cout << "Время ДП: " << duration<double, milli>(end1 - start1).count() << " мс\n";
        cout << "Длина LCS: " << result1.first << "\n";
        if (str1.size() <= 100) cout << "LCS: " << result1.second << "\n"; // Не выводить LCS, если он <= 100

        cout << "\n\n";
    }
    return 0;
}