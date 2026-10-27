#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <complex>

#include "Stack.h"
#include "lab2/MutableArraySequence.h"
#include "lab2/MutableListSequence.h"
#include "lab2/ImmutableArraySequence.h"
#include "lab2/ImmutableListSequence.h"


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

// pop и peek на пустом стеке, и после снятия последнего эл стек снова пустой
TEST(StackTest, PopAndPeekOnEmptyStack) {
    Stack<int> stack(new MutableArraySequence<int>());
    EXPECT_THROW(stack.Pop(), std::out_of_range); 
    EXPECT_THROW(stack.Peek(), std::out_of_range); 
    stack.Push(7);
    EXPECT_EQ(stack.Pop(), 7);
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_THROW(stack.Pop(), std::out_of_range);
}

// проверка на nullptr в конструкторе
TEST(StackTest, ConstructorNullptrThrows) {
    EXPECT_THROW(Stack<int>(nullptr), std::invalid_argument);
}

// get считает от вершины
TEST(StackTest, GetFromTop) {
    Stack<int> stack(new MutableArraySequence<int>());
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);
    EXPECT_EQ(stack.Peek(), 30);
    EXPECT_EQ(stack.Get(0), 30);
    EXPECT_EQ(stack.Get(1), 20);
    EXPECT_EQ(stack.Get(2), 10);
    EXPECT_THROW(stack.Get(-1), std::out_of_range);
    EXPECT_THROW(stack.Get(3), std::out_of_range);
    stack.Pop();
    EXPECT_EQ(stack.Get(0), 20);
    EXPECT_EQ(stack.GetCount(), 2);
}

// одно и то же на всех четырёх хранилищах: внутри push и pop работают по-разному
TEST(StackTest, WorksOnAllStorages) {
    int arr[] = {1 ,2, 3};
    Stack<int> stack1(new MutableArraySequence<int>(arr, 3));
    Stack<int> stack2(new ImmutableArraySequence<int>(arr, 3));
    Stack<int> stack3(new MutableListSequence<int>(arr, 3));
    Stack<int> stack4(new ImmutableListSequence<int>(arr, 3));

    EXPECT_EQ(stack1.Peek(), 3);
    EXPECT_EQ(stack1.Get(2), 1);
    EXPECT_EQ(stack1.Pop(), 3);
    EXPECT_EQ(stack1.GetCount(), 2);
    EXPECT_EQ(stack1.Peek(), 2);
    
    EXPECT_EQ(stack2.Peek(), 3);
    EXPECT_EQ(stack2.Get(2), 1);
    EXPECT_EQ(stack2.Pop(), 3);
    EXPECT_EQ(stack2.GetCount(), 2);
    EXPECT_EQ(stack2.Peek(), 2);

    EXPECT_EQ(stack3.Peek(), 3);
    EXPECT_EQ(stack3.Get(2), 1);
    EXPECT_EQ(stack3.Pop(), 3);
    EXPECT_EQ(stack3.GetCount(), 2);
    EXPECT_EQ(stack3.Peek(), 2);
    
    EXPECT_EQ(stack4.Peek(), 3);
    EXPECT_EQ(stack4.Get(2), 1);
    EXPECT_EQ(stack4.Pop(), 3);
    EXPECT_EQ(stack4.GetCount(), 2);
    EXPECT_EQ(stack4.Peek(), 2);

    EXPECT_TRUE(stack1 == stack4);
}

