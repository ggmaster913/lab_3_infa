#pragma once

#include "lab2/Sequence.h"

// универсальный генератор, initialTerms - начальные эл, totalCount - кол-во эл в итоговой последовательности, next - функция формула
template<typename T>
Sequence<T>* GenerateRecurrent(const Sequence<T>* initialTerms, int totalCount, T (*next)(const Sequence<T>*, int));

//пример next-та
long long FactorialNext(const Sequence<long long>* history, int n);

//пример next-та
long long FibonacciNext(const Sequence<long long>* history, int n);


/*
    лин рекуррентные послед
каждый след член - это сумма предыдущих k членов умноженных на заданные коэффициенты
x_n = c_0 x_{n-1} + c_1 x_{n-2} + c_{k-1} x_{n-k}
*/
// coefficients - список коэффициентов
template<typename T>
Sequence<T>* GenerateLinearRecurrent(const Sequence<T>* initialTerms,
                                      const Sequence<T>* coefficients,int totalCount);

#include "RecurrentSequence.tpp"
