import matplotlib.pyplot as plt
import numpy as np

# Данные
sizes = ['Small (12x12)', 'Medium (100x100)', 'Large (1000x1000)']
n_chars = [12, 100, 1000]

# Время в мс
time_dp = [0.0526, 0.9954, 84.4394]
time_recursion = [3418.67, None, None]  # Пропуски для больших строк

plt.figure(figsize=(10, 6))

# Построение линии для ДП
plt.plot(sizes, time_dp, marker='s', color='#2ca02c', linewidth=2.5, 
         label='Динамическое программирование (O(n*m))')

# Построение точки для рекурсии
plt.plot(sizes[0], time_recursion[0], marker='o', color='#d62728', 
         markersize=10, label='Рекурсия (O(2^n)) - остановлена на 12 симв.')

# Оформление
plt.yscale('log') # Логарифмическая шкала обязательна
plt.ylabel('Время выполнения (мс, логарифмическая шкала)', fontsize=11)
plt.xlabel('Размер входных строк', fontsize=11)
plt.title('Производительность алгоритмов LCS (LCS Performance)', 
          fontsize=13, fontweight='bold')

# Аннотации для ДП
for i, txt in enumerate(time_dp):
    plt.annotate(f"{txt:.2f} ms", (sizes[i], time_dp[i]), 
                 textcoords="offset points", xytext=(0,10), ha='center')

# Аннотация для рекурсии
plt.annotate(f"{time_recursion[0]:.1f} ms", (sizes[0], time_recursion[0]), 
             textcoords="offset points", xytext=(0,10), ha='center', color='#d62728')

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(loc='lower right', frameon=True, shadow=True)

plt.tight_layout()
plt.show()