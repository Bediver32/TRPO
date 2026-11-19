#ifndef PRIMELIST_H
#define PRIMELIST_H

#include <vector>

// Возвращает все простые числа из отрезка [l, r] по возрастанию.
// Выбрасывает std::out_of_range, если 1 <= l < r <= INT_MAX не выполняется.
std::vector<int> primelist(long long l, long long r);

#endif // PRIMELIST_H
