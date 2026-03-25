import pandas as pd
import matplotlib.pyplot as plt

# Загрузка
df = pd.read_csv('results.csv', encoding='cp1251')

# Группируем данные: считаем, сколько ноутбуков в каждом слое
layer_counts = df['layer'].value_counts().sort_index()

plt.figure(figsize=(10, 6))

# Рисуем столбики
bars = plt.bar(layer_counts.index, layer_counts.values, color='skyblue', edgecolor='black')

# Добавляем линию для наглядности "горки"
plt.plot(layer_counts.index, layer_counts.values, color='red', marker='o', linestyle='--', linewidth=2)

# Подписываем количество над каждым столбиком
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 1, int(yval), ha='center', va='bottom')

plt.title('Распределение количества моделей по слоям Парето')
plt.xlabel('Номер слоя (от лучшего к худшему)')
plt.ylabel('Количество ноутбуков в слое')
plt.xticks(layer_counts.index) # чтобы были все номера слоев
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.show()