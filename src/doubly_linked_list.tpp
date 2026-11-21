#ifndef DOUBLY_LINKED_LIST_TPP
#define DOUBLY_LINKED_LIST_TPP

#include <utility>  // std::swap

// конструкторы / деструктор

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
    : head_(nullptr), tail_(nullptr), size_(0) {
    for (const Node* cur = other.head_; cur != nullptr; cur = cur->next) {
        pushBack(cur->value);
    }
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

// присваивание / swap

template <typename T>
void DoublyLinkedList<T>::swap(DoublyLinkedList& other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
    if (this != &other) {
        DoublyLinkedList tmp(other);
        swap(tmp);
    }
    return *this;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&& other) noexcept {
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

// обход с ближайшего конца

template <typename T>
typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::nodeAt(std::size_t index) {
    if (index < size_ / 2) {
        Node* cur = head_;
        for (std::size_t i = 0; i < index; ++i)
            cur = cur->next;
        return cur;
    }
    Node* cur = tail_;
    for (std::size_t i = size_ - 1; i > index; --i)
        cur = cur->prev;
    return cur;
}

// вставка

template <typename T>
void DoublyLinkedList<T>::pushFront(const T& value) {
    Node* node = new Node(value, nullptr, head_);
    if (head_ != nullptr) {
        head_->prev = node;
    } else {
        tail_ = node;  // список был пуст
    }
    head_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(const T& value) {
    Node* node = new Node(value, tail_, nullptr);
    if (tail_ != nullptr) {
        tail_->next = node;
    } else {
        head_ = node;
    }
    tail_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(std::size_t index, const T& value) {
    if (index > size_) {
        throw std::out_of_range("DoublyLinkedList::insertAt: index out of range");
    }
    if (index == 0) {
        pushFront(value);
        return;
    }
    if (index == size_) {
        pushBack(value);
        return;
    }

    Node* cur = nodeAt(index);
    Node* node = new Node(value, cur->prev, cur);
    cur->prev->next = node;
    cur->prev = node;
    ++size_;
}

// удаление

template <typename T>
void DoublyLinkedList<T>::removeFront() {
    if (isEmpty()) {
        throw std::out_of_range("DoublyLinkedList::removeFront: list is empty");
    }
    Node* old = head_;
    head_ = head_->next;
    if (head_ != nullptr) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;  // список стал пустым
    }
    delete old;
    --size_;
}

template <typename T>
void DoublyLinkedList<T>::removeBack() {
    if (isEmpty()) {
        throw std::out_of_range("DoublyLinkedList::removeBack: list is empty");
    }
    Node* old = tail_;
    tail_ = tail_->prev;  // O(1): предпоследний известен сразу
    if (tail_ != nullptr) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }
    delete old;
    --size_;
}

template <typename T>
void DoublyLinkedList<T>::removeAt(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("DoublyLinkedList::removeAt: index out of range");
    }
    if (index == 0) {
        removeFront();
        return;
    }
    if (index == size_ - 1) {
        removeBack();
        return;
    }

    Node* cur = nodeAt(index);
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    --size_;
}

// доступ / поиск

template <typename T>
T& DoublyLinkedList<T>::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("DoublyLinkedList::at: index out of range");
    }
    return nodeAt(index)->value;
}

template <typename T>
const T& DoublyLinkedList<T>::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("DoublyLinkedList::at: index out of range");
    }
    const Node* cur = head_;
    for (std::size_t i = 0; i < index; ++i)
        cur = cur->next;
    return cur->value;
}

template <typename T>
bool DoublyLinkedList<T>::contains(const T& value) const {
    for (const Node* cur = head_; cur != nullptr; cur = cur->next) {
        if (cur->value == value)
            return true;
    }
    return false;
}

// сервисные методы

template <typename T>
std::size_t DoublyLinkedList<T>::getSize() const {
    return size_;
}

template <typename T>
bool DoublyLinkedList<T>::isEmpty() const {
    return size_ == 0;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while (head_ != nullptr) {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
}

template <typename T>
bool DoublyLinkedList<T>::insertBefore(const T& value, const T& newValue) {
    Node* cur = head_;
    while (cur != nullptr) {
        if (cur->value == value) {
            Node* newNode = new Node{newValue, cur->prev, cur};
            if (cur->prev != nullptr) {
                cur->prev->next = newNode;
            } else {
                head_ = newNode;
            }
            cur->prev = newNode;
            size_++;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <typename T>
bool DoublyLinkedList<T>::insertAfter(const T& value, const T& newValue) {
    Node* cur = head_;
    while (cur != nullptr) {
        if (cur->value == value) {
            Node* newNode = new Node{newValue, cur, cur->next};
            if (cur->next != nullptr) {
                cur->next->prev = newNode;
            } else {
                tail_ = newNode;
            }
            cur->next = newNode;
            size_++;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <typename T>
bool DoublyLinkedList<T>::removeBefore(const T& value) {
    Node* cur = head_;
    while (cur != nullptr) {
        if (cur->value == value) {
            if (cur->prev == nullptr)
                return false;
            Node* toDelete = cur->prev;
            if (toDelete->prev != nullptr) {
                toDelete->prev->next = cur;
            } else {
                head_ = cur;
            }
            cur->prev = toDelete->prev;
            delete toDelete;
            size_--;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <typename T>
bool DoublyLinkedList<T>::removeAfter(const T& value) {
    Node* cur = head_;
    while (cur != nullptr) {
        if (cur->value == value) {
            if (cur->next == nullptr)
                return false;
            Node* toDelete = cur->next;
            if (toDelete->next != nullptr) {
                toDelete->next->prev = cur;
            } else {
                tail_ = cur;
            }
            cur->next = toDelete->next;
            delete toDelete;
            size_--;
            return true;
        }
        cur = cur->next;
    }
    return false;
}
#endif