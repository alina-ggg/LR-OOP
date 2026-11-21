#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "serializer.h"
#include "array.h"
#include "singly_linked_list.h"
#include "doubly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_binary_tree.h"

static void writeBadMagic(std::ostream& os) {
    std::uint32_t badMagic = 0xDEADBEEF;
    os.write(reinterpret_cast<const char*>(&badMagic), sizeof(badMagic));
}

TEST(SerializerTest, ArrayIntRoundTrip) {
    Array<int> original;
    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);
    original.pushBack(40);
    original.pushBack(50);

    std::stringstream ss;
    Serializer::saveArrayInt(original, ss);

    Array<int> loaded;
    Serializer::loadArrayInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, ArrayIntEmpty) {
    Array<int> original;

    std::stringstream ss;
    Serializer::saveArrayInt(original, ss);

    Array<int> loaded;
    Serializer::loadArrayInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, ArrayIntLarge) {
    Array<int> original;
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        original.pushBack(i * i);
    }

    std::stringstream ss;
    Serializer::saveArrayInt(original, ss);

    Array<int> loaded;
    Serializer::loadArrayInt(loaded, ss);

    ASSERT_EQ(static_cast<std::size_t>(N), loaded.getSize());
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(i * i, loaded.at(static_cast<std::size_t>(i)));
    }
}