// копия через указатель на ICollection, не зная что там стек
TEST(StackTest, CopyThroughICollection) {
    Stack<int> stack(new MutableListSequence<int>());
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    ICollection<int>* col = &stack;
    ICollection<int>* copy = col->Copy();
    EXPECT_EQ(copy->GetCount(), 3);
    EXPECT_EQ(copy->Get(0), 3);

    stack.Push(99);
    EXPECT_EQ(copy->GetCount(), 3);
    EXPECT_EQ(copy->Get(0), 3);
    EXPECT_EQ(col->GetCount(), 4);
    EXPECT_EQ(col->Get(0), 99);

    delete copy;
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

// concat не портит исходные стеки: a на мутабл (без копии concat дописал бы прямо в a), b на иммутабл
TEST(StackTest, ConcatKeepsOriginals) {
    Stack<int> a(new MutableArraySequence<int>());
    a.Push(1);
    a.Push(2);
    Stack<int> b(new ImmutableListSequence<int>());
    b.Push(3);
    b.Push(4);

    Stack<int>* res = a.Concat(&b);
    EXPECT_EQ(res->GetCount(), 4);
    EXPECT_EQ(res->Get(0), 4);
    EXPECT_EQ(res->Get(1), 3);
    EXPECT_EQ(res->Get(2), 2);
    EXPECT_EQ(res->Get(3), 1);
    EXPECT_EQ(a.GetCount(), 2);
    EXPECT_EQ(a.Peek(), 2);
    EXPECT_EQ(b.GetCount(), 2);
    EXPECT_EQ(b.Peek(), 4);

    Stack<int>* res2 = b.Concat(&a);
    EXPECT_EQ(res2->Get(0), 2);
    EXPECT_EQ(res2->Get(1), 1);
    EXPECT_EQ(res2->Get(2), 4);
    EXPECT_EQ(res2->Get(3), 3);
    EXPECT_EQ(b.GetCount(), 2);

    Stack<int> sum = a + b;
    EXPECT_TRUE(sum == *res);

    Stack<int> chain = a + b + a;
    EXPECT_EQ(chain.GetCount(), 6);
    EXPECT_EQ(chain.Peek(), 2);

    EXPECT_THROW(a.Concat(nullptr), std::invalid_argument);

    delete res;
    delete res2;
}

// кусок из GetSubSequence находится FindSubsequence там же, откуда вырезан: оба считают от вершины
TEST(StackTest, SubSequenceAndFindAgree) {
    Stack<int> stack(new MutableListSequence<int>());
    for (int i = 1; i <= 5; ++i) {
        stack.Push(i);
    }

    Stack<int>* sub = stack.GetSubSequence(1, 3);
    EXPECT_EQ(sub->GetCount(), 3);
    EXPECT_EQ(sub->Get(0), 4);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->Get(2), 2);
    EXPECT_EQ(stack.FindSubsequence(*sub), 1);
    EXPECT_EQ(stack.GetCount(), 5);

    Stack<int>* full = stack.GetSubSequence(0, 4);
    EXPECT_TRUE(*full == stack);

    // от вершины в стеке идёт 4, потом 3, а в образце наоборот
    Stack<int> pattern(new MutableArraySequence<int>());
    pattern.Push(4);
    pattern.Push(3);
    EXPECT_EQ(stack.FindSubsequence(pattern), -1);

    EXPECT_THROW(stack.GetSubSequence(3, 1), std::out_of_range);
    EXPECT_THROW(stack.GetSubSequence(0, 5), std::out_of_range);
    EXPECT_THROW(stack.GetSubSequence(-1, 0), std::out_of_range);

    delete sub;
    delete full;
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

// --- другие типы элементов ---

static double halfOf(const double& x) { return x / 2; }
static double sumDouble(const double& acc, const double& x) { return acc + x; }
static bool isPositive(const double& x) { return x > 0; }

// вещественные и комплексные числа
TEST(StackTest, WorksWithDoubleAndComplex) {
    Stack<double> s(new MutableArraySequence<double>());
    s.Push(1.5);
    s.Push(-2.0);
    s.Push(4.0);
    EXPECT_DOUBLE_EQ(s.Peek(), 4.0);
    EXPECT_DOUBLE_EQ(s.Reduce<double>(sumDouble, 0.0), 3.5);

    Stack<double>* halves = s.Map(halfOf);
    EXPECT_DOUBLE_EQ(halves->Peek(), 2.0);
    Stack<double>* positive = s.Where(isPositive);
    EXPECT_EQ(positive->GetCount(), 2);
    delete halves;
    delete positive;

    Stack<std::complex<double>> c(new MutableListSequence<std::complex<double>>());
    c.Push(std::complex<double>(1, 2));
    c.Push(std::complex<double>(3, -1));
    EXPECT_EQ(c.Pop(), std::complex<double>(3, -1));
    EXPECT_EQ(c.Peek(), std::complex<double>(1, 2));
}

// строки: поиск образца, копия и сравнение
TEST(StackTest, WorksWithStrings) {
    Stack<std::string> s(new MutableArraySequence<std::string>());
    s.Push("a");
    s.Push("b");
    s.Push("c");
    EXPECT_EQ(s.Get(0), "c");

    Stack<std::string> pattern(new MutableArraySequence<std::string>());
    pattern.Push("b");
    pattern.Push("c");
    EXPECT_EQ(s.FindSubsequence(pattern), 0);

    Stack<std::string> copy(s);
    EXPECT_TRUE(copy == s);
    EXPECT_EQ(s.Pop(), "c");
    EXPECT_FALSE(copy == s);
}

// студенту нужен конструктор по умолч (из-за DynamicArray) и operator== (для поиска и ==)
struct Student {
    int id;
    std::string name;
    bool operator==(const Student& other) const { return id == other.id && name == other.name; }
};

static int studentId(const Student& s) { return s.id; }

// структура студента на иммутабл и на списке
TEST(StackTest, WorksWithStudents) {
    Stack<Student> s(new ImmutableArraySequence<Student>());
    s.Push({1, "Иванов"});
    s.Push({2, "Петров"});
    EXPECT_EQ(s.Peek().name, "Петров");

    Stack<int>* ids = s.Map(studentId);
    EXPECT_EQ(ids->Peek(), 2);
    EXPECT_EQ(ids->Get(1), 1);
    delete ids;

    Stack<Student> other(new MutableListSequence<Student>());
    other.Push({1, "Иванов"});
    other.Push({2, "Петров"});
    EXPECT_TRUE(s == other);

    EXPECT_EQ(s.Pop().id, 2);
    EXPECT_EQ(s.GetCount(), 1);
}
