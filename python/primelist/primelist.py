"""Модуль получения списка простых чисел.

Не взаимодействует с пользователем, поэтому может использоваться
без изменений в приложении с графическим интерфейсом.
Публичный интерфейс модуля - только функция prime_list.
"""
from math import isqrt

__all__ = ["prime_list"]


def _is_prime(n: int) -> bool:
    """Проверка числа на простоту перебором делителей до sqrt(n)."""
    if n < 2:
        return False
    for i in range(2, isqrt(n) + 1):
        if n % i == 0:
            return False
    return True


def prime_list(l: int, r: int) -> list[int]:
    """Все простые числа из отрезка [l, r] по возрастанию."""
    result = []
    for n in range(l, r + 1):
        if _is_prime(n):
            result.append(n)
    return result
