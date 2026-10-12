#include <iostream>
#include <limits>
#include <random>
#include <chrono>
#include <stdexcept>
#include <cstdlib>

#include "Stack.h"
#include "NumberTheory.h"
#include "RecurrentSequence.h"
#include "lab2/MutableArraySequence.h"
#include "lab2/MutableListSequence.h"

// --- Функции-помощники ---

bool PredicateEven(const int& x) {
    return x % 2 == 0;
}

int TransformSquare(const int& x) {
    return x * x;
}

int AccumulateSum(const int& acc, const int& x) {
    return acc + x;
}

// --- Ввод и вывод ---

int ReadInt(const char* prompt) {
    int val;
    while (true) {
        if (prompt && prompt[0] != '\0') {
            std::cout << prompt;
        }
        if (std::cin >> val) {
            return val;
        }
        if (std::cin.eof()) {
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка: некорректный ввод. Ожидается целое число.\n";
    }
}

void OutputStack(const Stack<int>& stack) {
    int count = stack.GetCount();
    if (count == 0) {
        std::cout << "Стек пуст (размер: 0)\n";
        return;
    }
    std::cout << "Стек (размер " << count << "): [вершина] ";
    for (int i = 0; i < count; ++i) {
        std::cout << stack.Get(i);
        if (i + 1 < count) {
            std::cout << " -> ";
        }
    }
    std::cout << " [дно]\n";
}

template<typename T>
void OutputSequence(const Sequence<T>* seq) {
    if (seq == nullptr) {
        std::cout << "Последовательность: nullptr\n";
        return;
    }
    int len = seq->GetLength();
    std::cout << "Последовательность (длина " << len << "): [";
    for (int i = 0; i < len; ++i) {
        std::cout << seq->Get(i);
        if (i + 1 < len) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

// --- Создание стека ---

Stack<int>* BuildStackInteractive() {
    std::cout << "\n--- Создание стека ---\n";
    std::cout << "Выберите хранилище:\n";
    std::cout << "1. Динамический массив (MutableArraySequence)\n";
    std::cout << "2. Связный список (MutableListSequence)\n";
    int storageChoice = ReadInt("Ваш выбор: ");
    while (storageChoice != 1 && storageChoice != 2) {
        std::cout << "Некорректный выбор. Введите 1 или 2.\n";
        storageChoice = ReadInt("Ваш выбор: ");
    }

    Sequence<int>* backing = nullptr;
    if (storageChoice == 1) {
        backing = new MutableArraySequence<int>();
    } else {
        backing = new MutableListSequence<int>();
    }

    Stack<int>* stack = new Stack<int>(backing);

    std::cout << "Способ заполнения:\n";
    std::cout << "1. Пустой стек\n";
    std::cout << "2. Ручной ввод\n";
    std::cout << "3. Случайные числа\n";
    int fillChoice = ReadInt("Ваш выбор: ");
    while (fillChoice < 1 || fillChoice > 3) {
        std::cout << "Некорректный выбор. Введите число от 1 до 3.\n";
        fillChoice = ReadInt("Ваш выбор: ");
    }

    if (fillChoice == 2) {
        int count = ReadInt("Количество элементов (0..1000): ");
        while (count < 0 || count > 1000) {
            std::cout << "Количество должно быть в диапазоне от 0 до 1000.\n";
            count = ReadInt("Количество элементов: ");
        }
        for (int i = 0; i < count; ++i) {
            int val = ReadInt("Элемент: ");
            stack->Push(val);
        }
    } else if (fillChoice == 3) {
        int count = ReadInt("Количество элементов (0..1000): ");
        while (count < 0 || count > 1000) {
            std::cout << "Количество должно быть в диапазоне от 0 до 1000.\n";
            count = ReadInt("Количество элементов: ");
        }
        int minVal = 0, maxVal = 0;
        while (true) {
            minVal = ReadInt("Минимум диапазона: ");
            maxVal = ReadInt("Максимум диапазона: ");
            if (minVal <= maxVal) {
                break;
            }
            std::cout << "Ошибка: минимум не может быть больше максимума. Повторите ввод.\n";
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(minVal, maxVal);
        for (int i = 0; i < count; ++i) {
            stack->Push(dist(gen));
        }
    }

    std::cout << "Созданный стек:\n";
    OutputStack(*stack);
    return stack;
}

// --- Меню стека ---

void ExecuteStackMenu() {
    Stack<int>* current = BuildStackInteractive();

    while (true) {
        std::cout << "\n--- Меню операций со стеком ---\n";
        std::cout << "1. Push (добавить элемент)\n";
        std::cout << "2. Pop (снять элемент с вершины)\n";
        std::cout << "3. Peek (посмотреть вершину)\n";
        std::cout << "4. Get по индексу (от вершины)\n";
        std::cout << "5. Map (возведение в квадрат)\n";
        std::cout << "6. Where (оставить только четные)\n";
        std::cout << "7. Reduce (сумма элементов)\n";
        std::cout << "8. Concat и operator+ со вторым стеком\n";
        std::cout << "9. GetSubSequence (подпоследовательность)\n";
        std::cout << "10. FindSubsequence (поиск образца)\n";
        std::cout << "11. operator== со вторым стеком\n";
        std::cout << "0. Назад\n";

        int command = ReadInt("Выберите команду: ");
        if (command == 0) {
            break;
        }

        try {
            switch (command) {
                case 1: {
                    int val = ReadInt("Введите число для добавления: ");
                    std::cout << "Стек до операции:\n";
                    OutputStack(*current);
                    current->Push(val);
                    std::cout << "Стек после Push:\n";
                    OutputStack(*current);
                    break;
                }
                case 2: {
                    std::cout << "Стек до операции:\n";
                    OutputStack(*current);
                    int popped = current->Pop();
                    std::cout << "Снятое значение: " << popped << "\n";
                    std::cout << "Стек после Pop:\n";
                    OutputStack(*current);
                    break;
                }
                case 3: {
                    std::cout << "Текущий стек:\n";
                    OutputStack(*current);
                    int topVal = current->Peek();
                    std::cout << "Значение на вершине (Peek): " << topVal << "\n";
                    break;
                }
                case 4: {
                    int idx = ReadInt("Введите индекс от вершины: ");
                    std::cout << "Текущий стек:\n";
                    OutputStack(*current);
                    int val = current->Get(idx);
                    std::cout << "Элемент по индексу " << idx << ": " << val << "\n";
                    break;
                }
                case 5: {
                    std::cout << "Исходный стек:\n";
                    OutputStack(*current);
                    Stack<int>* mapped = current->Map(TransformSquare);
                    std::cout << "Результат Map (возведение в квадрат):\n";
                    OutputStack(*mapped);
                    delete mapped;
                    break;
                }
                case 6: {
                    std::cout << "Исходный стек:\n";
                    OutputStack(*current);
                    Stack<int>* filtered = current->Where(PredicateEven);
                    std::cout << "Результат Where (только четные):\n";
                    OutputStack(*filtered);
                    delete filtered;
                    break;
                }
                case 7: {
                    std::cout << "Исходный стек:\n";
                    OutputStack(*current);
                    int sum = current->Reduce<int>(AccumulateSum, 0);
                    std::cout << "Результат Reduce (сумма элементов): " << sum << "\n";
                    break;
                }
                case 8: {
                    std::cout << "Создание второго стека:\n";
                    Stack<int>* second = nullptr;
                    Stack<int>* concatted = nullptr;
                    try {
                        second = BuildStackInteractive();
                        std::cout << "\nПервый стек:\n";
                        OutputStack(*current);
                        std::cout << "Второй стек:\n";
                        OutputStack(*second);

                        concatted = current->Concat(second);
                        std::cout << "Результат Concat:\n";
                        OutputStack(*concatted);

                        Stack<int> sumStack = (*current) + (*second);
                        std::cout << "Результат operator+:\n";
                        OutputStack(sumStack);

                        delete concatted;
                        delete second;
                    } catch (...) {
                        delete concatted;
                        delete second;
                        throw;
                    }
                    break;
                }
                case 9: {
                    int startIdx = ReadInt("Начальный индекс (от вершины): ");
                    int endIdx = ReadInt("Конечный индекс (от вершины): ");
                    std::cout << "Исходный стек:\n";
                    OutputStack(*current);
                    Stack<int>* sub = current->GetSubSequence(startIdx, endIdx);
                    std::cout << "Результат GetSubSequence:\n";
                    OutputStack(*sub);
                    delete sub;
                    break;
                }
                case 10: {
                    std::cout << "Создание стека-образца:\n";
                    Stack<int>* pattern = nullptr;
                    try {
                        pattern = BuildStackInteractive();
                        std::cout << "\nОсновной стек:\n";
                        OutputStack(*current);
                        std::cout << "Стек-образец:\n";
                        OutputStack(*pattern);

                        int pos = current->FindSubsequence(*pattern);
                        if (pos != -1) {
                            std::cout << "Образец найден на позиции от вершины: " << pos << "\n";
                        } else {
                            std::cout << "Образец не найден (-1)\n";
                        }
                        delete pattern;
                    } catch (...) {
                        delete pattern;
                        throw;
                    }
                    break;
                }
                case 11: {
                    std::cout << "Создание второго стека для сравнения:\n";
                    Stack<int>* second = nullptr;
                    try {
                        second = BuildStackInteractive();
                        std::cout << "\nПервый стек:\n";
                        OutputStack(*current);
                        std::cout << "Второй стек:\n";
                        OutputStack(*second);

                        if (*current == *second) {
                            std::cout << "Результат: Стеки равны\n";
                        } else {
                            std::cout << "Результат: Стеки не равны\n";
                        }
                        delete second;
                    } catch (...) {
                        delete second;
                        throw;
                    }
                    break;
                }
                default: {
                    std::cout << "Некорректная команда. Введите число от 0 до 11.\n";
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }

    delete current;
}

// --- Меню теории чисел ---

void ExecuteNumberTheoryMenu() {
    while (true) {
        std::cout << "\n--- Меню теории чисел ---\n";
        std::cout << "1. Проверить число на простоту (IsPrime)\n";
        std::cout << "2. Простые числа на отрезке [l, r]\n";
        std::cout << "3. Разложить число на множители (Factorialize)\n";
        std::cout << "0. Назад\n";

        int command = ReadInt("Выберите команду: ");
        if (command == 0) {
            break;
        }

        try {
            switch (command) {
                case 1: {
                    int x = ReadInt("Введите число для проверки: ");
                    if (IsPrime(x)) {
                        std::cout << "Число " << x << " является простым.\n";
                    } else {
                        std::cout << "Число " << x << " не является простым.\n";
                    }
                    break;
                }
                case 2: {
                    int l = ReadInt("Левая граница отрезка l: ");
                    int r = ReadInt("Правая граница отрезка r: ");
                    if (l > r) {
                        throw std::out_of_range("левая граница не может быть больше правой");
                    }
                    if (r - l + 1 > 50000) {
                        throw std::invalid_argument("длина отрезка не должна превышать 50 000");
                    }

                    Sequence<int>* range = Range(l, r);
                    Sequence<int>* primes = range->Where(IsPrime);

                    std::cout << "Найдено простых чисел: " << primes->GetLength() << "\n";
                    OutputSequence(primes);

                    delete range;
                    delete primes;
                    break;
                }
                case 3: {
                    int n = ReadInt("Введите число для факторизации (2..50000): ");
                    if (n < 2 || n > 50000) {
                        throw std::invalid_argument("число должно быть в диапазоне от 2 до 50 000");
                    }

                    Sequence<int>* factors = Factorialize(n);

                    std::cout << n << " = ";
                    int fLen = factors->GetLength();
                    for (int i = 0; i < fLen; ++i) {
                        std::cout << factors->Get(i);
                        if (i + 1 < fLen) {
                            std::cout << " * ";
                        }
                    }
                    std::cout << "\n";

                    delete factors;
                    break;
                }
                default: {
                    std::cout << "Некорректная команда. Введите число от 0 до 3.\n";
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

// --- Меню рекуррент ---

void ExecuteRecurrentMenu() {
    while (true) {
        std::cout << "\n--- Меню рекуррентных последовательностей ---\n";
        std::cout << "1. Факториалы (FactorialNext)\n";
        std::cout << "2. Числа Фибоначчи (FibonacciNext)\n";
        std::cout << "3. Линейная рекуррентная последовательность\n";
        std::cout << "0. Назад\n";

        int command = ReadInt("Выберите команду: ");
        if (command == 0) {
            break;
        }

        try {
            switch (command) {
                case 1: {
                    int count = ReadInt("Количество элементов (1..21): ");
                    if (count < 1 || count > 21) {
                        throw std::invalid_argument("количество должно быть от 1 до 21 (предел типа long long)");
                    }

                    MutableArraySequence<long long> init;
                    init.Append(1);

                    Sequence<long long>* res = GenerateRecurrent<long long>(&init, count, FactorialNext);
                    std::cout << "Сгенерированная последовательность факториалов:\n";
                    OutputSequence(res);
                    delete res;
                    break;
                }
                case 2: {
                    int count = ReadInt("Количество элементов (2..93): ");
                    if (count < 2 || count > 93) {
                        throw std::invalid_argument("количество должно быть от 2 до 93 (предел типа long long)");
                    }

                    MutableArraySequence<long long> init;
                    init.Append(0);
                    init.Append(1);

                    Sequence<long long>* res = GenerateRecurrent<long long>(&init, count, FibonacciNext);
                    std::cout << "Сгенерированная последовательность чисел Фибоначчи:\n";
                    OutputSequence(res);
                    delete res;
                    break;
                }
                case 3: {
                    std::cout << "Внимание: при больших коэффициентах возможно переполнение типа long long!\n";
                    int k = ReadInt("Порядок рекурренты k (1..10): ");
                    if (k < 1 || k > 10) {
                        throw std::invalid_argument("порядок должен быть от 1 до 10");
                    }

                    MutableArraySequence<long long> coeffs;
                    std::cout << "Введите " << k << " коэффициентов (c_0 .. c_{k-1}):\n";
                    for (int i = 0; i < k; ++i) {
                        coeffs.Append(ReadInt("Коэффициент: "));
                    }

                    MutableArraySequence<long long> init;
                    std::cout << "Введите " << k << " начальных членов (x_0 .. x_{k-1}):\n";
                    for (int i = 0; i < k; ++i) {
                        init.Append(ReadInt("Начальный член: "));
                    }

                    int totalCount = ReadInt("Общее количество элементов (k..600): ");
                    if (totalCount < k || totalCount > 600) {
                        throw std::invalid_argument("общее количество должно быть от k до 600");
                    }

                    Sequence<long long>* res = GenerateLinearRecurrent<long long>(&init, &coeffs, totalCount);
                    std::cout << "Результат генерации:\n";
                    OutputSequence(res);
                    delete res;
                    break;
                }
                default: {
                    std::cout << "Некорректная команда. Введите число от 0 до 3.\n";
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
}

// --- Тест производительности ---

void ExecuteBenchmark() {
    std::cout << "\n--- Тест производительности ---\n";
    int n = ReadInt("Введите N (количество операций, 1..10000): ");
    if (n < 1 || n > 10000) {
        std::cout << "Ошибка: N должно быть в диапазоне от 1 до 10 000.\n";
        return;
    }

    try {
        // 1. Стек на базе динамического массива
        Stack<int> arrayStack(new MutableArraySequence<int>());

        auto startArrPush = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            arrayStack.Push(i);
        }
        auto endArrPush = std::chrono::high_resolution_clock::now();
        double arrPushMs = std::chrono::duration<double, std::milli>(endArrPush - startArrPush).count();

        auto startArrPop = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            arrayStack.Pop();
        }
        auto endArrPop = std::chrono::high_resolution_clock::now();
        double arrPopMs = std::chrono::duration<double, std::milli>(endArrPop - startArrPop).count();

        // 2. Стек на базе связного списка
        Stack<int> listStack(new MutableListSequence<int>());

        auto startListPush = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            listStack.Push(i);
        }
        auto endListPush = std::chrono::high_resolution_clock::now();
        double listPushMs = std::chrono::duration<double, std::milli>(endListPush - startListPush).count();

        auto startListPop = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            listStack.Pop();
        }
        auto endListPop = std::chrono::high_resolution_clock::now();
        double listPopMs = std::chrono::duration<double, std::milli>(endListPop - startListPop).count();

        std::cout << "\nРезультаты замеров времени для N = " << n << ":\n";
        std::cout << "1. Стек на массиве (MutableArraySequence):\n";
        std::cout << "   Время Push: " << arrPushMs << " мс\n";
        std::cout << "   Время Pop:  " << arrPopMs << " мс\n";
        std::cout << "2. Стек на списке (MutableListSequence):\n";
        std::cout << "   Время Push: " << listPushMs << " мс\n";
        std::cout << "   Время Pop:  " << listPopMs << " мс\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка при выполнении теста: " << e.what() << "\n";
    }
}

// --- Главная функция ---

int main() {
    while (true) {
        std::cout << "\n==============================\n";
        std::cout << "    ЛАБОРАТОРНАЯ РАБОТА 3     \n";
        std::cout << "==============================\n";
        std::cout << "1. Стек\n";
        std::cout << "2. Простые числа и разложение на множители\n";
        std::cout << "3. Рекуррентные последовательности\n";
        std::cout << "4. Тест производительности\n";
        std::cout << "0. Выход\n";

        int command = ReadInt("Выберите раздел: ");
        if (command == 0) {
            std::cout << "Выход из программы.\n";
            break;
        }

        switch (command) {
            case 1:
                ExecuteStackMenu();
                break;
            case 2:
                ExecuteNumberTheoryMenu();
                break;
            case 3:
                ExecuteRecurrentMenu();
                break;
            case 4:
                ExecuteBenchmark();
                break;
            default:
                std::cout << "Некорректная команда. Введите число от 0 до 4.\n";
                break;
        }
    }

    return 0;
}