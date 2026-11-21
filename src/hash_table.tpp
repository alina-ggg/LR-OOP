#ifndef HASH_TABLE_TPP
#define HASH_TABLE_TPP

#include <utility>  // std::move

template <typename K, typename V>
HashTable<K, V>::HashTable() : HashTable(8) {}  // делегирующий конструктор

template <typename K, typename V>
HashTable<K, V>::HashTable(std::size_t bucketCount) : buckets_(), size_(0) {
    std::size_t n = bucketCount ? bucketCount : 1;
    for (std::size_t i = 0; i < n; ++i)
        buckets_.pushBack(SinglyLinkedList<Entry>());
}

template <typename K, typename V>
std::size_t HashTable<K, V>::bucketIndex(const K& key) const {
    return HashTraits<K>::hash(key) % buckets_.getSize();
}

template <typename K, typename V>
std::size_t HashTable<K, V>::bucketIndexOf(const K& key) const {
    return bucketIndex(key);
}

template <typename K, typename V>
std::size_t HashTable<K, V>::bucketSize(std::size_t index) const {
    return buckets_.at(index).getSize();
}

template <typename K, typename V>
HashTable<K, V>::HashTable(HashTable&& other) noexcept
    : buckets_(std::move(other.buckets_)), size_(other.size_) {
    other.size_ = 0;  // скаляр default-move не обнуляет — делаем вручную
}

template <typename K, typename V>
HashTable<K, V>& HashTable<K, V>::operator=(HashTable&& other) noexcept {
    if (this != &other) {
        buckets_ = std::move(other.buckets_);
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    Array<SinglyLinkedList<Entry>> newBuckets;
    for (std::size_t i = 0; i < buckets_.getSize() * 2; ++i) {
        newBuckets.pushBack(SinglyLinkedList<Entry>());
    }
    for (std::size_t b = 0; b < buckets_.getSize(); ++b) {
        // опустошаем цепочку с головы: at(0) + removeFront = O(1) на элемент
        while (!buckets_.at(b).isEmpty()) {
            Entry e = buckets_.at(b).at(0);
            buckets_.at(b).removeFront();
            std::size_t idx = HashTraits<K>::hash(e.key) % newBuckets.getSize();
            newBuckets.at(idx).pushBack(e);
        }
    }
    buckets_ = std::move(newBuckets);
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    if (buckets_.getSize() == 0) {  // moved-from состояние: восстанавливаем бакеты
        for (std::size_t i = 0; i < 8; ++i)
            buckets_.pushBack(SinglyLinkedList<Entry>());
    }
    std::size_t idx = bucketIndex(key);
    Entry* e = buckets_.at(idx).findIf([&](const Entry& en) { return en.key == key; });
    if (e != nullptr) {
        e->value = value;  // ключ уже есть -> обновляем, размер не растёт
        return;
    }
    buckets_.at(idx).pushBack(Entry(key, value));
    ++size_;
    if (loadFactor() > MAX_LOAD_FACTOR)
        rehash();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K& key) const {
    std::size_t idx = bucketIndex(key);
    return buckets_.at(idx).findIf([&](const Entry& en) { return en.key == key; }) != nullptr;
}

template <typename K, typename V>
V& HashTable<K, V>::at(const K& key) {
    std::size_t idx = bucketIndex(key);
    Entry* e = buckets_.at(idx).findIf([&](const Entry& en) { return en.key == key; });
    if (e == nullptr)
        throw std::out_of_range("HashTable::at: key not found");
    return e->value;
}

template <typename K, typename V>
const V& HashTable<K, V>::at(const K& key) const {
    std::size_t idx = bucketIndex(key);
    const Entry* e = buckets_.at(idx).findIf([&](const Entry& en) { return en.key == key; });
    if (e == nullptr)
        throw std::out_of_range("HashTable::at: key not found");
    return e->value;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    std::size_t idx = bucketIndex(key);
    bool removed = buckets_.at(idx).removeIf([&](const Entry& en) { return en.key == key; });
    if (removed)
        --size_;
    return removed;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    for (std::size_t b = 0; b < buckets_.getSize(); ++b)
        buckets_.at(b).clear();
    size_ = 0;
}

template <typename K, typename V>
std::size_t HashTable<K, V>::getSize() const {
    return size_;
}

template <typename K, typename V>
bool HashTable<K, V>::isEmpty() const {
    return size_ == 0;
}

template <typename K, typename V>
std::size_t HashTable<K, V>::getBucketCount() const {
    return buckets_.getSize();
}

template <typename K, typename V>
double HashTable<K, V>::loadFactor() const {
    return static_cast<double>(size_) / static_cast<double>(buckets_.getSize());
}

template <typename K, typename V>
template <typename Fn>
void HashTable<K, V>::forEach(Fn fn) const {
    for (std::size_t b = 0; b < buckets_.getSize(); ++b) {
        const auto& chain = buckets_.at(b);
        for (std::size_t i = 0; i < chain.getSize(); ++i) {
            const Entry& e = chain.at(i);
            fn(e.key, e.value);
        }
    }
}

#endif