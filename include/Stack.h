#ifndef STACK_H
#define STACK_H

#include <stdexcept>

#include "lab2/Sequence.h"
#include "lab2/ICollection.h"
#include "lab2/MutableArraySequence.h"
#include "lab2/DynamicArray.h"

template<typename T>
class Stack : public ICollection<T> {
    Sequence<T>* storage;

public:
    explicit Stack(Sequence<T>* backing){
        if (backing == nullptr) throw std::invalid_argument("нулевой указатель");
        storage = backing;
    }

    Stack(const Stack<T>& other){
        if (other.IsEmpty()) {
            DynamicArray<T> emptyData(0);
            this->storage = new MutableArraySequence<T>(emptyData);
        }else {
            this->storage = other.storage->GetSubSequence(0, other.storage->GetLength() - 1);
        }
    }

    Stack<T>& operator=(const Stack<T>& other){
        if (this == &other) return *this;

        delete storage;

        if (other.IsEmpty()) {
            DynamicArray<T> emptyData(0);
            this->storage = new MutableArraySequence<T>(emptyData);
        }else {
            this->storage = other.storage->GetSubSequence(0, other.storage->GetLength() - 1);
        }

        return *this;
    }

    ~Stack() override {
        delete storage;
    }

    void Push(const T& value){
        Sequence<T>* next = storage->Append(value);
        if (next != storage) {
            delete storage; // Освобождаем старый неизменяемый объект
            //это для иммутабл, чтоб небыло утечки при storage = storage->Append(value);
        }
        storage = next;
    }
    

    T Pop(){
        if (this->IsEmpty()) throw std::out_of_range("стек пуст");
        T temp = this->Peek();
        int newLen = storage->GetLength() - 1;
        Sequence<T>* newStorage = nullptr;
        if (newLen > 0) { 
            newStorage = storage->GetSubSequence(0, newLen - 1);
        }else {
            DynamicArray<T> emptyData(0);
            newStorage = new MutableArraySequence<T>(emptyData);
        }
        delete storage;
        storage = newStorage;
        return temp;
    }

    const T& Peek() const{
        if (this->IsEmpty()) throw std::out_of_range("стек пуст");
        return storage->GetLast();
    }

    bool IsEmpty() const{
        return (storage->GetLength() == 0);
    }

    int GetCount() const override { 
        return storage->GetLength(); 
    }
    
    const T& Get(int index) const override {
        if (index < 0 || index >= storage->GetLength()) throw std::out_of_range("неправильный индекс");
        return storage->Get(storage->GetLength() - index - 1);
    }

    template<typename T2>
    Stack<T2>* Map(T2 (*f)(const T&)) const{
        Sequence<T2>* mappedStorage = storage->Map(f);
        return new Stack<T2>(mappedStorage);
    }

    Stack<T>* Where(bool (*predicate)(const T&)) const{
        Sequence<T>* filteredStorage = storage->Where(predicate);
        return new Stack<T>(filteredStorage);
    }

    template<typename T2>
    T2 Reduce(T2 (*f)(const T2&, const T&), const T2& initial) const{
        return storage->Reduce(f, initial);
    }

    Stack<T>* Concat(const Stack<T>* other) const{
        if (other == nullptr) throw std::invalid_argument("неправильный аргумент");

        Sequence<T>* newStorage = nullptr;
        if (this->IsEmpty()) {
            DynamicArray<T> emptyData(0);
            newStorage = new MutableArraySequence<T>(emptyData);
        }else {
            newStorage = storage->GetSubSequence(0, storage->GetLength() - 1);
        }

        newStorage = newStorage->Concat(other->storage);
        return new Stack<T>(newStorage);
    }

    Stack<T>* GetSubSequence(int startIndex, int endIndex) const{
        if (startIndex < 0 || endIndex >= storage->GetLength() || startIndex > endIndex) {
            throw std::out_of_range("неправильный индекс");
        }

        Sequence<T>* sub = storage->GetSubSequence(storage->GetLength()- endIndex - 1 ,storage->GetLength() - startIndex - 1 );
        return new Stack<T>(sub);
    }

    int FindSubsequence(const Stack<T>& pattern) const{

        int patternLen = pattern.GetCount();
        int mainLen = this->GetCount();

        if (patternLen == 0) return 0; // входит полностью 
        if (patternLen > mainLen) return -1; // больше 

        for (int i = 0; i <= mainLen - patternLen; i++) {
            bool match = true;
            for (int j = 0; j < patternLen; j++) {
                if (!(this->Get(i + j) == pattern.Get(j))) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
    return -1;
    }

    Stack<T> operator+(const Stack<T>& other) const{  
        Stack<T>* concatenated = this->Concat(&other);
        Stack<T> result(*concatenated);
        delete concatenated;
        return result;
    } 

    bool operator==(const Stack<T>& other) const {
    if (storage->GetLength() != other.storage->GetLength()) {
        return false;
    }
    for (int i = 0; i < storage->GetLength(); i++) {
        if (!(storage->Get(i) == other.storage->Get(i))) {
            return false;
        }
    }
    return true;
}
};

#endif
