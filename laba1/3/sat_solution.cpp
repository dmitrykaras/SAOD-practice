#include "sat_solution.h"

// Функции алгоритма DPLL
// Упрощение формулы на основе выбранного значения переменной
void simplify(CNFFormula& f, int var, int value) {
    vector<Clause> newClauses;
    // Проходим по каждой клаузе (скобке) в текущей формуле
    for (auto& c : f.clauses) {
        bool satisfied = false;
        Clause newClause;
        // Проверяем литералы внутри конкретной клаузы
        for (int lit : c.literals) {
            int v = abs(lit) - 1; // Индекс переменной (от 0)
            if (v == var) {
                // Если литерал становится истинным при данном значении — вся клауза удовлетворена
                if ((lit > 0 && value == 1) || (lit < 0 && value == 0)) {
                    satisfied = true;
                    break;
                }
            }
            else {
                // Если переменная не совпадает с выбранной, сохраняем литерал для новой версии формулы
                newClause.literals.push_back(lit);
            }
        }
        // Если клауза не была удовлетворена текущим выбором, добавляем её (в сокращенном виде) в новую формулу
        if (!satisfied) newClauses.push_back(newClause);
    }
    f.clauses = newClauses;
}

// Правило распространения единицы (Unit Propagation)
bool unitPropagation(CNFFormula& f, vector<int>& assignment) {
    bool changed = true;
    // Цикл работает, пока находятся новые "единичные" клаузы
    while (changed) {
        changed = false;
        for (auto& c : f.clauses) {
            // Если в клаузе остался всего один литерал — у нас нет выбора, он должен быть истинным
            if (c.literals.size() == 1) {
                int lit = c.literals[0];
                int var = abs(lit) - 1;
                int val = (lit > 0) ? 1 : 0;

                // Проверяем на противоречие с уже назначенными значениями
                if (assignment[var] == -1) {
                    assignment[var] = val; // Назначаем значение
                    simplify(f, var, val); // Сразу упрощаем формулу
                    changed = true;
                    break;
                }
                else if (assignment[var] != val) return false; // Нашли противоречие (UNSAT в данной ветке)
            }
        }
    }
    return true;
}

int dpllSteps = 0;

// Основная рекурсивная функция DPLL
bool DPLLRecursive(CNFFormula f, vector<int>& assignment) {

    dpllSteps++;
    // 1. Применяем Unit Propagation для автоматического вывода значений
    if (!unitPropagation(f, assignment)) return false;

    // 2. Базовые случаи: формулы пуста (все удовлетворены) или есть пустая клауза (противоречие)
    if (f.clauses.empty()) return true;
    for (auto& c : f.clauses) if (c.literals.empty()) return false;

    // 3. Выбираем следующую свободную переменную для ветвления
    int var = -1;
    for (int i = 0; i < (int)assignment.size(); ++i)
        if (assignment[i] == -1) { var = i; break; }

    if (var == -1) return false;

    // 4. Ветвление: Пробуем назначить переменной ИСТИНУ (1)
    CNFFormula f1 = f; auto a1 = assignment;
    a1[var] = 1; simplify(f1, var, 1);
    if (DPLLRecursive(f1, a1)) { assignment = a1; return true; }

    // 5. Если ИСТИНА не подошла, пробуем ЛОЖЬ (0)
    CNFFormula f0 = f; auto a0 = assignment;
    a0[var] = 0; simplify(f0, var, 0);
    if (DPLLRecursive(f0, a0)) { assignment = a0; return true; }

    return false; // Ни один вариант не привел к решению
}

bool runDPLL(CNFFormula f, vector<int>& assignment) {
    dpllSteps = 0;
    assignment.assign(f.numVars, -1); // Инициализация: -1 означает "не назначено"
    return DPLLRecursive(f, assignment);
}