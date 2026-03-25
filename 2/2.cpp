#include "pareto.h"

// Вспомогательная функция для парсинга CPU
double safeStod(string s) {
    if (s.empty()) return 0.0;

    // Убираем кавычки, если они есть
    s.erase(remove(s.begin(), s.end(), '\"'), s.end());

    // Оставляем только цифры и точку
    string clean;
    for (char c : s) {
        if (isdigit(c) || c == '.') clean += c;
    }

    try {
        if (clean.empty()) return 0.0;
        return stod(clean);
    }
    catch (...) {
        return 0.0;
    }
}

// Метод парсинга
vector<Laptop> loadFromCSV(string filename, int limit) {
    vector<Laptop> data;
    ifstream file(filename);
    if (!file.is_open()) return data;

    string line;
    getline(file, line); // Пропуск заголовка

    int count = 0;
    while (getline(file, line) && count < limit) {
        if (line.empty()) continue; // Пропуск пустых строк

        stringstream ss(line);
        string cell;
        vector<string> row;

        // ВАЖНО: правильное чтение CSV с учетом кавычек
        // Если данные простые, getline по запятой пойдет, но лучше так:
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (row.size() >= 13) {
            try {
                Laptop l;
                // Используем safeStod для всех числовых полей
                l.id = (int)safeStod(row[0]);
                l.model = row[1] + " " + row[2];

                l.ram = safeStod(row[7]);    // safeStod сам проигнорирует "GB"
                l.weight = safeStod(row[11]); // safeStod сам проигнорирует "kg"
                l.price = safeStod(row[12]);

                // Парсинг CPU (более надежный поиск числа перед GHz)
                string cpuStr = row[6];
                size_t ghzPos = cpuStr.find("GHz");
                if (ghzPos != string::npos) {
                    string freqPart = "";
                    // Идем назад от "GHz" и собираем цифры и точку
                    for (int i = (int)ghzPos - 1; i >= 0; i--) {
                        if (isdigit(cpuStr[i]) || cpuStr[i] == '.') {
                            freqPart = cpuStr[i] + freqPart;
                        }
                        else if (!freqPart.empty()) {
                            break; // Встретили пробел или букву после числа
                        }
                    }
                    l.cpu_freq = (freqPart.empty()) ? 0.0 : stod(freqPart);
                }
                else {
                    l.cpu_freq = 0.0;
                }

                data.push_back(l);
                count++;
            }
            catch (...) {
                continue; // Если одна строка битая, идем к следующей
            }
        }
    }
    file.close();
    return data;
}

// Функция для ручного ввода данных
vector<Laptop> inputManual() {
    int n;
    cout << "Введите количество ноутбуков для сравнения: ";
    cin >> n;

    vector<Laptop> data;
    for (int i = 0; i < n; ++i) {
        Laptop l;
        l.id = i + 1;
        cout << "\nНоутбук #" << l.id << endl;
        cout << "  Название модели: ";
        cin.ignore(); // Очистка буфера после cin >> n
        getline(cin, l.model);

        cout << "  Цена (Руб, минимизация): ";
        cin >> l.price;
        cout << "  Вес (Кг., минимизация): ";
        cin >> l.weight;
        cout << "  RAM (GB, максимизация): ";
        cin >> l.ram;
        cout << "  Частота CPU (GHz, максимизация): ";
        cin >> l.cpu_freq;

        data.push_back(l);
    }
    return data;
}

// Функция сохранения Парето
void saveToCSV(const vector<Laptop>& data, string filename) {
    ofstream out(filename);
    // Заголовок важен для Python
    out << "id,model,price,ram,cpu,weight,layer\n";
    for (const auto& l : data) {
        out << l.id << ","
            << l.model << ","
            << l.price << ","
            << l.ram << ","
            << l.cpu_freq << ","
            << l.weight << ","
            << l.pareto_layer << "\n";
    }
    out.close();
    cout << "Данные сохранены в " << filename << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (true) {
        cout << "Программа построения множества Парето\n";
        cout << "Выберите режим работы:\n";
        cout << "1. Загрузить из файла (laptop_price.csv)\n";
        cout << "2. Ввести данные вручную\n";
        cout << "0. Выход из программы\n";
        cout << "Ваш выбор: ";

        int mode;
        cin >> mode;
        cout << "\n";

        if (mode == 0) {
            cout << "Завершение работы.\n";
            break;
        }

        vector<Laptop> dataset;

        if (mode == 1) {
            cout << "Введите количество моделей (до 1303): ";
            int limit;
            cin >> limit;
            if (limit <= 0 || limit > 1303) {
                cout << "Ошибка: Неверное количество!\n";
                continue;
            }
            dataset = loadFromCSV("laptop_price.csv", limit);
        }
        else if (mode == 2) {
            dataset = inputManual();
        }
        else {
            cout << "Неверный режим! Попробуйте снова.\n";
            continue;
        }

        if (dataset.empty()) {
            cout << "Данные не загружены!\n";
            continue;
        }

        cout << "\nЗагружено моделей: " << dataset.size() << endl;
        cout << "Расчет слоев Парето...\n";

        // Замер времени
        auto start = high_resolution_clock::now();
        computeParetoLayers(dataset); // Основной алгоритм
        auto end = high_resolution_clock::now();
        saveToCSV(dataset, "results.csv");

        cout << "Время computeParetoLayers: " << duration<double, milli>(end - start).count() << " мс\n";

        auto start1 = high_resolution_clock::now();
        computeParetoLayersOptimized(dataset); // Оптизмированный Патеро
        auto end1 = high_resolution_clock::now();
        saveToCSV(dataset, "results2.csv");

        cout << "Время оптизмированного Патеро: " << duration<double, milli>(end1 - start1).count() << " мс\n";

        // Находим макс. слой для информации
        int maxLayer = 0;
        for (const auto& l : dataset) {
            if (l.pareto_layer > maxLayer) maxLayer = l.pareto_layer;
        }
        cout << "Всего уровней Парето: " << maxLayer << endl;

        cout << "Запуск визуализации...\n\n";

        system("start python viz.py");
    }

    return 0;
}