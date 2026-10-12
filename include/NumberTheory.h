#ifndef NUMBERTHEORY_H
#define NUMBERTHEORY_H

#include "lab2/MutableArraySequence.h"
#include <stdexcept>

Sequence<int>* Range(int l, int r){
    if (l > r) throw std::out_of_range("неправильный диапазон");
    MutableArraySequence<int>* result = new MutableArraySequence<int>();
    for (int i = l; i <= r; i++){
        result->Append(i);
    }
    return result;
}

bool IsPrime(const int& x){
    if (x <= 1) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;

    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

Sequence<int>* Factorialize(int n){
    if (n <= 1) {
        throw std::invalid_argument("число для факторизации должно быть больше 1");
    }

    Sequence<int>* fullRange = Range(2, n);

    Sequence<int>* candidates = fullRange->Where(IsPrime);

    delete fullRange; // Range(2, n)->Where(IsPrime) так нельзя, тк утечка (new в Range не очищается)

    MutableArraySequence<int>* factors = new MutableArraySequence<int>();

    int current = n;
    for (int i = 0; i < candidates->GetLength(); ++i) {
        int prime = candidates->Get(i);
        
        while (current % prime == 0) {
            factors->Append(prime);
            current /= prime;
        }

        if (current == 1) {
            break;
        }
    }

    delete candidates; 
    return factors;
}

#endif 