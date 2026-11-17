#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility> // std::move

#include "doubly_linked_list.h"

// конструктор

TEST(DoublyLinkedListTest, DefaultConstructor) {
  DoublyLinkedList<int> list;
  EXPECT_TRUE(list.isEmpty());
  EXPECT_EQ(list.getSize(), 0u);
  EXPECT_FALSE(list.contains(1));
  EXPECT_THROW(list.at(0), std::out_of_range);
}

// вставка
TEST(DoublyLinkedListTest, PushFront) {
  DoublyLinkedList<int> list;
  list.pushFront(2); // в пустой: tail_ = node
  list.pushFront(1); // в непустой: head_->prev = node
  ASSERT_EQ(list.getSize(), 2u);
  EXPECT_EQ(list.at(0), 1);
  EXPECT_EQ(list.at(1), 2);
}

TEST(DoublyLinkedListTest, PushBack) {
  DoublyLinkedList<int> list;
  list.pushBack(1); // в пустой: head_ = node
  list.pushBack(2);
  list.pushBack(3);
  ASSERT_EQ(list.getSize(), 3u);
  EXPECT_EQ(list.at(0), 1);
  EXPECT_EQ(list.at(2), 3);
}

TEST(DoublyLinkedListTest, InsertAtAllPositions) {
  DoublyLinkedList<int> list;
  list.insertAt(0, 2); // index == 0 в пустой
  list.insertAt(1, 4); // index == size
  list.insertAt(0, 1); // в голову
  list.insertAt(2, 3); // в середину (обход от хвоста) -> 1 2 3 4
  list.insertAt(list.getSize(), 5); // в хвост
  ASSERT_EQ(list.getSize(), 5u);
  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(list.at(i), static_cast<int>(i + 1));
  }
}

TEST(DoublyLinkedListTest, InsertAtInvalidIndex) {
  DoublyLinkedList<int> list;
  EXPECT_THROW(list.insertAt(1, 9), std::out_of_range);
  list.pushBack(1);
  EXPECT_THROW(list.insertAt(5, 9), std::out_of_range);
}

// nodeAt: обход с обоих концов

TEST(DoublyLinkedListTest, NodeAtBothDirections) {
  DoublyLinkedList<int> list;
  for (int v = 0; v < 6; ++v)
    list.pushBack(v);       // 0..5, половина = 3
  EXPECT_EQ(list.at(0), 0); // голова
  EXPECT_EQ(list.at(1), 1); // 1 < 3  -> обход от головы
  EXPECT_EQ(list.at(4), 4); // 4 >= 3 -> обход от хвоста
  EXPECT_EQ(list.at(5), 5); // сам хвост (0 итераций цикла)
}

// удаление

TEST(DoublyLinkedListTest, RemoveFront) {
  DoublyLinkedList<int> list;
  EXPECT_THROW(list.removeFront(), std::out_of_range);

  list.pushBack(1);
  list.removeFront();
  EXPECT_TRUE(list.isEmpty());

  list.pushBack(1);
  list.pushBack(2);
  list.removeFront();
  EXPECT_EQ(list.getSize(), 1u);
  EXPECT_EQ(list.at(0), 2);

  list.pushFront(0);
  EXPECT_EQ(list.at(0), 0);
  EXPECT_EQ(list.at(1), 2);
}

TEST(DoublyLinkedListTest, RemoveBack) {
  DoublyLinkedList<int> list;
  EXPECT_THROW(list.removeBack(), std::out_of_range);

  list.pushBack(1);
  list.removeBack(); //  -> head_ = nullptr
  EXPECT_TRUE(list.isEmpty());

  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.removeBack(); // O(1): tail_ = tail_->prev
  ASSERT_EQ(list.getSize(), 2u);
  EXPECT_EQ(list.at(1), 2);

  list.pushBack(4); // tail_ корректен
  EXPECT_EQ(list.at(2), 4);
}

TEST(DoublyLinkedListTest, RemoveAtAllPositions) {
  DoublyLinkedList<int> list;
  for (int v = 1; v <= 6; ++v)
    list.pushBack(v); // 1..6

  list.removeAt(1); // середина, 1-я половина -> 1 3 4 5 6
  list.removeAt(3); // середина, 2-я половина -> 1 3 4 6
  list.removeAt(0); // голова -> 3 4 6
  list.removeAt(list.getSize() - 1); // хвост  -> 3 4
  ASSERT_EQ(list.getSize(), 2u);
  EXPECT_EQ(list.at(0), 3);
  EXPECT_EQ(list.at(1), 4);

  EXPECT_THROW(list.removeAt(2), std::out_of_range);
  EXPECT_THROW(list.removeAt(100), std::out_of_range);
}

