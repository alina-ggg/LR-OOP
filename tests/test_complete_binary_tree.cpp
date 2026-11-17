#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "complete_binary_tree.h"

namespace {
// дерево из 7 узлов: индексы 0..6 = значения 1..7
CompleteBinaryTree<int> makeTree7() {
  CompleteBinaryTree<int> t;
  for (int v = 1; v <= 7; ++v)
    t.insert(v);
  return t;
}
} // namespace

TEST(CompleteBinaryTreeTest, DefaultConstructor) {
  CompleteBinaryTree<int> t;
  EXPECT_TRUE(t.isEmpty());
  EXPECT_EQ(t.getSize(), 0u);
  EXPECT_EQ(t.getHeight(), 0u);
  EXPECT_FALSE(t.contains(1));
  EXPECT_THROW(t.root(), std::out_of_range);
  EXPECT_THROW(t.removeLast(), std::out_of_range);
}

TEST(CompleteBinaryTreeTest, InsertAndRoot) {
  CompleteBinaryTree<int> t;
  t.insert(1);
  EXPECT_EQ(t.root(), 1);
  EXPECT_EQ(t.getHeight(), 1u);
  t.insert(2);
  t.insert(3);
  EXPECT_EQ(t.leftChildOf(0), 2);
  EXPECT_EQ(t.rightChildOf(0), 3);
  EXPECT_EQ(t.getSize(), 3u);
  EXPECT_EQ(t.getHeight(), 2u);
}

// индексная арифметика полного дерева: дети 2i+1/2i+2, родитель (i-1)/2
TEST(CompleteBinaryTreeTest, IndexRelations) {
  CompleteBinaryTree<int> t = makeTree7();
  EXPECT_FALSE(t.hasParent(0));
  EXPECT_TRUE(t.hasParent(3));
  EXPECT_EQ(t.parentOf(3), 2);
  EXPECT_EQ(t.parentOf(4), 2);
  EXPECT_EQ(t.parentOf(5), 3);
  EXPECT_EQ(t.parentOf(6), 3);
  EXPECT_EQ(t.leftChildOf(0), 2);
  EXPECT_EQ(t.rightChildOf(0), 3);
  EXPECT_EQ(t.leftChildOf(2), 6);
  EXPECT_EQ(t.rightChildOf(2), 7);
  EXPECT_TRUE(t.hasLeftChild(2));
  EXPECT_TRUE(t.hasRightChild(2));
  EXPECT_FALSE(t.hasLeftChild(3)); // лист
  EXPECT_FALSE(t.hasRightChild(3));
  EXPECT_FALSE(t.hasLeftChild(10)); // невалидный индекс
  EXPECT_FALSE(t.hasRightChild(10));
  EXPECT_FALSE(t.hasParent(10));
}

TEST(CompleteBinaryTreeTest, AccessorsThrow) {
  CompleteBinaryTree<int> t = makeTree7();
  EXPECT_THROW(t.parentOf(0), std::out_of_range);
  EXPECT_THROW(t.parentOf(10), std::out_of_range);
  EXPECT_THROW(t.leftChildOf(3), std::out_of_range);
  EXPECT_THROW(t.rightChildOf(3), std::out_of_range);
  EXPECT_THROW(t.at(7), std::out_of_range);
  const auto &cr = t;
  EXPECT_THROW(cr.parentOf(0), std::out_of_range);
  EXPECT_THROW(cr.leftChildOf(3), std::out_of_range);
  EXPECT_THROW(cr.rightChildOf(3), std::out_of_range);
  EXPECT_THROW(cr.at(7), std::out_of_range);
  const CompleteBinaryTree<int> empty;
  EXPECT_THROW(empty.root(), std::out_of_range);
}

TEST(CompleteBinaryTreeTest, ConstAccess) {
  CompleteBinaryTree<int> t = makeTree7();
  const auto &cr = t;
  EXPECT_EQ(cr.root(), 1);
  EXPECT_EQ(cr.at(4), 5);
  EXPECT_EQ(cr.parentOf(3), 2);
  EXPECT_EQ(cr.leftChildOf(0), 2);
  EXPECT_EQ(cr.rightChildOf(0), 3);
}

