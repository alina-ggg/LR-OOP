#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "hash_table.h"

TEST(HashTableTest, DefaultConstructor) {
    HashTable<int, int> t;
    EXPECT_TRUE(t.isEmpty());
    EXPECT_EQ(t.getSize(), 0u);
    EXPECT_EQ(t.getBucketCount(), 8u);
    EXPECT_DOUBLE_EQ(t.loadFactor(), 0.0);
    EXPECT_FALSE(t.contains(1));
    EXPECT_THROW(t.at(1), std::out_of_range);
    EXPECT_FALSE(t.remove(1));
}

TEST(HashTableTest, InsertAndAt) {
    HashTable<int, int> t;
    t.insert(1, 10);
    t.insert(2, 20);
    EXPECT_EQ(t.getSize(), 2u);
    EXPECT_EQ(t.at(1), 10);
    EXPECT_EQ(t.at(2), 20);
    t.at(1) = 99;  // non-const ссылка
    EXPECT_EQ(t.at(1), 99);
}

TEST(HashTableTest, UpdateDoesNotGrow) {
    HashTable<int, int> t;
    t.insert(1, 10);
    t.insert(1, 20);  // тот же ключ -> обновление
    EXPECT_EQ(t.getSize(), 1u);
    EXPECT_EQ(t.at(1), 20);
}

TEST(HashTableTest, ContainsAndRemove) {
    HashTable<int, int> t;
    t.insert(1, 10);
    EXPECT_TRUE(t.contains(1));
    EXPECT_TRUE(t.remove(1));
    EXPECT_FALSE(t.contains(1));
    EXPECT_EQ(t.getSize(), 0u);
    EXPECT_FALSE(t.remove(1));  // повторное удаление -> false
}

TEST(HashTableTest, RemoveMissingReturnsFalse) {
    HashTable<int, int> t;
    t.insert(1, 10);
    EXPECT_FALSE(t.remove(42));
    EXPECT_EQ(t.getSize(), 1u);
}

TEST(HashTableTest, AtMissingThrows) {
    HashTable<int, int> t;
    t.insert(1, 10);
    EXPECT_THROW(t.at(42), std::out_of_range);
    const auto& cref = t;
    EXPECT_THROW(cref.at(42), std::out_of_range);
}

TEST(HashTableTest, StringKeys) {
    HashTable<std::string, int> t;
    t.insert("alice", 20);
    t.insert("bob", 25);
    EXPECT_EQ(t.at("alice"), 20);
    EXPECT_TRUE(t.contains("bob"));
    EXPECT_TRUE(t.remove("bob"));
    EXPECT_FALSE(t.contains("bob"));
}

//  коллизия: три ключа в одном бакете, цепочка их разрешает
TEST(HashTableTest, CollisionChaining) {
    HashTable<int, int> t;
    int a = 0, b = -1, c = -1;
    for (int k = 1; k < 10000 && (b < 0 || c < 0); ++k) {
        if (t.bucketIndexOf(k) == t.bucketIndexOf(a)) {
            if (b < 0) {
                b = k;
            } else if (c < 0) {
                c = k;
            }
        }
    }
    ASSERT_GE(b, 0);
    ASSERT_GE(c, 0);

    t.insert(a, 10);
    t.insert(b, 20);
    t.insert(c, 30);
    std::size_t idx = t.bucketIndexOf(a);
    EXPECT_EQ(t.bucketSize(idx), 3u);  // три элемента в одной цепочке
    EXPECT_EQ(t.at(a), 10);
    EXPECT_EQ(t.at(b), 20);
    EXPECT_EQ(t.at(c), 30);

    t.remove(b);  // удаление из середины цепочки
    EXPECT_EQ(t.bucketSize(idx), 2u);
    EXPECT_EQ(t.at(a), 10);
    EXPECT_EQ(t.at(c), 30);
    t.remove(a);  // удаление головы
    EXPECT_EQ(t.at(c), 30);
    t.remove(c);  // удаление последнего
    EXPECT_TRUE(t.isEmpty());
}

// Rehash: бакеты удваиваются, данные не теряются, load factor в норме
TEST(HashTableTest, RehashGrowsAndPreserves) {
    HashTable<int, int> t;
    for (int i = 0; i < 100; ++i)
        t.insert(i, i * 2);
    EXPECT_EQ(t.getSize(), 100u);
    EXPECT_GT(t.getBucketCount(), 8u);
    EXPECT_LE(t.loadFactor(), 0.75);
    for (int i = 0; i < 100; ++i)
        EXPECT_EQ(t.at(i), i * 2);
}

