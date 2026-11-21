#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* prev;  // отличие от односвязного
        Node* next;
        explicit Node(const T& v, Node* p = nullptr, Node* n = nullptr)
            : value(v), prev(p), next(n) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

    void swap(DoublyLinkedList& other) noexcept;
    // обход с ближайшего конца: O(n/2) вместо O(n)
    Node* nodeAt(std::size_t index);

public:
    DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    ~DoublyLinkedList();

    DoublyLinkedList& operator=(const DoublyLinkedList& other);
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;

    void pushFront(const T& value);
    void pushBack(const T& value);
    void insertAt(std::size_t index, const T& value);

    void removeFront();
    void removeBack();
    void removeAt(std::size_t index);

    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    bool insertBefore(const T& value, const T& newValue);
    bool insertAfter(const T& value, const T& newValue);
    bool removeBefore(const T& value);
    bool removeAfter(const T& value);
    bool contains(const T& value) const;
    std::size_t getSize() const;
    bool isEmpty() const;
    void clear();
};

#include "../src/doubly_linked_list.tpp"

#endif