// доступ / поиск

TEST(DoublyLinkedListTest, AtModify) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);
  list.at(1) = 20;
  EXPECT_EQ(list.at(1), 20);
  EXPECT_THROW(list.at(2), std::out_of_range);
}

TEST(DoublyLinkedListTest, ConstAt) {
  DoublyLinkedList<int> list;
  list.pushBack(7);
  list.pushBack(8);
  const auto &cref = list;
  EXPECT_EQ(cref.at(0), 7);
  EXPECT_EQ(cref.at(1), 8);
  EXPECT_THROW(cref.at(2), std::out_of_range);
}

TEST(DoublyLinkedListTest, Contains) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  EXPECT_TRUE(list.contains(1));
  EXPECT_TRUE(list.contains(3));
  EXPECT_FALSE(list.contains(99));
}

// копирование

TEST(DoublyLinkedListTest, CopyConstructorDeepCopy) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);

  DoublyLinkedList<int> copy(list);
  copy.at(0) = 99;
  EXPECT_EQ(list.at(0), 1); // оригинал не изменился
  EXPECT_EQ(copy.at(0), 99);
  copy.pushBack(3);
  copy.removeFront(); // prev-связи копии должны быть валидны
  EXPECT_EQ(copy.at(0), 2);
  EXPECT_EQ(copy.at(1), 3);

  DoublyLinkedList<int> empty;
  DoublyLinkedList<int> emptyCopy(empty);
  EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(DoublyLinkedListTest, CopyAssignment) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);

  DoublyLinkedList<int> other;
  other.pushBack(9);
  other = list; // copy-and-swap
  ASSERT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.at(1), 2);
  other.at(0) = 55;
  EXPECT_EQ(list.at(0), 1);

  auto *ptr = &list; // самоприсваивание
  list = *ptr;
  EXPECT_EQ(list.getSize(), 2u);

  DoublyLinkedList<int> empty;
  list = empty;
  EXPECT_TRUE(list.isEmpty());
}

// перемещение

TEST(DoublyLinkedListTest, MoveConstructor) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);

  DoublyLinkedList<int> moved(std::move(list));
  EXPECT_EQ(moved.getSize(), 2u);
  EXPECT_EQ(moved.at(1), 2);
  EXPECT_TRUE(list.isEmpty());

  list.pushBack(5);
  EXPECT_EQ(list.at(0), 5);
}

TEST(DoublyLinkedListTest, MoveAssignment) {
  DoublyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(2);

  DoublyLinkedList<int> other;
  other.pushBack(9);
  other = std::move(list);
  EXPECT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.at(0), 1);
  EXPECT_TRUE(list.isEmpty());

  auto *ptr = &other; // самоперемещение
  other = std::move(*ptr);
  EXPECT_EQ(other.getSize(), 2u);
}

// clear / целостность связей
TEST(DoublyLinkedListTest, Clear) {
  DoublyLinkedList<int> list;
  list.clear(); // пустой
  EXPECT_TRUE(list.isEmpty());

  list.pushBack(1);
  list.pushBack(2);
  list.clear();
  EXPECT_TRUE(list.isEmpty());

  list.pushBack(3);
  list.pushFront(2);
  EXPECT_EQ(list.at(0), 2);
  EXPECT_EQ(list.at(1), 3);
}

TEST(DoublyLinkedListTest, BidirectionalIntegrity) {
  DoublyLinkedList<int> list;
  for (int v = 0; v < 5; ++v)
    list.pushBack(v); // 0 1 2 3 4
  list.removeFront(); // 1 2 3 4
  list.removeBack();  // 1 2 3
  list.removeFront(); // 2 3
  list.removeBack();  // 2
  ASSERT_EQ(list.getSize(), 1u);
  EXPECT_EQ(list.at(0), 2);
  list.removeBack(); // пусто
  EXPECT_TRUE(list.isEmpty());

  list.pushFront(7);
  list.pushBack(8);
  EXPECT_EQ(list.at(0), 7);
  EXPECT_EQ(list.at(1), 8);
}

// шаблон с другим типом

TEST(DoublyLinkedListTest, WorksWithStrings) {
  DoublyLinkedList<std::string> list;
  list.pushBack("hello");
  list.pushFront("say");
  list.insertAt(2, "world");
  EXPECT_EQ(list.at(0), "say");
  EXPECT_EQ(list.at(2), "world");
  EXPECT_TRUE(list.contains("hello"));
  EXPECT_FALSE(list.contains("bye"));
}