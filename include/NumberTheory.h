#pragma once

#include "lab2/Sequence.h"

Sequence<int>* Range(int l, int r);

bool IsPrime(const int& x);

// раскладывает на простые множители: 12 -> 2 2 3
Sequence<int>* Factorialize(int n);
