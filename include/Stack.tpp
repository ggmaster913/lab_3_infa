#pragma once

#include "Stack.h"
#include "lab2/MutableArraySequence.h"
#include "lab2/DynamicArray.h"
#include <stdexcept>

template<typename T>
Stack<T>::Stack(Sequence<T>* backing){
    if (backing == nullptr) throw std::invalid_argument("нулевой указатель");
    storage = backing;
}

template<typename T>
Stack<T>::Stack(const Stack<T>& other){
    // GetSubSequence не умеет пустой диапазон, пустой делаем сами
    if (other.IsEmpty()) {
        DynamicArray<T> emptyData(0);
        this->storage = new MutableArraySequence<T>(emptyData);
    }else {
        this->storage = other.storage->GetSubSequence(0, other.storage->GetLength() - 1);
    }
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other){
    if (this == &other) return *this; //чтоб не делать лишнюю копию

    Sequence<T>* newStorage = nullptr;
    if (other.IsEmpty()) {
        DynamicArray<T> emptyData(0);
        newStorage = new MutableArraySequence<T>(emptyData);
    }else {
        newStorage = other.storage->GetSubSequence(0, other.storage->GetLength() - 1);
    }

    delete storage; // старое удаляем только когда новое готово
    storage = newStorage;

    return *this;
}

template<typename T>
Stack<T>::~Stack() {
    delete storage;
}

template<typename T>
void Stack<T>::Push(const T& value){
    Sequence<T>* next = storage->Append(value);
    if (next != storage) {
        delete storage; 
        //это для иммутабл
    }
    storage = next;
}

template<typename T>
T Stack<T>::Pop(){
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

template<typename T>
const T& Stack<T>::Peek() const{
    if (this->IsEmpty()) throw std::out_of_range("стек пуст");
    return storage->GetLast(); // вершина = последний эл storage
}

template<typename T>
bool Stack<T>::IsEmpty() const{
    return (storage->GetLength() == 0);
}

template<typename T>
int Stack<T>::GetCount() const {
    return storage->GetLength();
}

template<typename T>
Stack<T>* Stack<T>::Copy() const {
    return new Stack<T>(*this);
}

template<typename T>
const T& Stack<T>::Get(int index) const {
    if (index < 0 || index >= storage->GetLength()) throw std::out_of_range("неправильный индекс");
    return storage->Get(storage->GetLength() - index - 1); // индекс считаем от вершины, тк вершина в конце
}

template<typename T>
template<typename T2>
Stack<T2>* Stack<T>::Map(T2 (*f)(const T&)) const{
    Sequence<T2>* mappedStorage = storage->Map(f); // новая последовательность из f(эл), всегда мутаблэррэйсиквенс
    return new Stack<T2>(mappedStorage);
}

template<typename T>
Stack<T>* Stack<T>::Where(bool (*predicate)(const T&)) const{
    Sequence<T>* filteredStorage = storage->Where(predicate);
    return new Stack<T>(filteredStorage);
}

template<typename T>
template<typename T2>
T2 Stack<T>::Reduce(T2 (*f)(const T2&, const T&), const T2& initial) const{
    return storage->Reduce(f, initial);
}

template<typename T>
Stack<T>* Stack<T>::Concat(const Stack<T>* other) const{
    if (other == nullptr) throw std::invalid_argument("неправильный аргумент");

    Sequence<T>* newStorage = nullptr;
    if (this->IsEmpty()) {
        DynamicArray<T> emptyData(0);
        newStorage = new MutableArraySequence<T>(emptyData);
    }else {
        newStorage = storage->GetSubSequence(0, storage->GetLength() - 1); // копия, тк Concat у мутабл дописал бы прямо в this
    }

    Sequence<T>* joined = newStorage->Concat(other->storage);
    if (joined != newStorage) {
        delete newStorage; // для иммутабл: удаляем копию
    }
    return new Stack<T>(joined);
}

template<typename T>
Stack<T>* Stack<T>::GetSubSequence(int startIndex, int endIndex) const{
    if (startIndex < 0 || endIndex >= storage->GetLength() || startIndex > endIndex) {
        throw std::out_of_range("неправильный индекс");
    }

    // индексы от вершины переводим в индексы storage, поэтому начало и конец меняются местами
    Sequence<T>* sub = storage->GetSubSequence(storage->GetLength()- endIndex - 1 ,storage->GetLength() - startIndex - 1 );
    return new Stack<T>(sub);
}

template<typename T>
int Stack<T>::FindSubsequence(const Stack<T>& pattern) const{

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

template<typename T>
Stack<T> Stack<T>::operator+(const Stack<T>& other) const{
    Stack<T>* concatenated = this->Concat(&other);
    Stack<T> result(*concatenated);
    delete concatenated;
    return result;
}

template<typename T>
bool Stack<T>::operator==(const Stack<T>& other) const {
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
