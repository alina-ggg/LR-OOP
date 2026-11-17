#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <stdexcept>

#include "singly_linked_list.h" // голова + хвост дают O(1) с обоих концов

template <typename T> class Queue {
private:
  SinglyLinkedList<T> data_;

public:
  Queue() = default;
  Queue(const Queue &) = default;
  Queue(Queue &&) noexcept = default;
  Queue &operator=(const Queue &) = default;
  Queue &operator=(Queue &&) noexcept = default;
  ~Queue() = default;

  void enqueue(const T &value);
  T dequeue();
  T &front();
  const T &front() const;

  std::size_t getSize() const;
  bool isEmpty() const;
  void clear();
};

#include "../src/queue.tpp"

#endif