TEST(SerializerTest, ArrayStringRoundTrip) {
    Array<std::string> original;
    original.pushBack("hello");
    original.pushBack("world");
    original.pushBack("test string with spaces");
    original.pushBack("");
    original.pushBack("special chars: !@#$%^&*()");

    std::stringstream ss;
    Serializer::saveArrayString(original, ss);

    Array<std::string> loaded;
    Serializer::loadArrayString(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, SinglyLinkedListIntRoundTrip) {
    SinglyLinkedList<int> original;
    original.pushBack(1);
    original.pushBack(2);
    original.pushBack(3);
    original.pushFront(0);  // 0, 1, 2, 3

    std::stringstream ss;
    Serializer::saveSinglyInt(original, ss);

    SinglyLinkedList<int> loaded;
    Serializer::loadSinglyInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, SinglyLinkedListIntEmpty) {
    SinglyLinkedList<int> original;

    std::stringstream ss;
    Serializer::saveSinglyInt(original, ss);

    SinglyLinkedList<int> loaded;
    Serializer::loadSinglyInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, SinglyLinkedListStringRoundTrip) {
    SinglyLinkedList<std::string> original;
    original.pushBack("alpha");
    original.pushBack("beta");
    original.pushBack("gamma");

    std::stringstream ss;
    Serializer::saveSinglyString(original, ss);

    SinglyLinkedList<std::string> loaded;
    Serializer::loadSinglyString(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, DoublyLinkedListIntRoundTrip) {
    DoublyLinkedList<int> original;
    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);
    original.pushFront(5);  // 5, 10, 20, 30

    std::stringstream ss;
    Serializer::saveDoublyInt(original, ss);

    DoublyLinkedList<int> loaded;
    Serializer::loadDoublyInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, DoublyLinkedListIntEmpty) {
    DoublyLinkedList<int> original;

    std::stringstream ss;
    Serializer::saveDoublyInt(original, ss);

    DoublyLinkedList<int> loaded;
    Serializer::loadDoublyInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, DoublyLinkedListStringRoundTrip) {
    DoublyLinkedList<std::string> original;
    original.pushBack("one");
    original.pushBack("two");
    original.pushBack("three");

    std::stringstream ss;
    Serializer::saveDoublyString(original, ss);

    DoublyLinkedList<std::string> loaded;
    Serializer::loadDoublyString(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(SerializerTest, StackIntRoundTrip) {
    Stack<int> original;
    original.push(1);
    original.push(2);
    original.push(3);
    original.push(4);
    original.push(5);

    std::stringstream ss;
    Serializer::saveStackInt(original, ss);

    Stack<int> loaded;
    Serializer::loadStackInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    while (!original.isEmpty() && !loaded.isEmpty()) {
        EXPECT_EQ(original.pop(), loaded.pop());
    }
}

TEST(SerializerTest, StackIntEmpty) {
    Stack<int> original;

    std::stringstream ss;
    Serializer::saveStackInt(original, ss);

    Stack<int> loaded;
    Serializer::loadStackInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, StackStringRoundTrip) {
    Stack<std::string> original;
    original.push("first");
    original.push("second");
    original.push("third");

    std::stringstream ss;
    Serializer::saveStackString(original, ss);

    Stack<std::string> loaded;
    Serializer::loadStackString(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(original.pop(), loaded.pop());
    EXPECT_EQ(original.pop(), loaded.pop());
    EXPECT_EQ(original.pop(), loaded.pop());
}

TEST(SerializerTest, QueueIntRoundTrip) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);
    original.enqueue(40);

    std::stringstream ss;
    Serializer::saveQueueInt(original, ss);

    Queue<int> loaded;
    Serializer::loadQueueInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    while (!original.isEmpty() && !loaded.isEmpty()) {
        EXPECT_EQ(original.dequeue(), loaded.dequeue());
    }
}

TEST(SerializerTest, QueueIntEmpty) {
    Queue<int> original;

    std::stringstream ss;
    Serializer::saveQueueInt(original, ss);

    Queue<int> loaded;
    Serializer::loadQueueInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, QueueStringRoundTrip) {
    Queue<std::string> original;
    original.enqueue("alpha");
    original.enqueue("beta");
    original.enqueue("gamma");

    std::stringstream ss;
    Serializer::saveQueueString(original, ss);

    Queue<std::string> loaded;
    Serializer::loadQueueString(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
}

TEST(SerializerTest, HashTableIntIntRoundTrip) {
    HashTable<int, int> original;
    original.insert(1, 100);
    original.insert(2, 200);
    original.insert(3, 300);
    original.insert(100, 1000);

    std::stringstream ss;
    Serializer::saveHashIntInt(original, ss);

    HashTable<int, int> loaded;
    Serializer::loadHashIntInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(100, loaded.at(1));
    EXPECT_EQ(200, loaded.at(2));
    EXPECT_EQ(300, loaded.at(3));
    EXPECT_EQ(1000, loaded.at(100));
}

TEST(SerializerTest, HashTableIntIntEmpty) {
    HashTable<int, int> original;

    std::stringstream ss;
    Serializer::saveHashIntInt(original, ss);

    HashTable<int, int> loaded;
    Serializer::loadHashIntInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
}

TEST(SerializerTest, HashTableStringIntRoundTrip) {
    HashTable<std::string, int> original;
    original.insert("alice", 20);
    original.insert("bob", 25);
    original.insert("charlie", 30);

    std::stringstream ss;
    Serializer::saveHashStringInt(original, ss);

    HashTable<std::string, int> loaded;
    Serializer::loadHashStringInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(20, loaded.at("alice"));
    EXPECT_EQ(25, loaded.at("bob"));
    EXPECT_EQ(30, loaded.at("charlie"));
}

TEST(SerializerTest, CompleteBinaryTreeIntRoundTrip) {
    CompleteBinaryTree<int> original;
    for (int i = 1; i <= 7; ++i) {
        original.insert(i);
    }

    std::stringstream ss;
    Serializer::saveTreeInt(original, ss);

    CompleteBinaryTree<int> loaded;
    Serializer::loadTreeInt(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
    EXPECT_EQ(1, loaded.at(0));
    EXPECT_EQ(2, loaded.at(1));
    EXPECT_EQ(3, loaded.at(2));
    EXPECT_EQ(4, loaded.at(3));
}

TEST(SerializerTest, CompleteBinaryTreeIntEmpty) {
    CompleteBinaryTree<int> original;

    std::stringstream ss;
    Serializer::saveTreeInt(original, ss);

    CompleteBinaryTree<int> loaded;
    Serializer::loadTreeInt(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(SerializerTest, BadMagicThrows) {
    std::stringstream ss;
    writeBadMagic(ss);

    // Добавляем остальные поля заголовка, чтобы поток был валидным после magic
    std::uint32_t version = 1;
    ss.write(reinterpret_cast<const char*>(&version), sizeof(version));
    std::string tag = "ARRAY_INT";
    std::uint32_t tagLen = static_cast<std::uint32_t>(tag.size());
    ss.write(reinterpret_cast<const char*>(&tagLen), sizeof(tagLen));
    ss.write(tag.data(), tagLen);
    std::uint64_t size = 0;
    ss.write(reinterpret_cast<const char*>(&size), sizeof(size));

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayInt(loaded, ss), std::runtime_error);
}

TEST(SerializerTest, BadVersionThrows) {
    std::stringstream ss;
    std::uint32_t magic = Serializer::kMagic;
    ss.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
    std::uint32_t badVersion = 999;
    ss.write(reinterpret_cast<const char*>(&badVersion), sizeof(badVersion));

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayInt(loaded, ss), std::runtime_error);
}

TEST(SerializerTest, BadTagThrows) {
    std::stringstream ss;
    std::uint32_t magic = Serializer::kMagic;
    ss.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
    std::uint32_t version = Serializer::kVersion;
    ss.write(reinterpret_cast<const char*>(&version), sizeof(version));
    std::string badTag = "WRONG_TAG";
    std::uint32_t tagLen = static_cast<std::uint32_t>(badTag.size());
    ss.write(reinterpret_cast<const char*>(&tagLen), sizeof(tagLen));
    ss.write(badTag.data(), tagLen);

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayInt(loaded, ss), std::runtime_error);
}

TEST(SerializerTest, LoadClearsExistingData) {
    Array<int> original;
    original.pushBack(10);
    original.pushBack(20);

    Array<int> existing;
    existing.pushBack(999);
    existing.pushBack(888);
    existing.pushBack(777);

    std::stringstream ss;
    Serializer::saveArrayInt(original, ss);

    Serializer::loadArrayInt(existing, ss);

    ASSERT_EQ(2u, existing.getSize());
    EXPECT_EQ(10, existing.at(0));
    EXPECT_EQ(20, existing.at(1));
}