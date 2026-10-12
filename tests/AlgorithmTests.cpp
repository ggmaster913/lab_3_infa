#include <gtest/gtest.h>
#include <stdexcept>

#include "NumberTheory.h"
#include "RecurrentSequence.h"

// диапазон чисел: обычный случай и диапазон из одного элемента
TEST(NumberTheoryTest, RangeValues) {
    Sequence<int>* r1 = Range(2, 6);
    EXPECT_EQ(r1->GetLength(), 5);
    EXPECT_EQ(r1->Get(0), 2);
    EXPECT_EQ(r1->Get(1), 3);
    EXPECT_EQ(r1->Get(2), 4);
    EXPECT_EQ(r1->Get(3), 5);
    EXPECT_EQ(r1->Get(4), 6);
    delete r1;

    Sequence<int>* r2 = Range(5, 5);
    EXPECT_EQ(r2->GetLength(), 1);
    EXPECT_EQ(r2->Get(0), 5);
    delete r2;
}

// некорректный диапазон бросает исключение
TEST(NumberTheoryTest, RangeInvalidThrows) {
    EXPECT_THROW(Range(10, 2), std::out_of_range);
}

// проверка чисел на простоту
TEST(NumberTheoryTest, IsPrimeEdgeCases) {
    EXPECT_FALSE(IsPrime(-5));
    EXPECT_FALSE(IsPrime(0));
    EXPECT_FALSE(IsPrime(1));

    EXPECT_TRUE(IsPrime(2));
    EXPECT_TRUE(IsPrime(3));
    EXPECT_TRUE(IsPrime(17));
    EXPECT_TRUE(IsPrime(97));

    EXPECT_FALSE(IsPrime(4));
    EXPECT_FALSE(IsPrime(9));
    EXPECT_FALSE(IsPrime(25));
    EXPECT_FALSE(IsPrime(100));
}

// фильтрация диапазона через where и проверку на простоту
TEST(NumberTheoryTest, RangeFilteredByIsPrime) {
    Sequence<int>* range = Range(1, 20);
    Sequence<int>* primes = range->Where(IsPrime);

    EXPECT_EQ(primes->GetLength(), 8);
    EXPECT_EQ(primes->Get(0), 2);
    EXPECT_EQ(primes->Get(1), 3);
    EXPECT_EQ(primes->Get(2), 5);
    EXPECT_EQ(primes->Get(3), 7);
    EXPECT_EQ(primes->Get(4), 11);
    EXPECT_EQ(primes->Get(5), 13);
    EXPECT_EQ(primes->Get(6), 17);
    EXPECT_EQ(primes->Get(7), 19);

    delete range;
    delete primes;
}

// факторизация чисел на простые множители
TEST(NumberTheoryTest, FactorizeValues) {
    Sequence<int>* f12 = Factorialize(12);
    EXPECT_EQ(f12->GetLength(), 3);
    EXPECT_EQ(f12->Get(0), 2);
    EXPECT_EQ(f12->Get(1), 2);
    EXPECT_EQ(f12->Get(2), 3);
    delete f12;

    Sequence<int>* f7 = Factorialize(7);
    EXPECT_EQ(f7->GetLength(), 1);
    EXPECT_EQ(f7->Get(0), 7);
    delete f7;

    Sequence<int>* f16 = Factorialize(16);
    EXPECT_EQ(f16->GetLength(), 4);
    EXPECT_EQ(f16->Get(0), 2);
    EXPECT_EQ(f16->Get(1), 2);
    EXPECT_EQ(f16->Get(2), 2);
    EXPECT_EQ(f16->Get(3), 2);
    delete f16;
}

// числа меньше 2 нельзя факторизовать
TEST(NumberTheoryTest, FactorizeInvalidThrows) {
    EXPECT_THROW(Factorialize(1), std::invalid_argument);
    EXPECT_THROW(Factorialize(0), std::invalid_argument);
    EXPECT_THROW(Factorialize(-4), std::invalid_argument);
}

// --- recurrent sequences ---

// вычисление факториала через общую рекурренту
TEST(RecurrentTest, Factorial) {
    MutableArraySequence<long long> init;
    init.Append(1);

    Sequence<long long>* res = GenerateRecurrent(&init, 8, FactorialNext);

    EXPECT_EQ(res->GetLength(), 8);
    EXPECT_EQ(res->Get(0), 1);
    EXPECT_EQ(res->Get(1), 1);
    EXPECT_EQ(res->Get(2), 2);
    EXPECT_EQ(res->Get(3), 6);
    EXPECT_EQ(res->Get(4), 24);
    EXPECT_EQ(res->Get(5), 120);
    EXPECT_EQ(res->Get(6), 720);
    EXPECT_EQ(res->Get(7), 5040);

    delete res;
}