TEST(HashTableTest, ConstAccess) {
    HashTable<int, int> t;
    t.insert(1, 10);
    const auto& cref = t;
    EXPECT_EQ(cref.at(1), 10);
    EXPECT_TRUE(cref.contains(1));
    EXPECT_FALSE(cref.contains(2));
    const HashTable<int, int> empty;
    EXPECT_THROW(empty.at(1), std::out_of_range);
}

TEST(HashTableTest, CopyConstructorDeepCopy) {
    HashTable<int, int> t;
    t.insert(1, 10);
    HashTable<int, int> copy(t);
    copy.at(1) = 99;
    EXPECT_EQ(t.at(1), 10);  // оригинал не изменился
    EXPECT_EQ(copy.at(1), 99);
    copy.remove(1);
    EXPECT_TRUE(t.contains(1));
    HashTable<int, int> empty;
    HashTable<int, int> emptyCopy(empty);
    EXPECT_TRUE(emptyCopy.isEmpty());
}

TEST(HashTableTest, CopyAssignment) {
    HashTable<int, int> t;
    t.insert(1, 10);
    HashTable<int, int> other;
    other.insert(2, 20);
    other = t;
    EXPECT_EQ(other.at(1), 10);
    EXPECT_FALSE(other.contains(2));
    auto* ptr = &t;  // самоприсваивание
    t = *ptr;
    EXPECT_EQ(t.at(1), 10);
    HashTable<int, int> empty;
    t = empty;
    EXPECT_TRUE(t.isEmpty());
}

TEST(HashTableTest, MoveConstructor) {
    HashTable<int, int> t;
    t.insert(1, 10);
    HashTable<int, int> moved(std::move(t));
    EXPECT_EQ(moved.at(1), 10);
    EXPECT_TRUE(t.isEmpty());
    t.insert(2, 20);  // moved-from пригоден (guard восстановит бакеты)
    EXPECT_EQ(t.at(2), 20);
}

TEST(HashTableTest, MoveAssignment) {
    HashTable<int, int> t;
    t.insert(1, 10);
    HashTable<int, int> other;
    other.insert(2, 20);
    other = std::move(t);
    EXPECT_EQ(other.at(1), 10);
    EXPECT_TRUE(t.isEmpty());
    auto* ptr = &other;  // самоперемещение
    other = std::move(*ptr);
    EXPECT_EQ(other.at(1), 10);
}

TEST(HashTableTest, Clear) {
    HashTable<int, int> t;
    t.clear();
    EXPECT_TRUE(t.isEmpty());
    t.insert(1, 10);
    t.insert(2, 20);
    std::size_t buckets = t.getBucketCount();
    t.clear();
    EXPECT_TRUE(t.isEmpty());
    EXPECT_EQ(t.getBucketCount(), buckets);  // бакеты не сбрасываются
    t.insert(3, 30);
    EXPECT_EQ(t.at(3), 30);
}

TEST(HashTableTest, StringTableCopyMoveAndStats) {
    HashTable<std::string, int> t;
    t.insert("a", 1);
    t.insert("b", 2);
    EXPECT_GT(t.loadFactor(), 0.0);
    EXPECT_EQ(t.bucketSize(t.bucketIndexOf("a")), 1u);

    HashTable<std::string, int> copy(t);
    EXPECT_EQ(copy.at("a"), 1);
    HashTable<std::string, int> assigned;
    assigned = t;
    EXPECT_EQ(assigned.at("b"), 2);

    HashTable<std::string, int> moved(std::move(t));
    EXPECT_EQ(moved.at("a"), 1);
    EXPECT_TRUE(t.isEmpty());
    HashTable<std::string, int> moveAssigned;
    moveAssigned = std::move(moved);
    EXPECT_EQ(moveAssigned.at("b"), 2);
    EXPECT_TRUE(moved.isEmpty());
}

// Const-обход цепочки из двух узлов: закрывает advance в const findIf
TEST(HashTableTest, ConstLookupInChain) {
    HashTable<int, int> t;
    int a = 0, b = -1;
    for (int k = 1; k < 10000 && b < 0; ++k) {
        if (t.bucketIndexOf(k) == t.bucketIndexOf(a))
            b = k;
    }
    ASSERT_GE(b, 0);
    t.insert(a, 10);
    t.insert(b, 20);  // оба в одной цепочке

    const auto& cref = t;
    EXPECT_EQ(cref.at(b), 20);  // шагаем через голову цепочки ко второму узлу
    EXPECT_TRUE(cref.contains(b));
    EXPECT_FALSE(cref.contains(-1));  // полный обход цепочки без совпадения
}