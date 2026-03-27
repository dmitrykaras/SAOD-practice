#include "pareto.h"

// Функция опеределяющая доминируе ли a над b
bool isDominating(const Laptop& a, const Laptop& b) {
    
    // Критерии: Price (min), Weight (min), Ram (max), CPU (max)
    bool notWorse = (a.price <= b.price) &&
        (a.weight <= b.weight) &&
        (a.ram >= b.ram) &&
        (a.cpu_freq >= b.cpu_freq);

    // true, если 'a' превосходит 'b' хотя бы по одному показателю
    bool hasAnyAdvantage = (a.price < b.price) ||
        (a.weight < b.weight) ||
        (a.ram > b.ram) ||
        (a.cpu_freq > b.cpu_freq);

    return notWorse && hasAnyAdvantage;
}

// Построение N-множества (Парето-слои) - Наивный алгоритм O(N^2 * Layers)
void computeParetoLayers(vector<Laptop>& data) {
    int currentLayer = 1;
    int processedCount = 0;
    int n = data.size();

    while (processedCount < n) {
        vector<int> currentLayerIndices;

        for (int i = 0; i < n; ++i) {
            if (data[i].pareto_layer != 0) continue; // уже использовано? пропускаем

            bool dominatedByOthersInRemaining = false;
            for (int j = 0; j < n; ++j) {
                // Не сравниваем с самим собой или ноутбук j уже не участвует в отборе - пропускаем
                if (i == j || data[j].pareto_layer != 0) continue;

                // Определяем что доминирует и даём флаг "доминирует другие оставшиеся"
                if (isDominating(data[j], data[i])) {
                    dominatedByOthersInRemaining = true;
                    break;
                }
            }

            // Если не доминирует оставшиеся, то заносим i в текущий фронт Парето
            if (!dominatedByOthersInRemaining) {
                currentLayerIndices.push_back(i);
            }
        }

        // Перебираем победителей текущего раунда
        for (int idx : currentLayerIndices) {
            data[idx].pareto_layer = currentLayer;
            processedCount++;
        }
        currentLayer++;
    }
}

// Оптимизированный Парето
void computeParetoLayersOptimized(vector<Laptop>& data) {
    int n = data.size();
    if (n == 0) return;

    // 1. Создаем массив индексов для безопасной сортировки без изменения оригинального порядка
    vector<int> indices(n);
    for (int i = 0; i < n; ++i) {
        indices[i] = i;
    }

    // 2. Лексикографическая сортировка
    // Сортируем от лучших значений к худшим
    sort(indices.begin(), indices.end(), [&](int i, int j) {
        const Laptop& a = data[i];
        const Laptop& b = data[j];

        // Чем дешевле, тем лучше
        if (a.price != b.price) return a.price < b.price;
        // Чем легче, тем лучше
        if (a.weight != b.weight) return a.weight < b.weight;

        // Чем больше RAM, тем лучше
        if (a.ram != b.ram) return a.ram > b.ram;

        // Максимизируем частоту процессора
        return a.cpu_freq > b.cpu_freq;
        });

    // 3. Распределение по фронтам (слоям)
    // fronts хранит индексы вин, разбитые по слоям (0-й индекс массива = 1-й слой Парето)
    vector<vector<int>> fronts;

    for (int idx : indices) {
        const Laptop& current_wine = data[idx];
        bool placed = false;

        // Ищем первый слой, где НИ ОДИН элемент не доминирует над current_wine
        for (size_t layer_idx = 0; layer_idx < fronts.size(); ++layer_idx) {
            bool dominated_by_this_layer = false;

            for (int front_wine_idx : fronts[layer_idx]) {
                if (isDominating(data[front_wine_idx], current_wine)) {
                    dominated_by_this_layer = true;
                    break; // Сразу прерываем, если нашли того, кто доминирует
                }
            }

            // Если в текущем слое никто не доминирует над нашим вином — кладем его сюда
            if (!dominated_by_this_layer) {
                fronts[layer_idx].push_back(idx);
                data[idx].pareto_layer = layer_idx + 1;
                placed = true;
                break;
            }
        }

        // Если вино хуже всех существующих слоев (доминируется кем-то в каждом слое),
        // оно открывает новый слой Парето
        if (!placed) {
            fronts.push_back({ idx });
            data[idx].pareto_layer = fronts.size();
        }
    }
}