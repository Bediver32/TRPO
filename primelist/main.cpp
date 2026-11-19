#include <climits>
#include <iostream>
#include <sstream>

#include "primelist.h"

enum ExitCode {
    Success = 0,
    MissingArguments = 1,
    InvalidDecimal = 2,
    InvalidBounds = 3
};

int main(int argc, char * argv[]) {
    if(argc < 3) {
        std::cerr << "Ошибка: укажите нижнюю и верхнюю границы диапазона.\n";
        return MissingArguments;
    }
    long long left = 0, right = 0;
    std::istringstream leftInput(argv[1]);
    std::istringstream rightInput(argv[2]);
    leftInput >> std::noskipws >> left;
    rightInput >> std::noskipws >> right;
    if(leftInput.fail() || !leftInput.eof()) {
        std::cerr << "Ошибка: параметр A (нижняя граница) должен быть десятичным целым числом, представимым в long long.\n";
        return InvalidDecimal;
    }
    if(rightInput.fail() || !rightInput.eof()) {
        std::cerr << "Ошибка: параметр B (верхняя граница) должен быть десятичным целым числом, представимым в long long.\n";
        return InvalidDecimal;
    }
    if(left < 1 || left > INT_MAX) {
        std::cerr << "Ошибка: параметр A (нижняя граница) должен быть натуральным числом от 1 до "
                  << INT_MAX << ".\n";
        return InvalidBounds;
    }
    if(right < 1 || right > INT_MAX) {
        std::cerr << "Ошибка: параметр B (верхняя граница) должен быть натуральным числом от 1 до "
                  << INT_MAX << ".\n";
        return InvalidBounds;
    }
    if(right <= left) {
        std::cerr << "Ошибка: верхняя граница B должна быть больше нижней границы A.\n";
        return InvalidBounds;
    }
    for(int n : primelist(static_cast<int>(left), static_cast<int>(right))) {
        std::cout << n << '\n';
    }
    return Success;
}
