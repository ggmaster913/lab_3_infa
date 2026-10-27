#include "RecurrentSequence.h"
#include <stdexcept>

long long FactorialNext(const Sequence<long long>* history, int n){

    // пример ошибки в функции формуле
    if (n > 20) {
        throw std::overflow_error("Переполнение типа long long");
    }
    return history->Get(n - 1) * n;
}

long long FibonacciNext(const Sequence<long long>* history, int n){
    return history->Get(n - 1) + history->Get(n - 2);
}
