#ifndef COMPLETE_BINARY_TREE_H
#define COMPLETE_BINARY_TREE_H

#include <cstddef>
#include <stdexcept>

#include "array.h"

// полное бинарное дерево
// узел i: дети 2i+1 и 2i+2, родитель (i-1)/2
template <typename T> class CompleteBinaryTree {
private:
  Array<T> nodes_; // обход по уровням слева направо

  static std::size_t parentIndex(std::size_t i);
  static std::size_t leftChildIndex(std::size_t i);
  static std::size_t rightChildIndex(std::size_t i);

public:
  CompleteBinaryTree() = default;
  CompleteBinaryTree(const CompleteBinaryTree &) = default;
  CompleteBinaryTree(CompleteBinaryTree &&) noexcept = default;
  CompleteBinaryTree &operator=(const CompleteBinaryTree &) = default;
  CompleteBinaryTree &operator=(CompleteBinaryTree &&) noexcept = default;
  ~CompleteBinaryTree() = default;

  void insert(const T &value); // следующий узел уровня, O(1)
  void removeLast();           // удалить последний узел
  T &root();
  const T &root() const;
  T &at(std::size_t index);
  const T &at(std::size_t index) const;

  bool hasParent(std::size_t index) const;
  bool hasLeftChild(std::size_t index) const;
  bool hasRightChild(std::size_t index) const;
  T &parentOf(std::size_t index);
  const T &parentOf(std::size_t index) const;
  T &leftChildOf(std::size_t index);
  const T &leftChildOf(std::size_t index) const;
  T &rightChildOf(std::size_t index);
  const T &rightChildOf(std::size_t index) const;

  bool contains(const T &value) const;
  std::size_t getSize() const;
  std::size_t getHeight() const; // число уровней
  bool isEmpty() const;
  void clear();
};

#include "../src/complete_binary_tree.tpp"

#endif