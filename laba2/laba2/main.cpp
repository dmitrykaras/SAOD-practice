#include "common.h"

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
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    while (true) {
        cout << "\nИсследование алгоритмов на графах\n";
        cout << "1. Обходы графа (BFS, DFS, DFS-лес)\n";
        cout << "2. Сильно связные компоненты (Алгоритм Косарайю)\n";
        cout << "3. Минимальное остовное дерево (Краскал и Прим)\n";
        cout << "4. Кратчайшие пути (Беллман-Мур)\n";
        cout << "0. Выход\n";
        cout << "Ваше выбор: ";

        int mode = getSafeInt();
        if (mode == 0) {
            cout << "Завершение работы...\n";
            break;
        }

        if (mode >= 5) {
            cout << "Неверный пункт меню!\n";
            continue;
        }

        if (mode == 1) {
            runBFS_DFS();
        }

        if (mode == 2) {
            runKosaraju();
        }

        if (mode == 3) {
            runKruskal_Prim();
        }

        if (mode == 4) {
            runBellman_Moore();
        }

        cout << "\n\n";
    }
    return 0;
}