// числа фибоначчи через общую рекурренту
TEST(RecurrentTest, Fibonacci) {
    MutableArraySequence<long long> init;
    init.Append(0);
    init.Append(1);

    Sequence<long long>* res = GenerateRecurrent(&init, 10, FibonacciNext);

    EXPECT_EQ(res->GetLength(), 10);
    EXPECT_EQ(res->Get(0), 0);
    EXPECT_EQ(res->Get(1), 1);
    EXPECT_EQ(res->Get(2), 1);
    EXPECT_EQ(res->Get(3), 2);
    EXPECT_EQ(res->Get(4), 3);
    EXPECT_EQ(res->Get(5), 5);
    EXPECT_EQ(res->Get(6), 8);
    EXPECT_EQ(res->Get(7), 13);
    EXPECT_EQ(res->Get(8), 21);
    EXPECT_EQ(res->Get(9), 34);

    delete res;
}

// длина результата равна количеству начальных условий
TEST(RecurrentTest, TotalCountEqualsInitial) {
    MutableArraySequence<long long> init;
    init.Append(0);
    init.Append(1);

    Sequence<long long>* res = GenerateRecurrent(&init, 2, FibonacciNext);

    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(res->Get(0), 0);
    EXPECT_EQ(res->Get(1), 1);

    delete res;
}

// невалидные аргументы для generaterecurrent
TEST(RecurrentTest, GenerateInvalidArgsThrow) {
    MutableArraySequence<long long> init;
    init.Append(0);
    init.Append(1);

    EXPECT_THROW(GenerateRecurrent<long long>(nullptr, 5, FibonacciNext), std::invalid_argument);
    EXPECT_THROW(GenerateRecurrent<long long>(&init, 1, FibonacciNext), std::invalid_argument);
}

// проброс исключения из функции шага наверх
TEST(RecurrentTest, ExceptionFromNextPropagates) {
    MutableArraySequence<long long> init;
    init.Append(1);

    // fibonaccinext обратится к get(n - 2) при n = 1, что вызовет get(-1)
    EXPECT_THROW(GenerateRecurrent<long long>(&init, 5, FibonacciNext), std::out_of_range);
}

// линейная рекуррента второго порядка совпадает с рядом фибоначчи
TEST(RecurrentTest, LinearOrderTwoMatchesFibonacci) {
    MutableArraySequence<int> init;
    init.Append(0);
    init.Append(1);

    MutableArraySequence<int> coeffs;
    coeffs.Append(1);
    coeffs.Append(1);

    Sequence<int>* res = GenerateLinearRecurrent(&init, &coeffs, 10);

    EXPECT_EQ(res->GetLength(), 10);
    EXPECT_EQ(res->Get(0), 0);
    EXPECT_EQ(res->Get(1), 1);
    EXPECT_EQ(res->Get(2), 1);
    EXPECT_EQ(res->Get(3), 2);
    EXPECT_EQ(res->Get(4), 3);
    EXPECT_EQ(res->Get(5), 5);
    EXPECT_EQ(res->Get(6), 8);
    EXPECT_EQ(res->Get(7), 13);
    EXPECT_EQ(res->Get(8), 21);
    EXPECT_EQ(res->Get(9), 34);

    delete res;
}

// линейная рекуррента третьего порядка
TEST(RecurrentTest, LinearOrderThree) {
    MutableArraySequence<int> init;
    init.Append(1);
    init.Append(2);
    init.Append(3);

    MutableArraySequence<int> coeffs;
    coeffs.Append(2);
    coeffs.Append(1);
    coeffs.Append(-1);

    Sequence<int>* res = GenerateLinearRecurrent(&init, &coeffs, 6);

    EXPECT_EQ(res->GetLength(), 6);
    EXPECT_EQ(res->Get(0), 1);
    EXPECT_EQ(res->Get(1), 2);
    EXPECT_EQ(res->Get(2), 3);
    EXPECT_EQ(res->Get(3), 7);
    EXPECT_EQ(res->Get(4), 15);
    EXPECT_EQ(res->Get(5), 34);

    delete res;
}

// невалидные аргументы для generatelinearrecurrent
TEST(RecurrentTest, LinearInvalidArgsThrow) {
    MutableArraySequence<int> init3;
    init3.Append(1);
    init3.Append(2);
    init3.Append(3);

    MutableArraySequence<int> coeffs2;
    coeffs2.Append(1);
    coeffs2.Append(1);

    MutableArraySequence<int> emptyCoeffs;

    EXPECT_THROW(GenerateLinearRecurrent<int>(nullptr, &coeffs2, 5), std::invalid_argument);
    EXPECT_THROW(GenerateLinearRecurrent<int>(&init3, nullptr, 5), std::invalid_argument);
    EXPECT_THROW(GenerateLinearRecurrent<int>(&init3, &emptyCoeffs, 5), std::invalid_argument);
    EXPECT_THROW(GenerateLinearRecurrent<int>(&init3, &coeffs2, 5), std::invalid_argument);
    EXPECT_THROW(GenerateLinearRecurrent<int>(&coeffs2, &coeffs2, 1), std::invalid_argument);
}