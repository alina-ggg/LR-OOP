#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <string>
#include <cstdint>
#include "array.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_binary_tree.h"

class Serializer {
public:
    // Magic number и версия для бинарного формата
    static constexpr std::uint32_t kMagic = 0x4C523353u;  // "LR3S"
    static constexpr std::uint32_t kVersion = 1;

    // бинарная

    // Массив
    static void saveArrayInt(const Array<int>& arr, std::ostream& os);
    static void loadArrayInt(Array<int>& arr, std::istream& is);
    static void saveArrayString(const Array<std::string>& arr, std::ostream& os);
    static void loadArrayString(Array<std::string>& arr, std::istream& is);

    // Односвязный список
    static void saveSinglyInt(const SinglyLinkedList<int>& list, std::ostream& os);
    static void loadSinglyInt(SinglyLinkedList<int>& list, std::istream& is);
    static void saveSinglyString(const SinglyLinkedList<std::string>& list, std::ostream& os);
    static void loadSinglyString(SinglyLinkedList<std::string>& list, std::istream& is);

    // Двусвязный список
    static void saveDoublyInt(const DoublyLinkedList<int>& list, std::ostream& os);
    static void loadDoublyInt(DoublyLinkedList<int>& list, std::istream& is);
    static void saveDoublyString(const DoublyLinkedList<std::string>& list, std::ostream& os);
    static void loadDoublyString(DoublyLinkedList<std::string>& list, std::istream& is);

    // Стек
    static void saveStackInt(const Stack<int>& stack, std::ostream& os);
    static void loadStackInt(Stack<int>& stack, std::istream& is);
    static void saveStackString(const Stack<std::string>& stack, std::ostream& os);
    static void loadStackString(Stack<std::string>& stack, std::istream& is);

    // Очередь
    static void saveQueueInt(const Queue<int>& queue, std::ostream& os);
    static void loadQueueInt(Queue<int>& queue, std::istream& is);
    static void saveQueueString(const Queue<std::string>& queue, std::ostream& os);
    static void loadQueueString(Queue<std::string>& queue, std::istream& is);

    // Хеш-таблица
    static void saveHashIntInt(const HashTable<int, int>& table, std::ostream& os);
    static void loadHashIntInt(HashTable<int, int>& table, std::istream& is);
    static void saveHashStringInt(const HashTable<std::string, int>& table, std::ostream& os);
    static void loadHashStringInt(HashTable<std::string, int>& table, std::istream& is);

    // Дерево
    static void saveTreeInt(const CompleteBinaryTree<int>& tree, std::ostream& os);
    static void loadTreeInt(CompleteBinaryTree<int>& tree, std::istream& is);

    // текстовая

    // Массив
    static void saveArrayIntText(const Array<int>& arr, std::ostream& os);
    static void loadArrayIntText(Array<int>& arr, std::istream& is);
    static void saveArrayStringText(const Array<std::string>& arr, std::ostream& os);
    static void loadArrayStringText(Array<std::string>& arr, std::istream& is);

    // Односвязный список
    static void saveSinglyIntText(const SinglyLinkedList<int>& list, std::ostream& os);
    static void loadSinglyIntText(SinglyLinkedList<int>& list, std::istream& is);
    static void saveSinglyStringText(const SinglyLinkedList<std::string>& list, std::ostream& os);
    static void loadSinglyStringText(SinglyLinkedList<std::string>& list, std::istream& is);

    // Двусвязный список
    static void saveDoublyIntText(const DoublyLinkedList<int>& list, std::ostream& os);
    static void loadDoublyIntText(DoublyLinkedList<int>& list, std::istream& is);
    static void saveDoublyStringText(const DoublyLinkedList<std::string>& list, std::ostream& os);
    static void loadDoublyStringText(DoublyLinkedList<std::string>& list, std::istream& is);

    // Стек
    static void saveStackIntText(const Stack<int>& stack, std::ostream& os);
    static void loadStackIntText(Stack<int>& stack, std::istream& is);
    static void saveStackStringText(const Stack<std::string>& stack, std::ostream& os);
    static void loadStackStringText(Stack<std::string>& stack, std::istream& is);

    // Очередь
    static void saveQueueIntText(const Queue<int>& queue, std::ostream& os);
    static void loadQueueIntText(Queue<int>& queue, std::istream& is);
    static void saveQueueStringText(const Queue<std::string>& queue, std::ostream& os);
    static void loadQueueStringText(Queue<std::string>& queue, std::istream& is);

    // Хеш-таблица
    static void saveHashIntIntText(const HashTable<int, int>& table, std::ostream& os);
    static void loadHashIntIntText(HashTable<int, int>& table, std::istream& is);
    static void saveHashStringIntText(const HashTable<std::string, int>& table, std::ostream& os);
    static void loadHashStringIntText(HashTable<std::string, int>& table, std::istream& is);

    // Дерево
    static void saveTreeIntText(const CompleteBinaryTree<int>& tree, std::ostream& os);
    static void loadTreeIntText(CompleteBinaryTree<int>& tree, std::istream& is);

private:
    // помощники для бинарного формата
    static void writeHeader(std::ostream& os, const std::string& tag, std::size_t size);
    static std::size_t readHeader(std::istream& is, const std::string& expectedTag);
    static void writeString(std::ostream& os, const std::string& str);
    static std::string readString(std::istream& is);

    // помощники для текстового формата
    static void writeTextHeader(std::ostream& os, const std::string& tag, std::size_t size);
    static std::size_t readTextHeader(std::istream& is, const std::string& expectedTag);
    static std::string quoteString(const std::string& s);
    static std::string unquoteString(const std::string& s);
};

#endif