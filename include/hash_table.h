#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstddef>
#include <stdexcept>
#include <string>

#include "array.h"
#include "hash_utils.h"
#include "singly_linked_list.h"

// специализации шаблона выбирают хеш-функцию под тип ключа
template <typename K>
struct HashTraits;
template <>
struct HashTraits<int> {
    static std::size_t hash(int k) { return hashInt(k); }
};
template <>
struct HashTraits<std::string> {
    static std::size_t hash(const std::string& k) { return hashString(k); }
};

// хеш-таблица, метод цепочек
template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    Array<SinglyLinkedList<Entry>> buckets_;
    std::size_t size_;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    std::size_t bucketIndex(const K& key) const;
    void rehash();

public:
    HashTable();
    explicit HashTable(std::size_t bucketCount);
    HashTable(const HashTable&) = default;
    HashTable(HashTable&&) noexcept;
    HashTable& operator=(const HashTable&) = default;
    HashTable& operator=(HashTable&&) noexcept;
    ~HashTable() = default;

    void insert(const K& key, const V& value);  // вставка или обновление
    bool contains(const K& key) const;
    V& at(const K& key);
    const V& at(const K& key) const;
    bool remove(const K& key);
    void clear();

    std::size_t getSize() const;
    bool isEmpty() const;
    std::size_t getBucketCount() const;
    double loadFactor() const;
    std::size_t bucketIndexOf(const K& key) const;  // для демонстрации коллизий
    std::size_t bucketSize(std::size_t index) const;

    template <typename Fn>
    void forEach(Fn fn) const;
};

#include "../src/hash_table.tpp"

#endif