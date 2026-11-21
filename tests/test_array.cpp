#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "array.h"

// конструкторы
TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.getSize(), 0u);
    EXPECT_EQ(arr.getCapacity(), 0u);
    EXPECT_TRUE(arr.isEmpty());
}

TEST(ArrayTest, CapacityConstructor) {
    Array<int> arr(10);
    EXPECT_EQ(arr.getSize(), 0u);
    EXPECT_EQ(arr.getCapacity(), 10u);
    EXPECT_TRUE(arr.isEmpty());

    Array<int> zero(0);
    EXPECT_EQ(zero.getCapacity(), 0u);
}

// pushBack и рост буфера
TEST(ArrayTest, PushBackAndGrowth) {
    Array<int> arr;
    arr.pushBack(1);  // capacity 0 -> 1
    EXPECT_EQ(arr.getCapacity(), 1u);
    arr.pushBack(2);  // 1 -> 2
    EXPECT_EQ(arr.getCapacity(), 2u);
    arr.pushBack(3);  // 2 -> 4
    EXPECT_EQ(arr.getCapacity(), 4u);

    EXPECT_EQ(arr.getSize(), 3u);
    EXPECT_FALSE(arr.isEmpty());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

// insertAt

TEST(ArrayTest, InsertAt) {
    Array<int> arr;
    arr.pushBack(10);
    arr.pushBack(30);
    arr.insertAt(1, 20);              // в середину
    arr.insertAt(0, 5);               // в начало
    arr.insertAt(arr.getSize(), 40);  // в конец

    ASSERT_EQ(arr.getSize(), 5u);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 20);
    EXPECT_EQ(arr[3], 30);
    EXPECT_EQ(arr[4], 40);
}

TEST(ArrayTest, InsertIntoEmpty) {
    Array<int> arr;
    arr.insertAt(0, 7);  // рост 0 -> 1 внутри insertAt
    EXPECT_EQ(arr[0], 7);
    EXPECT_EQ(arr.getSize(), 1u);
}

TEST(ArrayTest, InsertAtInvalidIndex) {
    Array<int> arr;
    arr.pushBack(1);
    EXPECT_THROW(arr.insertAt(2, 99), std::out_of_range);
    EXPECT_THROW(arr.insertAt(100, 99), std::out_of_range);
}

// removeAt / popBack
TEST(ArrayTest, RemoveAt) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);
    arr.pushBack(3);

    arr.removeAt(1);  // [1, 3]
    ASSERT_EQ(arr.getSize(), 2u);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);

    arr.removeAt(0);
    arr.removeAt(0);
    EXPECT_TRUE(arr.isEmpty());
    EXPECT_THROW(arr.removeAt(0), std::out_of_range);
}

TEST(ArrayTest, PopBack) {
    Array<int> arr;
    EXPECT_THROW(arr.popBack(), std::out_of_range);
    arr.pushBack(42);
    EXPECT_EQ(arr.popBack(), 42);
    EXPECT_TRUE(arr.isEmpty());
}

// доступ: at / operator[]

TEST(ArrayTest, AccessAndModify) {
    Array<int> arr;
    arr.pushBack(1);
    arr[0] = 100;     // non-const operator[]
    arr.at(0) = 200;  // non-const at
    EXPECT_EQ(arr[0], 200);
    EXPECT_THROW(arr[1], std::out_of_range);
    EXPECT_THROW(arr.at(5), std::out_of_range);
}

TEST(ArrayTest, ConstAccess) {
    Array<int> arr;
    arr.pushBack(7);
    const Array<int>& cref = arr;  // const-ссылка -> вызываются const-версии
    EXPECT_EQ(cref[0], 7);
    EXPECT_EQ(cref.at(0), 7);
    EXPECT_THROW(cref[1], std::out_of_range);
    EXPECT_THROW(cref.at(1), std::out_of_range);
}

// копирование

TEST(ArrayTest, CopyConstructorDeepCopy) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);

    Array<int> copy(arr);
    copy[0] = 99;
    EXPECT_EQ(arr[0], 1);  // оригинал не изменился -> копия глубокая
    EXPECT_EQ(copy[0], 99);
    EXPECT_EQ(copy.getSize(), 2u);

    Array<int> empty;
    Array<int> emptyCopy(empty);  // копия пустого (ветка capacity_ == 0)
    EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(ArrayTest, CopyAssignment) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);

    Array<int> other;
    other.pushBack(9);
    other = arr;
    ASSERT_EQ(other.getSize(), 2u);
    EXPECT_EQ(other[1], 2);
    other[0] = 55;
    EXPECT_EQ(arr[0], 1);  // проверяем глубокую копию

    // самоприсваивание (через указатель)
    Array<int>* ptr = &arr;
    arr = *ptr;
    EXPECT_EQ(arr.getSize(), 2u);
    EXPECT_EQ(arr[0], 1);

    // присваивание пустого массива
    Array<int> empty;
    arr = empty;
    EXPECT_TRUE(arr.isEmpty());
}

// перемещение

TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);

    Array<int> moved(std::move(arr));
    EXPECT_EQ(moved.getSize(), 2u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_TRUE(arr.isEmpty());  // источник обнулён
    EXPECT_EQ(arr.getCapacity(), 0u);
}

TEST(ArrayTest, MoveAssignment) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);

    Array<int> other;
    other.pushBack(9);
    other = std::move(arr);
    EXPECT_EQ(other.getSize(), 2u);
    EXPECT_EQ(other[1], 2);
    EXPECT_TRUE(arr.isEmpty());

    // самоперемещение
    Array<int>* ptr = &other;
    other = std::move(*ptr);
    EXPECT_EQ(other.getSize(), 2u);
}

// clear

TEST(ArrayTest, Clear) {
    Array<int> arr;
    arr.pushBack(1);
    arr.pushBack(2);
    std::size_t cap = arr.getCapacity();
    arr.clear();
    EXPECT_TRUE(arr.isEmpty());
    EXPECT_EQ(arr.getSize(), 0u);
    EXPECT_EQ(arr.getCapacity(), cap);  // вместимость сохраняется
}

// шаблон с другим типом

TEST(ArrayTest, WorksWithStrings) {
    Array<std::string> arr;
    arr.pushBack("hello");
    arr.pushBack("world");
    arr.insertAt(1, "beautiful");
    EXPECT_EQ(arr[1], "beautiful");
    EXPECT_EQ(arr.getSize(), 3u);
}
