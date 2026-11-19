#include <climits>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "primelist.h"

namespace {

long long parseDecimal(const char * text, const char * parameter) {
    std::istringstream input(text);
    long long value = 0;
    input >> std::noskipws >> value;
    if(input.fail() || !input.eof()) {
        throw std::invalid_argument(std::string("Параметр ") + parameter +
                                    " должен быть десятичным целым числом, представимым в long long.");
    }
    return value;
}

} // namespace

int main(int argc, char * argv[]) {
    try {
        if(argc < 3) {
            throw std::length_error("Укажите нижнюю и верхнюю границы диапазона.");
        }
        const long long left = parseDecimal(argv[1], "A (нижняя граница)");
        const long long right = parseDecimal(argv[2], "B (верхняя граница)");
        if(left < 1 || left > INT_MAX) {
            throw std::out_of_range("Параметр A (нижняя граница) должен быть натуральным числом от 1 до " +
                                    std::to_string(INT_MAX) + ".");
        }
        if(right < 1 || right > INT_MAX) {
            throw std::out_of_range("Параметр B (верхняя граница) должен быть натуральным числом от 1 до " +
                                    std::to_string(INT_MAX) + ".");
        }
        if(right <= left) {
            throw std::out_of_range("Верхняя граница B должна быть больше нижней границы A.");
        }
        for(int n : primelist(static_cast<int>(left), static_cast<int>(right))) {
            std::cout << n << '\n';
        }
        return 0;
    } catch(const std::exception &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
