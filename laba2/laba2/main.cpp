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
    // Устанавливаем кодировку сразу при старте
    SET_CONSOLE_ENCODING();

    while (true) {
        cout << "Исследование алгоритмов на графах\n";
        cout << "1. Обходы графа (BFS, DFS, DFS-лес)\n";
        cout << "2. Сильно связные компоненты (Алгоритм Косараджу)\n";
        cout << "3. Минимальное остовное дерево (Краскал и Прим)\n";
        cout << "4. Кратчайшие пути (Беллман-Форд)\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";

        int mode = getSafeInt();
        if (mode == 0) {
            cout << "Завершение работы...\n";
            break;
        }

        if (mode < 0 || mode > 4) {
            cout << "Неверный пункт меню!\n";
            continue;
        }

        if (mode == 1) {
            runBFS_DFS();
        }
        else if (mode == 2) {
            // Заглушка, пока не реализовано
            cout << "\n[INFO] Алгоритм Косараджу будет реализован далее.\n";
            // runKosaraju(); 
        }
        else if (mode == 3) {
            // Заглушка
            cout << "\n[INFO] Алгоритмы Краскала и Прима будут реализованы далее.\n";
            // runKruskal_Prim();
        }
        else if (mode == 4) {
            // Заглушка
            cout << "\n[INFO] Алгоритм Беллмана-Форда будет реализован далее.\n";
            // runBellman_Moore();
        }
    }
    return 0;
}