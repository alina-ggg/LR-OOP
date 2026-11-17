#ifndef ARRAY_TPP
#define ARRAY_TPP

#include <algorithm>

// конструкторы / деструктор

template <typename T>
Array<T>::Array() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Array<T>::Array(std::size_t capacity)
    : data_(capacity ? new T[capacity] : nullptr), size_(0),
      capacity_(capacity) {}

template <typename T>
Array<T>::Array(const Array &other)
    : data_(other.capacity_ ? new T[other.capacity_] : nullptr),
      size_(other.size_), capacity_(other.capacity_) {
  for (std::size_t i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
}

template <typename T>
Array<T>::Array(Array &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T> Array<T>::~Array() { delete[] data_; }

// операторы присваивания

template <typename T> Array<T> &Array<T>::operator=(const Array &other) {
  if (this != &other) { // защита от самоприсваивания: a = a
    // сначала выделяем новый буфер: если new бросит исключение
    T *new_data = other.capacity_ ? new T[other.capacity_] : nullptr;
    for (std::size_t i = 0; i < other.size_; ++i) {
      new_data[i] = other.data_[i];
    }
    delete[] data_;
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
  }
  return *this;
}

template <typename T> Array<T> &Array<T>::operator=(Array &&other) noexcept {
  if (this != &other) {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

// перевыделение буфера

template <typename T> void Array<T>::resize(std::size_t new_capacity) {
  T *new_data = new_capacity ? new T[new_capacity] : nullptr;
  std::size_t n = std::min(size_, new_capacity);
  for (std::size_t i = 0; i < n; ++i) {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
  size_ = n;
}

// базовые операции

template <typename T> void Array<T>::pushBack(const T &value) {
  if (size_ == capacity_) {
    resize(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  data_[size_++] = value;
}

template <typename T>
void Array<T>::insertAt(std::size_t index, const T &value) {
  if (index > size_) { // вставка в конец
    throw std::out_of_range("Array::insertAt: index out of range");
  }
  if (size_ == capacity_) {
    resize(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  // сдвигаем элементы вправо, освобождая место
  for (std::size_t i = size_; i > index; --i) {
    data_[i] = data_[i - 1];
  }
  data_[index] = value;
  ++size_;
}

template <typename T> void Array<T>::removeAt(std::size_t index) {
  if (index >= size_) {
    throw std::out_of_range("Array::removeAt: index out of range");
  }
  // сдвигаем элементы влево, затирая удалённый
  for (std::size_t i = index; i + 1 < size_; ++i) {
    data_[i] = data_[i + 1];
  }
  --size_;
}

template <typename T> T Array<T>::popBack() {
  if (isEmpty()) {
    throw std::out_of_range("Array::popBack: array is empty");
  }
  return data_[--size_];
}

// доступ к элементам

template <typename T> T &Array<T>::at(std::size_t index) {
  if (index >= size_) {
    throw std::out_of_range("Array::at: index out of range");
  }
  return data_[index];
}

template <typename T> const T &Array<T>::at(std::size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("Array::at: index out of range");
  }
  return data_[index];
}

template <typename T> T &Array<T>::operator[](std::size_t index) {
  return at(index);
}

template <typename T> const T &Array<T>::operator[](std::size_t index) const {
  return at(index);
}

template <typename T> std::size_t Array<T>::getSize() const { return size_; }

template <typename T> std::size_t Array<T>::getCapacity() const {
  return capacity_;
}

template <typename T> bool Array<T>::isEmpty() const { return size_ == 0; }

template <typename T> void Array<T>::clear() { size_ = 0; }

#endif