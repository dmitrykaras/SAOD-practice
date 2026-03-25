import random
import string

def generate_lcs_file(filename, length):
    # Генерируем случайные строки из заглавных латинских букв
    str1 = ''.join(random.choices(string.ascii_uppercase, k=length))
    str2 = ''.join(random.choices(string.ascii_uppercase, k=length))
    
    with open(filename, 'w') as f:
        f.write(f"{str1}\n{str2}")
    print(f"Файл {filename} создан (длина строк: {length})")

# Создаем наборы данных
generate_lcs_file("lcs_small.txt", 12)    # Для теста рекурсии
generate_lcs_file("lcs_medium.txt", 100)  # Для теста ДП
generate_lcs_file("lcs_large.txt", 1000)  # Для замера времени ДП