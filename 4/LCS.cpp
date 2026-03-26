#include "LCS.h"

// Метод динамического программирования (Быстрый)
pair<int, string> longest_common_subsequence(const string& str1, const string& str2) {
    int n = str1.size(), m = str2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Заполнение таблицы dp
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // Если совпали, то увеличиваем длину подпоследовательности на 1
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            // Если не совпали, то берём лучшее из соседних ячеек (сверху или слева)
            else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Восстановление LCS
    string lcs;

    // По сути мы двигаемся от конца к началу
    int i = n, j = m;
    while (i > 0 && j > 0) {
        // Если символы на текущих позициях совпали, то этот символ является частью ответа
        if (str1[i - 1] == str2[j - 1]) {
            lcs += str1[i - 1]; // Добавляем символ
            --i; --j; // Переходим вверх влево
        }
        // Если не совпали, идем в ту сторону, где значение в таблице больше
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i; // Смещаемся вверх
        }
        else {
            --j; // // Смещаемся влево
        }
    }

    reverse(lcs.begin(), lcs.end()); // реверс с начала до конца (чтобы слово НОЛС было СЛОН)
    return { dp[n][m], lcs }; // pair для возврата 2-х значений в return
}

// Функция наивной рекурсии
pair<int, string> lcs_recursive_helper(const string& str1, const string& str2, int i, int j) {
    // Базовый случай
    if (i == 0 || j == 0) {
        return { 0, "" };
    }

    // Случай совпадения символов
    if (str1[i - 1] == str2[j - 1]) {
        pair<int, string> res = lcs_recursive_helper(str1, str2, i - 1, j - 1);
        return { res.first + 1, res.second + str1[i - 1] };
    }
    // Случай несовпадения (Ветвление)
    else {
        // с pair тут работать удобнее
        // рекурсии, которые очень долгие, если их много
        pair<int, string> res1 = lcs_recursive_helper(str1, str2, i - 1, j); // игнор последний символа 1 строки
        pair<int, string> res2 = lcs_recursive_helper(str1, str2, i, j - 1); // игнор последний символа 2 строки

        // Сравниваем и выводим того, кто длинее
        if (res1.first > res2.first) return res1;
        else return res2;
    }
}

// Обертка для рекурсии
pair<int, string> common_lcs(const string& str1, const string& str2) {
    return lcs_recursive_helper(str1, str2, (int)str1.size(), (int)str2.size());
}