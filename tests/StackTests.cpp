#include <gtest/gtest.h>
#include <stdexcept>

#include "Stack.h"
#include "lab2/MutableArraySequence.h"
#include "lab2/MutableListSequence.h"
#include "lab2/ImmutableArraySequence.h"


// функции-помощники
static bool isEven(const int& x) { return x % 2 == 0; }
static int square(const int& x) { return x * x; }
static int sumHelper(const int& acc, const int& x) { return acc + x; }

TEST(StackTest, NewStackEmpty) {
    Stack<int> exampl(new MutableArraySequence<int>()); 
    EXPECT_EQ(exampl.IsEmpty(), true);
    EXPECT_EQ(exampl.GetCount(), 0);
}

TEST(StackTest, PushPopLifoOrder) {
    Stack<int> exampl(new MutableArraySequence<int>());
    exampl.Push(1);
    exampl.Push(2);
    exampl.Push(3);
    EXPECT_EQ(exampl.GetCount(), 3);
    EXPECT_EQ(exampl.Peek(), 3);
    EXPECT_EQ(exampl.Pop(), 3);
    EXPECT_EQ(exampl.Pop(), 2);
    EXPECT_EQ(exampl.Pop(), 1);
    EXPECT_EQ(exampl.IsEmpty(), true);


}    

// пустой стек бросает при pop и peek
TEST(StackTest, PopAndPeekOnEmptyThrow) {
    Stack<int> s(new MutableArraySequence<int>());
    EXPECT_THROW(s.Pop(), std::out_of_range);
    EXPECT_THROW(s.Peek(), std::out_of_range);
}

// проверка на nullptr в конструкторе
TEST(StackTest, ConstructorNullptrThrows) {
    EXPECT_THROW(Stack<int>(nullptr), std::invalid_argument);
}

// отсчет элементов от вершины и границы
TEST(StackTest, GetCountsFromTopAndBounds) {
    Stack<int> s(new MutableArraySequence<int>());
    s.Push(1);
    s.Push(2);
    s.Push(3);

    EXPECT_EQ(s.Get(0), 3);
    EXPECT_EQ(s.Get(1), 2);
    EXPECT_EQ(s.Get(2), 1);
    EXPECT_EQ(s.Get(0), s.Peek());

    EXPECT_THROW(s.Get(-1), std::out_of_range);
    EXPECT_THROW(s.Get(3), std::out_of_range);
}

// стек на связном списке и на иммутабельном массиве
TEST(StackTest, WorksOnListAndImmutableBacking) {
    Stack<int> listStack(new MutableListSequence<int>());
    listStack.Push(1);
    listStack.Push(2);
    listStack.Push(3);
    EXPECT_EQ(listStack.Pop(), 3);
    EXPECT_EQ(listStack.Pop(), 2);
    EXPECT_EQ(listStack.Pop(), 1);

    Stack<int> immutStack(new ImmutableArraySequence<int>());
    immutStack.Push(1);
    immutStack.Push(2);
    immutStack.Push(3);
    EXPECT_EQ(immutStack.Pop(), 3);
    EXPECT_EQ(immutStack.Pop(), 2);
    EXPECT_EQ(immutStack.Pop(), 1);
}

// полиморфизм через интерфейс icollection
TEST(StackTest, ICollectionPolymorphism) {
    Stack<int> s(new MutableArraySequence<int>());
    s.Push(1);
    s.Push(2);
    s.Push(3);

    ICollection<int>* col = &s;
    EXPECT_EQ(col->GetCount(), 3);
    EXPECT_EQ(col->Get(0), 3);
}

// конструктор копирования: копия независима от оригинала
TEST(StackTest, CopyConstructorIndependence) {
    Stack<int> a(new MutableArraySequence<int>());
    a.Push(1);
    a.Push(2);
    a.Push(3);

    Stack<int> b = a;
    EXPECT_EQ(b.GetCount(), 3);
    EXPECT_EQ(b.Peek(), 3);

    a.Push(99);
    EXPECT_EQ(b.GetCount(), 3);
    EXPECT_EQ(b.Peek(), 3);

    b.Pop();
    EXPECT_EQ(a.GetCount(), 4);

    Stack<int> empty(new MutableArraySequence<int>());
    Stack<int> emptyCopy = empty;
    EXPECT_TRUE(emptyCopy.IsEmpty());
    EXPECT_EQ(emptyCopy.GetCount(), 0);
}

// оператор присваивания и самоприсваивание
TEST(StackTest, AssignmentIndependenceAndSelfAssign) {
    Stack<int> a(new MutableArraySequence<int>());
    a.Push(1);
    a.Push(2);

    Stack<int> c(new MutableArraySequence<int>());
    c.Push(10);
    c.Push(20);
    c.Push(30);

    c = a;
    EXPECT_EQ(c.GetCount(), 2);
    EXPECT_EQ(c.Peek(), 2);

    a.Push(99);
    EXPECT_EQ(c.GetCount(), 2);
    EXPECT_EQ(c.Peek(), 2);

    Stack<int> empty(new MutableArraySequence<int>());
    c = empty;
    EXPECT_TRUE(c.IsEmpty());
    EXPECT_EQ(c.GetCount(), 0);

    a = a;
    EXPECT_EQ(a.GetCount(), 3);
    EXPECT_EQ(a.Peek(), 99);
}

