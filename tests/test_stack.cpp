#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "stack.h"

TEST(StackTest, DefaultConstructor) {
  Stack<int> st;
  EXPECT_TRUE(st.isEmpty());
  EXPECT_EQ(st.getSize(), 0u);
  EXPECT_THROW(st.pop(), std::out_of_range);
  EXPECT_THROW(st.top(), std::out_of_range);
}

TEST(StackTest, PushPopLIFO) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  st.push(3);
  EXPECT_EQ(st.getSize(), 3u);
  EXPECT_EQ(st.pop(), 3); // LIFO: последний пришёл — первый ушёл
  EXPECT_EQ(st.pop(), 2);
  EXPECT_EQ(st.pop(), 1);
  EXPECT_TRUE(st.isEmpty());
  EXPECT_THROW(st.pop(), std::out_of_range);
}

TEST(StackTest, TopReflectsLast) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  EXPECT_EQ(st.top(), 2);
  st.pop();
  EXPECT_EQ(st.top(), 1);
}

TEST(StackTest, TopModify) {
  Stack<int> st;
  st.push(5);
  st.top() = 99; // non-const ссылка
  EXPECT_EQ(st.pop(), 99);
}

TEST(StackTest, ConstTop) {
  Stack<int> st;
  st.push(7);
  const auto &cref = st;
  EXPECT_EQ(cref.top(), 7);
  const Stack<int> empty;
  EXPECT_THROW(empty.top(), std::out_of_range);
}

TEST(StackTest, CopyConstructorDeepCopy) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  Stack<int> copy(st); // = default, но копия глубокая (через Array)
  EXPECT_EQ(copy.pop(), 2);
  EXPECT_EQ(st.top(), 2); // оригинал не изменился
  EXPECT_EQ(st.getSize(), 2u);
  Stack<int> empty;
  Stack<int> emptyCopy(empty);
  EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(StackTest, CopyAssignment) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  Stack<int> other;
  other.push(9);
  other = st;
  EXPECT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.pop(), 2);
  EXPECT_EQ(st.getSize(), 2u);
  auto *ptr = &st; // самоприсваивание
  st = *ptr;
  EXPECT_EQ(st.getSize(), 2u);
  Stack<int> empty;
  st = empty;
  EXPECT_TRUE(st.isEmpty());
}

TEST(StackTest, MoveConstructor) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  Stack<int> moved(std::move(st));
  EXPECT_EQ(moved.getSize(), 2u);
  EXPECT_EQ(moved.pop(), 2);
  EXPECT_TRUE(st.isEmpty());
  st.push(5); // moved-from пригоден
  EXPECT_EQ(st.top(), 5);
}

TEST(StackTest, MoveAssignment) {
  Stack<int> st;
  st.push(1);
  st.push(2);
  Stack<int> other;
  other.push(9);
  other = std::move(st);
  EXPECT_EQ(other.getSize(), 2u);
  EXPECT_EQ(other.top(), 2);
  EXPECT_TRUE(st.isEmpty());
  auto *ptr = &other; // самоперемещение
  other = std::move(*ptr);
  EXPECT_EQ(other.getSize(), 2u);
}

TEST(StackTest, Clear) {
  Stack<int> st;
  st.clear();
  EXPECT_TRUE(st.isEmpty());
  st.push(1);
  st.push(2);
  st.clear();
  EXPECT_TRUE(st.isEmpty());
  st.push(3);
  EXPECT_EQ(st.top(), 3);
}

TEST(StackTest, WorksWithStrings) {
  Stack<std::string> st;
  st.push("a");
  st.push("b");
  EXPECT_EQ(st.pop(), "b");
  EXPECT_EQ(st.top(), "a");
}