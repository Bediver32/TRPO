#ifndef PRIMELIST_H
#define PRIMELIST_H

#include <vector>

// Возвращает все простые числа из отрезка [l, r] по возрастанию.
// Модуль не взаимодействует с пользователем, поэтому его можно
// использовать без изменений в приложении с графическим интерфейсом.
std::vector<int> primelist(int l, int r);

#endif // PRIMELIST_H