// map, where и reduce
TEST(StackTest, MapWhereReduce) {
    Stack<int> s(new MutableArraySequence<int>());
    s.Push(1);
    s.Push(2);
    s.Push(3);
    s.Push(4);

    Stack<int>* mapped = s.Map(square);
    EXPECT_EQ(mapped->GetCount(), 4);
    EXPECT_EQ(mapped->Peek(), 16);

    Stack<int>* filtered = s.Where(isEven);
    EXPECT_EQ(filtered->GetCount(), 2);
    EXPECT_EQ(filtered->Peek(), 4);

    int sum = s.Reduce<int>(sumHelper, 0);
    EXPECT_EQ(sum, 10);

    Stack<int> odd(new MutableArraySequence<int>());
    odd.Push(1);
    odd.Push(3);
    Stack<int>* emptyFiltered = odd.Where(isEven);
    EXPECT_TRUE(emptyFiltered->IsEmpty());
    EXPECT_EQ(emptyFiltered->GetCount(), 0);

    delete mapped;
    delete filtered;
    delete emptyFiltered;
}

// concat: длины, пустые стеки и nullptr
TEST(StackTest, ConcatLengthsEmptyAndNullptr) {
    Stack<int> a(new MutableArraySequence<int>());
    a.Push(1);
    a.Push(2);
    a.Push(3);

    Stack<int> b(new MutableArraySequence<int>());
    b.Push(10);
    b.Push(20);

    Stack<int>* res1 = a.Concat(&b);
    EXPECT_EQ(res1->GetCount(), 5);
    EXPECT_EQ(res1->Peek(), 20);

    Stack<int> empty(new MutableArraySequence<int>());

    Stack<int>* res2 = a.Concat(&empty);
    EXPECT_EQ(res2->GetCount(), 3);

    Stack<int>* res3 = empty.Concat(&a);
    EXPECT_EQ(res3->GetCount(), 3);

    EXPECT_THROW(a.Concat(nullptr), std::invalid_argument);

    delete res1;
    delete res2;
    delete res3;
}

// срез подпоследовательности с вершины и ошибки индексов
TEST(StackTest, GetSubSequenceFromTopAndErrors) {
    Stack<int> s(new MutableArraySequence<int>());
    for (int i = 1; i <= 5; ++i) {
        s.Push(i);
    }

    Stack<int>* sub1 = s.GetSubSequence(0, 1);
    EXPECT_EQ(sub1->GetCount(), 2);
    EXPECT_EQ(sub1->Get(0), 5);
    EXPECT_EQ(sub1->Get(1), 4);

    Stack<int>* sub2 = s.GetSubSequence(4, 4);
    EXPECT_EQ(sub2->GetCount(), 1);
    EXPECT_EQ(sub2->Get(0), 1);

    EXPECT_THROW(s.GetSubSequence(-1, 2), std::out_of_range);
    EXPECT_THROW(s.GetSubSequence(0, 5), std::out_of_range);
    EXPECT_THROW(s.GetSubSequence(3, 1), std::out_of_range);

    delete sub1;
    delete sub2;
}

// поиск подпоследовательности 
TEST(StackTest, FindSubsequenceCases) {
    Stack<int> s(new MutableArraySequence<int>());
    for (int i = 1; i <= 5; ++i) {
        s.Push(i);
    }

    Stack<int> p1(new MutableArraySequence<int>());
    p1.Push(4);
    p1.Push(5);
    EXPECT_EQ(s.FindSubsequence(p1), 0);

    Stack<int> p2(new MutableArraySequence<int>());
    p2.Push(2);
    p2.Push(3);
    EXPECT_EQ(s.FindSubsequence(p2), 2);

    Stack<int> p3(new MutableArraySequence<int>());
    p3.Push(7);
    p3.Push(8);
    EXPECT_EQ(s.FindSubsequence(p3), -1);

    Stack<int> pLong(new MutableArraySequence<int>());
    for (int i = 1; i <= 6; ++i) {
        pLong.Push(i);
    }
    EXPECT_EQ(s.FindSubsequence(pLong), -1);

    Stack<int> pEmpty(new MutableArraySequence<int>());
    EXPECT_EQ(s.FindSubsequence(pEmpty), 0);
}

// оператор + дает такой же результат, как concat
TEST(StackTest, OperatorPlusMatchesConcat) {
    Stack<int> a(new MutableArraySequence<int>());
    a.Push(1);
    a.Push(2);

    Stack<int> b(new MutableArraySequence<int>());
    b.Push(3);
    b.Push(4);

    Stack<int> sum = a + b;
    EXPECT_EQ(sum.GetCount(), 4);
    EXPECT_EQ(sum.Peek(), 4);

    Stack<int>* res = a.Concat(&b);
    EXPECT_TRUE(sum == *res);

    EXPECT_EQ(a.GetCount(), 2);
    EXPECT_EQ(b.GetCount(), 2);

    delete res;
}

// оператор == для проверки равенства стеков
TEST(StackTest, OperatorEquals) {
    Stack<int> s1(new MutableArraySequence<int>());
    s1.Push(1);
    s1.Push(2);
    s1.Push(3);

    Stack<int> s2(new MutableArraySequence<int>());
    s2.Push(1);
    s2.Push(2);
    s2.Push(3);
    EXPECT_TRUE(s1 == s2);

    Stack<int> s3(new MutableArraySequence<int>());
    s3.Push(1);
    s3.Push(2);
    s3.Push(9);
    EXPECT_FALSE(s1 == s3);

    Stack<int> s4(new MutableArraySequence<int>());
    s4.Push(1);
    s4.Push(2);
    EXPECT_FALSE(s1 == s4);

    Stack<int> empty1(new MutableArraySequence<int>());
    Stack<int> empty2(new MutableArraySequence<int>());
    EXPECT_TRUE(empty1 == empty2);
}