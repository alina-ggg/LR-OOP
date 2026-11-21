#include "array.h"
#include "complete_binary_tree.h"
#include "doubly_linked_list.h"
#include "hash_table.h"
#include "queue.h"
#include "singly_linked_list.h"
#include "stack.h"
#include "serializer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <system_error>

// Вспомогательная функция для создания папки data, если её нет
void ensureDirectoryExists(const std::string& path) {
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
}

int main() {
    std::cout << "  Демонстрация операций и сериализации\n";

    std::cout << "1. Базовые операции \n";

    Array<int> arr;
    arr.pushBack(10);
    arr.pushBack(20);
    arr.insertAt(1, 15);

    std::cout << "Array: ";
    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        std::cout << arr.at(i) << " ";  // Используем at() для безопасности
    }
    std::cout << "(size=" << arr.getSize() << ", capacity=" << arr.getCapacity() << ")\n";

    arr.removeAt(0);
    std::cout << "Array after removeAt(0): " << arr.at(0) << ", " << arr.at(1) << "\n";

    SinglyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushFront(0);
    list.insertAt(2, 2);  // 0 1 2 3
    list.removeBack();    // 0 1 2
    list.removeFront();   // 1 2
    std::cout << "SinglyLinkedList contains(2): " << std::boolalpha << list.contains(2) << "\n";

    DoublyLinkedList<int> dl;
    dl.pushBack(2);
    dl.pushFront(1);
    dl.pushBack(3);     // 1 2 3
    dl.removeBack();    // 1 2 — O(1)
    dl.insertAt(1, 9);  // 1 9 2
    std::cout << "DoublyLinkedList: ";
    for (std::size_t i = 0; i < dl.getSize(); ++i) {
        std::cout << dl.at(i) << " ";
    }
    std::cout << "\n";

    Stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    std::cout << "Stack pop: " << st.pop() << " " << st.pop() << " (LIFO)\n";

    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    std::cout << "Queue dequeue: " << q.dequeue() << " " << q.dequeue() << " (FIFO)\n";
    std::cout << "Queue front: " << q.front() << "\n";

    HashTable<std::string, int> ages;
    ages.insert("alice", 20);
    ages.insert("bob", 25);
    ages.insert("alice", 21);  // обновление существующего ключа
    std::cout << "HashTable: alice=" << ages.at("alice") << ", bob=" << ages.at("bob")
              << " (size=" << ages.getSize() << ")\n";
    ages.remove("bob");
    std::cout << "HashTable after remove: size=" << ages.getSize() << "\n";

    CompleteBinaryTree<int> tree;
    for (int v = 1; v <= 7; ++v) {
        tree.insert(v);
    }
    std::cout << "CompleteBinaryTree: height=" << tree.getHeight() << ", size=" << tree.getSize()
              << ", contains(5)=" << std::boolalpha << tree.contains(5) << "\n\n";

    std::cout << " 2. Демонстрация сериализации \n";
    ensureDirectoryExists("data");

    // 2.1. Сериализация массива (Array<int>)
    std::cout << "\n[Array<int>]\n";
    Array<int> arrToSave;
    arrToSave.pushBack(100);
    arrToSave.pushBack(200);
    arrToSave.pushBack(300);

    // Бинарная сериализация
    {
        std::ofstream binFile("data/array.bin", std::ios::binary);
        Serializer::saveArrayInt(arrToSave, binFile);
        std::cout << "  [OK] Сохранено в бинарный файл: data/array.bin\n";
    }
    // Текстовая сериализация
    {
        std::ofstream txtFile("data/array.txt");
        Serializer::saveArrayIntText(arrToSave, txtFile);
        std::cout << "  [OK] Сохранено в текстовый файл: data/array.txt\n";
    }

    // Проверка загрузки
    {
        Array<int> loadedBin, loadedTxt;
        std::ifstream inBin("data/array.bin", std::ios::binary);
        Serializer::loadArrayInt(loadedBin, inBin);

        std::ifstream inTxt("data/array.txt");
        Serializer::loadArrayIntText(loadedTxt, inTxt);

        std::cout << "  Загружено из BIN: ";
        for (std::size_t i = 0; i < loadedBin.getSize(); ++i)
            std::cout << loadedBin.at(i) << " ";
        std::cout << "\n";

        std::cout << "  Загружено из TXT: ";
        for (std::size_t i = 0; i < loadedTxt.getSize(); ++i)
            std::cout << loadedTxt.at(i) << " ";
        std::cout << "\n";
    }

    // 2.2. Сериализация хеш-таблицы (HashTable<std::string, int>)
    std::cout << "\n[HashTable<std::string, int>]\n";
    HashTable<std::string, int> hashToSave;
    hashToSave.insert("apple", 10);
    hashToSave.insert("banana", 20);
    hashToSave.insert("cherry", 30);

    // Бинарная сериализация
    {
        std::ofstream binFile("data/hash.bin", std::ios::binary);
        Serializer::saveHashStringInt(hashToSave, binFile);
        std::cout << "  [OK] Сохранено в бинарный файл: data/hash.bin\n";
    }
    // Текстовая сериализация
    {
        std::ofstream txtFile("data/hash.txt");
        Serializer::saveHashStringIntText(hashToSave, txtFile);
        std::cout << "  [OK] Сохранено в текстовый файл: data/hash.txt\n";
    }

    // Проверка загрузки (Round-trip)
    {
        HashTable<std::string, int> loadedBin, loadedTxt;
        std::ifstream inBin("data/hash.bin", std::ios::binary);
        Serializer::loadHashStringInt(loadedBin, inBin);

        std::ifstream inTxt("data/hash.txt");
        Serializer::loadHashStringIntText(loadedTxt, inTxt);

        std::cout << "  Загружено из BIN: apple=" << loadedBin.at("apple")
                  << ", banana=" << loadedBin.at("banana") << "\n";
        std::cout << "  Загружено из TXT: apple=" << loadedTxt.at("apple")
                  << ", banana=" << loadedTxt.at("banana") << "\n";
    }

    std::cout << "  Демонстрация завершена успешно!\n";
    std::cout << "  Файлы сохранены в папке data/\n";

    return 0;
}