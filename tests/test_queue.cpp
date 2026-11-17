#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "queue.h"

TEST(QueueTest, DefaultConstructor) {
  Queue<int> q;
  EXPECT_TRUE(q.isEmpty());
  EXPECT_EQ(q.getSize(), 0u);
  EXPECT_THROW(q.dequeue(), std::out_of_range);
  EXPECT_THROW(q.front(), std::out_of_range);
}

TEST(QueueTest, EnqueueDequeueFIFO) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  q.enqueue(3);
  EXPECT_EQ(q.getSize(), 3u);
  EXPECT_EQ(q.dequeue(), 1); // FIFO: первый пришёл — первый ушёл
  EXPECT_EQ(q.dequeue(), 2);
  EXPECT_EQ(q.dequeue(), 3);
  EXPECT_TRUE(q.isEmpty());
  EXPECT_THROW(q.dequeue(), std::out_of_range);
}

TEST(QueueTest, FrontTracksHead) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  EXPECT_EQ(q.front(), 1);
  q.dequeue();
  EXPECT_EQ(q.front(), 2);
  q.enqueue(3);
  EXPECT_EQ(q.front(), 2); // голова не меняется от enqueue
}

TEST(QueueTest, FrontModify) {
  Queue<int> q;
  q.enqueue(5);
  q.front() = 99;
  EXPECT_EQ(q.dequeue(), 99);
}

TEST(QueueTest, ConstFront) {
  Queue<int> q;
  q.enqueue(7);
  const auto &cref = q;
  EXPECT_EQ(cref.front(), 7);
  const Queue<int> empty;
  EXPECT_THROW(empty.front(), std::out_of_range);
}

TEST(QueueTest, CopyConstructorDeepCopy) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  Queue<int> copy(q);
  EXPECT_EQ(copy.dequeue(), 1);
  EXPECT_EQ(q.front(), 1); // оригинал не изменился
  EXPECT_EQ(q.getSize(), 2u);
  Queue<int> empty;
  Queue<int> emptyCopy(empty);
  EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(QueueTest, CopyAssignment) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  Queue<int> other;
  other.enqueue(9);
  other = q;
  EXPECT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.dequeue(), 1);
  EXPECT_EQ(q.getSize(), 2u);
  auto *ptr = &q;
  q = *ptr;
  EXPECT_EQ(q.getSize(), 2u);
  Queue<int> empty;
  q = empty;
  EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, MoveConstructor) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  Queue<int> moved(std::move(q));
  EXPECT_EQ(moved.getSize(), 2u);
  EXPECT_EQ(moved.dequeue(), 1);
  EXPECT_TRUE(q.isEmpty());
  q.enqueue(5);
  EXPECT_EQ(q.front(), 5);
}

TEST(QueueTest, MoveAssignment) {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  Queue<int> other;
  other.enqueue(9);
  other = std::move(q);
  EXPECT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.front(), 1);
  EXPECT_TRUE(q.isEmpty());
  auto *ptr = &other;
  other = std::move(*ptr);
  EXPECT_EQ(other.getSize(), 2u);
}

TEST(QueueTest, Clear) {
  Queue<int> q;
  q.clear();
  EXPECT_TRUE(q.isEmpty());
  q.enqueue(1);
  q.enqueue(2);
  q.clear();
  EXPECT_TRUE(q.isEmpty());
  q.enqueue(3);
  EXPECT_EQ(q.front(), 3);
}

// многократное заполнение/опустошение: head_ и tail_ должны корректно
// сбрасываться
TEST(QueueTest, FillDrainIntegrity) {
  Queue<int> q;
  for (int round = 0; round < 50; ++round) {
    for (int i = 0; i < 10; ++i)
      q.enqueue(i);
    for (int i = 0; i < 10; ++i)
      EXPECT_EQ(q.dequeue(), i);
    EXPECT_TRUE(q.isEmpty());
  }
}

TEST(QueueTest, WorksWithStrings) {
  Queue<std::string> q;
  q.enqueue("a");
  q.enqueue("b");
  EXPECT_EQ(q.dequeue(), "a");
  EXPECT_EQ(q.front(), "b");
}