TEST(CompleteBinaryTreeTest, HeightProgression) {
  CompleteBinaryTree<int> t;
  EXPECT_EQ(t.getHeight(), 0u);
  t.insert(1);
  EXPECT_EQ(t.getHeight(), 1u);
  t.insert(2);
  EXPECT_EQ(t.getHeight(), 2u);
  t.insert(3);
  EXPECT_EQ(t.getHeight(), 2u);
  for (int v = 4; v <= 7; ++v)
    t.insert(v);
  EXPECT_EQ(t.getHeight(), 3u);
  t.insert(8);
  EXPECT_EQ(t.getHeight(), 4u);
}

TEST(CompleteBinaryTreeTest, RemoveLast) {
  CompleteBinaryTree<int> t = makeTree7();
  t.removeLast();
  EXPECT_EQ(t.getSize(), 6u);
  EXPECT_EQ(t.getHeight(), 3u);
  EXPECT_FALSE(t.hasRightChild(2));
  EXPECT_TRUE(t.hasLeftChild(2));
  while (!t.isEmpty())
    t.removeLast();
  EXPECT_EQ(t.getHeight(), 0u);
  EXPECT_THROW(t.removeLast(), std::out_of_range);
}

TEST(CompleteBinaryTreeTest, Contains) {
  CompleteBinaryTree<int> t = makeTree7();
  EXPECT_TRUE(t.contains(1));
  EXPECT_TRUE(t.contains(7));
  EXPECT_FALSE(t.contains(99));
}

TEST(CompleteBinaryTreeTest, AtModify) {
  CompleteBinaryTree<int> t = makeTree7();
  t.at(2) = 99;
  EXPECT_EQ(t.at(2), 99);
  EXPECT_EQ(t.root(), 1);
}

TEST(CompleteBinaryTreeTest, CopyConstructorDeepCopy) {
  CompleteBinaryTree<int> t = makeTree7();
  CompleteBinaryTree<int> copy(t);
  copy.at(0) = 100;
  EXPECT_EQ(t.root(), 1); // оригинал не изменился
  EXPECT_EQ(copy.root(), 100);
  EXPECT_EQ(copy.getSize(), 7u);
  CompleteBinaryTree<int> empty;
  CompleteBinaryTree<int> emptyCopy(empty);
  EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(CompleteBinaryTreeTest, CopyAssignment) {
  CompleteBinaryTree<int> t = makeTree7();
  CompleteBinaryTree<int> other;
  other.insert(9);
  other = t;
  EXPECT_EQ(other.getSize(), 7u);
  EXPECT_EQ(other.at(6), 7);
  other.at(0) = 55;
  EXPECT_EQ(t.root(), 1);
  auto *ptr = &t; // самоприсваивание
  t = *ptr;
  EXPECT_EQ(t.getSize(), 7u);
  CompleteBinaryTree<int> empty;
  t = empty;
  EXPECT_TRUE(t.isEmpty());
}

TEST(CompleteBinaryTreeTest, MoveConstructor) {
  CompleteBinaryTree<int> t = makeTree7();
  CompleteBinaryTree<int> moved(std::move(t));
  EXPECT_EQ(moved.getSize(), 7u);
  EXPECT_EQ(moved.root(), 1);
  EXPECT_TRUE(t.isEmpty());
  t.insert(5);
  EXPECT_EQ(t.root(), 5);
}

TEST(CompleteBinaryTreeTest, MoveAssignment) {
  CompleteBinaryTree<int> t = makeTree7();
  CompleteBinaryTree<int> other;
  other.insert(9);
  other = std::move(t);
  EXPECT_EQ(other.getSize(), 7u);
  EXPECT_TRUE(t.isEmpty());
  auto *ptr = &other; // самоперемещение
  other = std::move(*ptr);
  EXPECT_EQ(other.getSize(), 7u);
}

TEST(CompleteBinaryTreeTest, Clear) {
  CompleteBinaryTree<int> t;
  t.clear();
  EXPECT_TRUE(t.isEmpty());
  t = makeTree7();
  t.clear();
  EXPECT_TRUE(t.isEmpty());
  EXPECT_EQ(t.getHeight(), 0u);
  t.insert(1);
  EXPECT_EQ(t.root(), 1);
}

TEST(CompleteBinaryTreeTest, WorksWithStrings) {
  CompleteBinaryTree<std::string> t;
  t.insert("root");
  t.insert("left");
  t.insert("right");
  EXPECT_EQ(t.root(), "root");
  EXPECT_EQ(t.leftChildOf(0), "left");
  EXPECT_TRUE(t.contains("right"));
  EXPECT_FALSE(t.contains("none"));
}