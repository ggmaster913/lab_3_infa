#pragma once

#include "RecurrentSequence.h"
#include "lab2/MutableArraySequence.h"
#include <stdexcept>

template<typename T>
Sequence<T>* GenerateRecurrent(const Sequence<T>* initialTerms, int totalCount, T (*next)(const Sequence<T>*, int)){ // int у next - номер эл который считается
    if (initialTerms == nullptr) throw std::invalid_argument("нулевой указатель");
    int initialLen = initialTerms->GetLength();
    if (totalCount < initialLen) {
        throw std::invalid_argument("нельзя сгенерировать меньше элементов чем дано на старте");
    }

    MutableArraySequence<T>* result = new MutableArraySequence<T>();

    try {
        //копируем исходные эл в result
        for (int i = 0; i < initialLen; i++){
            result->Append(initialTerms->Get(i));
        }

        // Вычисляем каждый следующий элемент x_n
        for (int n = initialLen; n < totalCount; ++n) {
            T nextVal = next(result, n); // если кинет исключение
            result->Append(nextVal);
        }
    }

    catch (...) { // не знаем какая ошибка может вылезти, ловит исключения любого типа
        delete result;
        throw; // бросает пойманное исключение 
    }
    return result;
}

template<typename T>
Sequence<T>* GenerateLinearRecurrent(const Sequence<T>* initialTerms,
                                      const Sequence<T>* coefficients,int totalCount) {
    if (initialTerms == nullptr || coefficients == nullptr) {
        throw std::invalid_argument("передан nullptr в аргументы");
    }

    int k = coefficients->GetLength();
    if (k == 0) {
        throw std::invalid_argument("список коэффициентов не может быть пустым");
    }
    if (initialTerms->GetLength() != k) {
        throw std::invalid_argument("количество начальных условий должно совпадать с числом коэффициентов");
    }
    if (totalCount < k) {
        throw std::invalid_argument("totalCount не может быть меньше порядка рекурренты");
    }

    MutableArraySequence<T>* result = new MutableArraySequence<T>();

    for (int i = 0; i < k; ++i) {
        result->Append(initialTerms->Get(i));
    }

    // считаем следующие элементы через сумму произведений
    for (int n = k; n < totalCount; ++n) {
        T sum = 0;
        for (int j = 0; j < k; ++j) {
            // coefficients[0] умножается на x_{n-1}, coefficients[1] на x_{n-2} и тд
            sum += coefficients->Get(j) * result->Get(n - 1 - j);
        }
        result->Append(sum);
    }

    return result;
}
