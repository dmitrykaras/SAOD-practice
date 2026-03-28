import matplotlib.pyplot as plt
import numpy as np

# Данные
sizes = ['Small (5 вершин)', 'Medium (10 вершин)', 'Large (20 вершин)']

# Время в мс (перевод из микросекунд)
time_bfs = [0.043, 0.054, 0.085]
time_dfs = [0.026, 0.039, 0.054]
time_dfs_forest = [0.831, 0.550, 0.714]

# Время в мс
time_dp = [0.0526, 0.9954, 84.4394]
time_recursion = [3418.67, None, None]  # Пропуски для больших строк

plt.figure(figsize=(10, 6))

plt.plot(sizes, time_bfs, marker='o', linewidth=2.5, label='BFS')
plt.plot(sizes, time_dfs, marker='s', linewidth=2.5, label='DFS')
plt.plot(sizes, time_dfs_forest, marker='^', linewidth=2.5, label='DFS-лес')

plt.yscale('log')
plt.ylabel('Время выполнения (мс, логарифмическая шкала)', fontsize=11)
plt.xlabel('Размер графа', fontsize=11)
plt.title('Производительность обходов графа', fontsize=13, fontweight='bold')

# Аннотации
for i in range(len(sizes)):
    plt.annotate(f"{time_bfs[i]:.3f}", (sizes[i], time_bfs[i]), xytext=(0,10), textcoords="offset points", ha='center')
    plt.annotate(f"{time_dfs[i]:.3f}", (sizes[i], time_dfs[i]), xytext=(0,10), textcoords="offset points", ha='center')
    plt.annotate(f"{time_dfs_forest[i]:.3f}", (sizes[i], time_dfs_forest[i]), xytext=(0,10), textcoords="offset points", ha='center')

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.show()