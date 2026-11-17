#ifndef QUEUE_TPP
#define QUEUE_TPP

template <typename T> void Queue<T>::enqueue(const T &value) {
  data_.pushBack(value); // O(1) благодаря tail_
}

template <typename T> T Queue<T>::dequeue() {
  if (isEmpty()) {
    throw std::out_of_range("Queue::dequeue: queue is empty");
  }
  T value = data_.at(0);
  data_.removeFront(); // O(1)
  return value;
}

template <typename T> T &Queue<T>::front() {
  if (isEmpty()) {
    throw std::out_of_range("Queue::front: queue is empty");
  }
  return data_.at(0);
}

template <typename T> const T &Queue<T>::front() const {
  if (isEmpty()) {
    throw std::out_of_range("Queue::front: queue is empty");
  }
  return data_.at(0);
}

template <typename T> std::size_t Queue<T>::getSize() const {
  return data_.getSize();
}

template <typename T> bool Queue<T>::isEmpty() const { return data_.isEmpty(); }

template <typename T> void Queue<T>::clear() { data_.clear(); }

#endif