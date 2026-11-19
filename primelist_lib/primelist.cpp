#include "primelist.h"

#include <climits>
#include <stdexcept>
#include <string>

namespace
{

// Проверка числа на простоту перебором делителей.
// Достаточно проверить делители до sqrt(n): если у n есть делитель
// больше sqrt(n), то есть и парный делитель меньше sqrt(n).
bool isprime(int n)
{
  if(n < 2)
  {
    return false;
  }
  for(int i = 2; i <= n / i; ++i)
  {
    if(n % i == 0)
    {
      return false;
    }
  }
  return true;
}

} // namespace

std::vector<int> primelist(long long l, long long r)
{
  if(l < 1 || l > INT_MAX)
  {
    throw std::out_of_range("Параметр A (нижняя граница) должен быть натуральным числом от 1 до " +
                            std::to_string(INT_MAX) + ".");
  }
  if(r < 1 || r > INT_MAX)
  {
    throw std::out_of_range("Параметр B (верхняя граница) должен быть натуральным числом от 1 до " +
                            std::to_string(INT_MAX) + ".");
  }
  if(r <= l)
  {
    throw std::out_of_range("Верхняя граница B должна быть больше нижней границы A.");
  }
  std::vector<int> result;
  // long long - чтобы счётчик не переполнился при r == INT_MAX
  for(long long n = l; n <= r; ++n)
  {
    if(isprime(static_cast<int>(n)))
    {
      result.push_back(static_cast<int>(n));
    }
  }
  return result;
}
