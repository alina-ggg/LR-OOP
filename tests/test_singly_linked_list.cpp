#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "singly_linked_list.h"

// конструктор
TEST(SinglyLinkedListTest, DefaultConstructor) {
    SinglyLinkedList<int> list;
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0u);
    EXPECT_FALSE(list.contains(1));  // поиск в пустом
    EXPECT_THROW(list.at(0), std::out_of_range);
}

// вставка
TEST(SinglyLinkedListTest, PushFront) {
    SinglyLinkedList<int> list;
    list.pushFront(2);  // в пустой (ветка tail_ == nullptr)
    list.pushFront(1);  // в непустой
    ASSERT_EQ(list.getSize(), 2u);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 2);
}

TEST(SinglyLinkedListTest, PushBack) {
    SinglyLinkedList<int> list;
    list.pushBack(1);  // в пустой (ветка head_ = node)
    list.pushBack(2);  // через tail_->next
    list.pushBack(3);
    ASSERT_EQ(list.getSize(), 3u);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(2), 3);
}

TEST(SinglyLinkedListTest, InsertAtAllPositions) {
    SinglyLinkedList<int> list;
    list.insertAt(0, 2);               // index == 0 в пустой
    list.insertAt(1, 4);               // index == size (в конец)
    list.insertAt(0, 1);               // в голову
    list.insertAt(2, 3);               // в середину -> 1 2 3 4
    list.insertAt(list.getSize(), 5);  // в хвост
    ASSERT_EQ(list.getSize(), 5u);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(list.at(i), static_cast<int>(i + 1));
    }
}

TEST(SinglyLinkedListTest, InsertAtInvalidIndex) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.insertAt(1, 9), std::out_of_range);
    list.pushBack(1);
    EXPECT_THROW(list.insertAt(5, 9), std::out_of_range);
}

// удаление
TEST(SinglyLinkedListTest, RemoveFront) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.removeFront(), std::out_of_range);

    list.pushBack(1);
    list.removeFront();  // единственный элемент -> tail_ обнуляется
    EXPECT_TRUE(list.isEmpty());

    list.pushBack(1);
    list.pushBack(2);
    list.removeFront();
    EXPECT_EQ(list.getSize(), 1u);
    EXPECT_EQ(list.at(0), 2);

    list.pushBack(3);  // tail_ должен быть корректен после удалений
    EXPECT_EQ(list.at(1), 3);
}

TEST(SinglyLinkedListTest, RemoveBack) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.removeBack(), std::out_of_range);

    list.pushBack(1);
    list.removeBack();  // ветка size_ == 1
    EXPECT_TRUE(list.isEmpty());

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.removeBack();  // ветка с поиском предпоследнего (while)
    ASSERT_EQ(list.getSize(), 2u);
    EXPECT_EQ(list.at(1), 2);

    list.pushBack(4);  // tail_ корректен после removeBack
    EXPECT_EQ(list.at(2), 4);
}

TEST(SinglyLinkedListTest, RemoveAtAllPositions) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(5);

    list.removeAt(2);                   // середина -> 1 2 4 5
    list.removeAt(0);                   // голова   -> 2 4 5
    list.removeAt(list.getSize() - 1);  // хвост    -> 2 4
    ASSERT_EQ(list.getSize(), 2u);
    EXPECT_EQ(list.at(0), 2);
    EXPECT_EQ(list.at(1), 4);

    EXPECT_THROW(list.removeAt(2), std::out_of_range);
    EXPECT_THROW(list.removeAt(100), std::out_of_range);
}

// доступ / поиск
TEST(SinglyLinkedListTest, AtModify) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.at(1) = 20;  // non-const ссылка позволяет менять
    EXPECT_EQ(list.at(1), 20);
    EXPECT_THROW(list.at(2), std::out_of_range);
}

TEST(SinglyLinkedListTest, ConstAt) {
    SinglyLinkedList<int> list;
    list.pushBack(7);
    const auto& cref = list;  // вызываются const-версии
    EXPECT_EQ(cref.at(0), 7);
    EXPECT_THROW(cref.at(1), std::out_of_range);
}

TEST(SinglyLinkedListTest, Contains) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    EXPECT_TRUE(list.contains(1));    // голова
    EXPECT_TRUE(list.contains(3));    // хвост
    EXPECT_FALSE(list.contains(99));  // полный обход -> false
}

// копирование
TEST(SinglyLinkedListTest, CopyConstructorDeepCopy) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    SinglyLinkedList<int> copy(list);
    copy.at(0) = 99;
    EXPECT_EQ(list.at(0), 1);  // оригинал не изменился -> глубокая копия
    EXPECT_EQ(copy.at(0), 99);
    EXPECT_EQ(copy.getSize(), 2u);
    copy.pushBack(3);  // tail_ копии работает
    EXPECT_EQ(copy.at(2), 3);

    SinglyLinkedList<int> empty;
    SinglyLinkedList<int> emptyCopy(empty);
    EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(SinglyLinkedListTest, CopyAssignment) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    SinglyLinkedList<int> other;
    other.pushBack(9);
    other = list;  // copy-and-swap: старые узлы other освободятся
    ASSERT_EQ(other.getSize(), 2u);
    EXPECT_EQ(other.at(1), 2);
    other.at(0) = 55;
    EXPECT_EQ(list.at(0), 1);

    auto* ptr = &list;  // самоприсваивание
    list = *ptr;
    EXPECT_EQ(list.getSize(), 2u);

    SinglyLinkedList<int> empty;
    list = empty;  // присваивание пустого
    EXPECT_TRUE(list.isEmpty());
}

// перемещение
TEST(SinglyLinkedListTest, MoveConstructor) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    SinglyLinkedList<int> moved(std::move(list));
    EXPECT_EQ(moved.getSize(), 2u);
    EXPECT_EQ(moved.at(1), 2);
    EXPECT_TRUE(list.isEmpty());

    list.pushBack(5);
    EXPECT_EQ(list.at(0), 5);
}

TEST(SinglyLinkedListTest, MoveAssignment) {
    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    SinglyLinkedList<int> other;
    other.pushBack(9);
    other = std::move(list);  // clear() старых узлов
    EXPECT_EQ(other.getSize(), 2u);
    EXPECT_EQ(other.at(0), 1);
    EXPECT_TRUE(list.isEmpty());

    auto* ptr = &other;  // самоперемещение
    other = std::move(*ptr);
    EXPECT_EQ(other.getSize(), 2u);
}

// clear
TEST(SinglyLinkedListTest, Clear) {
    SinglyLinkedList<int> list;
    list.clear();  // очистка пустого (цикл не выполняется)
    EXPECT_TRUE(list.isEmpty());

    list.pushBack(1);
    list.pushBack(2);
    list.clear();
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0u);

    list.pushBack(3);  // список пригоден после clear
    EXPECT_EQ(list.at(0), 3);
}

// шаблон с другим типом
TEST(SinglyLinkedListTest, WorksWithStrings) {
    SinglyLinkedList<std::string> list;
    list.pushBack("hello");
    list.pushFront("say");
    list.insertAt(2, "world");
    EXPECT_EQ(list.at(0), "say");
    EXPECT_EQ(list.at(2), "world");
    EXPECT_TRUE(list.contains("hello"));
    EXPECT_FALSE(list.contains("bye"));
}