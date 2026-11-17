#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <stdexcept>

#include "array.h" // переиспользуем готовый массив

template <typename T> class Stack {
private:
  Array<T> data_; // пользователь не может нарушить LIFO

public:
  Stack() = default;
  Stack(const Stack &) = default;
  Stack(Stack &&) noexcept = default;
  Stack &operator=(const Stack &) = default;
  Stack &operator=(Stack &&) noexcept = default;
  ~Stack() = default;

  void push(const T &value);
  T pop();
  T &top();
  const T &top() const;

  std::size_t getSize() const;
  bool isEmpty() const;
  void clear();
};

#include "../src/stack.tpp"

#endif