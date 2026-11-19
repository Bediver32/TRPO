#ifndef PRIMELIST_H
#define PRIMELIST_H

#include <stdexcept>
#include <vector>

class PrimeRangeError : public std::out_of_range
{
public:
  enum class Reason
  {
    LowerBound,
    UpperBound,
    BoundOrder
  };

  explicit PrimeRangeError(Reason reason);
  Reason reason() const noexcept;

private:
  Reason cause;
};

// Возвращает все простые числа из отрезка [l, r] по возрастанию.
// Выбрасывает PrimeRangeError, если 1 <= l < r <= INT_MAX не выполняется.
std::vector<int> primelist(long long l, long long r);

#endif // PRIMELIST_H
