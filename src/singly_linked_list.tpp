#ifndef SINGLY_LINKED_LIST_TPP
#define SINGLY_LINKED_LIST_TPP

#include <utility>

// конструкторы / деструктор
template <typename T>
SinglyLinkedList<T>::SinglyLinkedList()
    : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList &other)
    : head_(nullptr), tail_(nullptr), size_(0) {
  for (const Node *cur = other.head_; cur != nullptr; cur = cur->next) {
    pushBack(cur->value);
  }
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList &&other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
  other.head_ = nullptr;
  other.tail_ = nullptr;
  other.size_ = 0;
}

template <typename T> SinglyLinkedList<T>::~SinglyLinkedList() { clear(); }

// присваивание
template <typename T>
void SinglyLinkedList<T>::swap(SinglyLinkedList &other) noexcept {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template <typename T>
SinglyLinkedList<T> &
SinglyLinkedList<T>::operator=(const SinglyLinkedList &other) {
  if (this != &other) {
    // copy-and-swap: сначала строим копию, потом обмениваемся.
    // Если new бросит исключение — наш объект не пострадал.
    SinglyLinkedList tmp(other);
    swap(tmp);
  } // деструктор tmp освободит наши старые узлы
  return *this;
}

template <typename T>
SinglyLinkedList<T> &
SinglyLinkedList<T>::operator=(SinglyLinkedList &&other) noexcept {
  if (this != &other) {
    clear();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

// вставка
template <typename T> void SinglyLinkedList<T>::pushFront(const T &value) {
  head_ = new Node(value, head_);
  if (tail_ == nullptr)
    tail_ = head_; // список был пуст
  ++size_;
}

template <typename T> void SinglyLinkedList<T>::pushBack(const T &value) {
  Node *node = new Node(value);
  if (tail_ != nullptr) {
    tail_->next = node;
  } else {
    head_ = node;
  }
  tail_ = node;
  ++size_;
}

template <typename T>
void SinglyLinkedList<T>::insertAt(std::size_t index, const T &value) {
  if (index > size_) {
    throw std::out_of_range("SinglyLinkedList::insertAt: index out of range");
  }
  if (index == 0) {
    pushFront(value);
    return;
  }
  if (index == size_) {
    pushBack(value);
    return;
  }

  Node *prev = head_;
  for (std::size_t i = 0; i + 1 < index; ++i)
    prev = prev->next;
  prev->next = new Node(value, prev->next);
  ++size_;
}

// удаление

template <typename T> void SinglyLinkedList<T>::removeFront() {
  if (isEmpty()) {
    throw std::out_of_range("SinglyLinkedList::removeFront: list is empty");
  }
  Node *old = head_;
  head_ = head_->next;
  if (head_ == nullptr)
    tail_ = nullptr; // список стал пустым
  delete old;
  --size_;
}

template <typename T> void SinglyLinkedList<T>::removeBack() {
  if (isEmpty()) {
    throw std::out_of_range("SinglyLinkedList::removeBack: list is empty");
  }
  if (size_ == 1) {
    removeFront();
    return;
  }

  // O(n): ищем предпоследний узел
  Node *prev = head_;
  while (prev->next != tail_)
    prev = prev->next;
  delete tail_;
  tail_ = prev;
  prev->next = nullptr;
  --size_;
}

template <typename T> void SinglyLinkedList<T>::removeAt(std::size_t index) {
  if (index >= size_) {
    throw std::out_of_range("SinglyLinkedList::removeAt: index out of range");
  }
  if (index == 0) {
    removeFront();
    return;
  }
  if (index == size_ - 1) {
    removeBack();
    return;
  }

  Node *prev = head_;
  for (std::size_t i = 0; i + 1 < index; ++i)
    prev = prev->next;
  Node *victim = prev->next;
  prev->next = victim->next;
  delete victim;
  --size_;
}

// доступ / поиск

template <typename T> T &SinglyLinkedList<T>::at(std::size_t index) {
  if (index >= size_) {
    throw std::out_of_range("SinglyLinkedList::at: index out of range");
  }
  Node *cur = head_;
  for (std::size_t i = 0; i < index; ++i)
    cur = cur->next;
  return cur->value;
}

template <typename T>
const T &SinglyLinkedList<T>::at(std::size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("SinglyLinkedList::at: index out of range");
  }
  const Node *cur = head_;
  for (std::size_t i = 0; i < index; ++i)
    cur = cur->next;
  return cur->value;
}

template <typename T> bool SinglyLinkedList<T>::contains(const T &value) const {
  for (const Node *cur = head_; cur != nullptr; cur = cur->next) {
    if (cur->value == value)
      return true;
  }
  return false;
}

// сервисные методы

template <typename T> std::size_t SinglyLinkedList<T>::getSize() const {
  return size_;
}

template <typename T> bool SinglyLinkedList<T>::isEmpty() const {
  return size_ == 0;
}

template <typename T> void SinglyLinkedList<T>::clear() {
  while (head_ != nullptr) {
    Node *next = head_->next;
    delete head_;
    head_ = next;
  }
  tail_ = nullptr;
  size_ = 0;
}

// для хеш-таблицы
template <typename T>
template <typename Pred>
T *SinglyLinkedList<T>::findIf(Pred pred) {
  for (Node *cur = head_; cur != nullptr; cur = cur->next) {
    if (pred(cur->value))
      return &cur->value;
  }
  return nullptr;
}

template <typename T>
template <typename Pred>
const T *SinglyLinkedList<T>::findIf(Pred pred) const {
  for (const Node *cur = head_; cur != nullptr; cur = cur->next) {
    if (pred(cur->value))
      return &cur->value;
  }
  return nullptr;
}

template <typename T>
template <typename Pred>
bool SinglyLinkedList<T>::removeIf(Pred pred) {
  Node *cur = head_;
  Node *prev = nullptr;
  while (cur != nullptr) {
    if (pred(cur->value)) {
      if (prev == nullptr) {
        head_ = cur->next;
      } else {
        prev->next = cur->next;
      }
      if (cur == tail_)
        tail_ = prev;
      delete cur;
      --size_;
      return true;
    }
    prev = cur;
    cur = cur->next;
  }
  return false;
}
#endif