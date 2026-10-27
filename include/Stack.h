#pragma once

#include "lab2/Sequence.h"
#include "lab2/ICollection.h"

template<typename T>
class Stack : public ICollection<T> {
    Sequence<T>* storage;

public:
    // чтоб нельзя было Size(seq): seq молча стал бы временным стеком и удалился
    explicit Stack(Sequence<T>* backing);
    Stack(const Stack<T>& other);

    // a = b = c
    Stack<T>& operator=(const Stack<T>& other);

    ~Stack() override;

    void Push(const T& value);
    T Pop();
    const T& Peek() const;
    bool IsEmpty() const;

    int GetCount() const override;
    const T& Get(int index) const override;
    Stack<T>* Copy() const override; // внутри обычный копирующий конструктор

    template<typename T2>
    Stack<T2>* Map(T2 (*f)(const T&)) const;
    Stack<T>* Where(bool (*predicate)(const T&)) const;
    template<typename T2>
    T2 Reduce(T2 (*f)(const T2&, const T&), const T2& initial) const;

    Stack<T>* Concat(const Stack<T>* other) const;
    Stack<T>* GetSubSequence(int startIndex, int endIndex) const;
    int FindSubsequence(const Stack<T>& pattern) const;

    Stack<T> operator+(const Stack<T>& other) const;
    bool operator==(const Stack<T>& other) const;
};

#include "Stack.tpp"
