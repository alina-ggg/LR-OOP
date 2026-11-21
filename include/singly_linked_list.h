#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T value;
        Node* next;
        explicit Node(const T& v, Node* n = nullptr) : value(v), next(n) {}
    };

    Node* head_;
    Node* tail_;  // хвост: чтобы pushBack был O(1)
    std::size_t size_;

    void swap(SinglyLinkedList& other) noexcept;

public:
    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList(SinglyLinkedList&& other) noexcept;
    ~SinglyLinkedList();

    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;

    void pushFront(const T& value);
    void pushBack(const T& value);
    void insertAt(std::size_t index, const T& value);

    void removeFront();
    void removeBack();  // O(n)
    void removeAt(std::size_t index);

    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    bool contains(const T& value) const;
    std::size_t getSize() const;
    bool isEmpty() const;
    void clear();

    // поиск/удаление по предикату за один проход O(n) — для хеш-таблицы
    template <typename Pred>
    T* findIf(Pred pred);
    template <typename Pred>
    const T* findIf(Pred pred) const;
    template <typename Pred>
    bool removeIf(Pred pred);

    bool insertBefore(const T& value, const T& newValue);
    bool insertAfter(const T& value, const T& newValue);
    bool removeBefore(const T& value);
    bool removeAfter(const T& value);
};

#include "../src/singly_linked_list.tpp"

#endif