import sys

import primelist


def read_naturals(count: int) -> list[int]:
    """Читает count натуральных чисел из стандартного ввода
    (через пробел и/или по одному на строке)."""
    values = []
    while len(values) < count:
        line = sys.stdin.readline()
        if not line:
            raise ValueError("недостаточно данных")
        for token in line.split():
            if not (token.isascii() and token.isdigit()) or int(token) < 1:
                raise ValueError("нужно ввести натуральное число")
            values.append(int(token))
    return values[:count]


def main() -> int:
    try:
        left, right = read_naturals(2)
    except ValueError:
        print("Ошибка: нужно ввести два натуральных числа.", file=sys.stderr)
        return 1

    if left > right:
        left, right = right, left

    for n in primelist.prime_list(left, right):
        print(n)
    return 0


if __name__ == "__main__":
    sys.exit(main())
