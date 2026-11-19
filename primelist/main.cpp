#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "primelist.h"

namespace {

enum ExitCode {
    Success = 0,
    MissingArguments = 1,
    InvalidDecimal = 2,
    InvalidBounds = 3,
    UnexpectedError = 4
};

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
        for(int n : primelist(left, right)) {
            std::cout << n << '\n';
        }
        return Success;
    } catch(const std::length_error &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return MissingArguments;
    } catch(const std::invalid_argument &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return InvalidDecimal;
    } catch(const std::out_of_range &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return InvalidBounds;
    } catch(const std::exception &error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return UnexpectedError;
    }
}
