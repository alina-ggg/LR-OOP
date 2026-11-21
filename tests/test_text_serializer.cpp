#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "serializer.h"

static std::string readStream(std::stringstream& ss) {
    return ss.str();
}

TEST(TextSerializerTest, ArrayIntRoundTrip) {
    Array<int> original;
    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);
    original.pushBack(40);
    original.pushBack(50);

    std::stringstream ss;
    Serializer::saveArrayIntText(original, ss);

    std::string content = readStream(ss);
    EXPECT_TRUE(content.find("ARRAY_INT 5") != std::string::npos);

    Array<int> loaded;
    Serializer::loadArrayIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, ArrayIntEmpty) {
    Array<int> original;
    std::stringstream ss;
    Serializer::saveArrayIntText(original, ss);

    Array<int> loaded;
    Serializer::loadArrayIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(TextSerializerTest, ArrayIntLarge) {
    Array<int> original;
    const int N = 100;
    for (int i = 0; i < N; ++i)
        original.pushBack(i * i);

    std::stringstream ss;
    Serializer::saveArrayIntText(original, ss);

    Array<int> loaded;
    Serializer::loadArrayIntText(loaded, ss);

    ASSERT_EQ(static_cast<std::size_t>(N), loaded.getSize());
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(i * i, loaded.at(static_cast<std::size_t>(i)));
    }
}

TEST(TextSerializerTest, ArrayStringRoundTrip) {
    Array<std::string> original;
    original.pushBack("hello world");
    original.pushBack("line\nbreak");
    original.pushBack("tab\there");
    original.pushBack("quote\"inside");
    original.pushBack("backslash\\here");
    original.pushBack("");

    std::stringstream ss;
    Serializer::saveArrayStringText(original, ss);

    // Проверяем, что строки экранированы
    std::string content = readStream(ss);
    EXPECT_TRUE(content.find("\"hello world\"") != std::string::npos);
    EXPECT_TRUE(content.find("\"line\\nbreak\"") != std::string::npos);
    EXPECT_TRUE(content.find("\"tab\\there\"") != std::string::npos);
    EXPECT_TRUE(content.find("\"quote\\\"inside\"") != std::string::npos);

    Array<std::string> loaded;
    Serializer::loadArrayStringText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, ArrayStringEmpty) {
    Array<std::string> original;
    std::stringstream ss;
    Serializer::saveArrayStringText(original, ss);

    Array<std::string> loaded;
    Serializer::loadArrayStringText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
}

TEST(TextSerializerTest, SinglyLinkedListIntRoundTrip) {
    SinglyLinkedList<int> original;
    original.pushBack(1);
    original.pushBack(2);
    original.pushBack(3);
    original.pushFront(0);

    std::stringstream ss;
    Serializer::saveSinglyIntText(original, ss);

    SinglyLinkedList<int> loaded;
    Serializer::loadSinglyIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, SinglyLinkedListIntEmpty) {
    SinglyLinkedList<int> original;
    std::stringstream ss;
    Serializer::saveSinglyIntText(original, ss);

    SinglyLinkedList<int> loaded;
    Serializer::loadSinglyIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
}

TEST(TextSerializerTest, SinglyLinkedListStringRoundTrip) {
    SinglyLinkedList<std::string> original;
    original.pushBack("alpha");
    original.pushBack("beta");
    original.pushBack("gamma");

    std::stringstream ss;
    Serializer::saveSinglyStringText(original, ss);

    SinglyLinkedList<std::string> loaded;
    Serializer::loadSinglyStringText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, DoublyLinkedListIntRoundTrip) {
    DoublyLinkedList<int> original;
    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);
    original.pushFront(5);

    std::stringstream ss;
    Serializer::saveDoublyIntText(original, ss);

    DoublyLinkedList<int> loaded;
    Serializer::loadDoublyIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, DoublyLinkedListIntEmpty) {
    DoublyLinkedList<int> original;
    std::stringstream ss;
    Serializer::saveDoublyIntText(original, ss);

    DoublyLinkedList<int> loaded;
    Serializer::loadDoublyIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
}

TEST(TextSerializerTest, DoublyLinkedListStringRoundTrip) {
    DoublyLinkedList<std::string> original;
    original.pushBack("one");
    original.pushBack("two");
    original.pushBack("three");

    std::stringstream ss;
    Serializer::saveDoublyStringText(original, ss);

    DoublyLinkedList<std::string> loaded;
    Serializer::loadDoublyStringText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
}

TEST(TextSerializerTest, StackIntRoundTrip) {
    Stack<int> original;
    original.push(1);
    original.push(2);
    original.push(3);
    original.push(4);
    original.push(5);

    std::stringstream ss;
    Serializer::saveStackIntText(original, ss);

    Stack<int> loaded;
    Serializer::loadStackIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    // Проверяем LIFO-порядок
    while (!original.isEmpty() && !loaded.isEmpty()) {
        EXPECT_EQ(original.pop(), loaded.pop());
    }
}

TEST(TextSerializerTest, StackIntEmpty) {
    Stack<int> original;
    std::stringstream ss;
    Serializer::saveStackIntText(original, ss);

    Stack<int> loaded;
    Serializer::loadStackIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(TextSerializerTest, StackStringRoundTrip) {
    Stack<std::string> original;
    original.push("first");
    original.push("second");
    original.push("third");

    std::stringstream ss;
    Serializer::saveStackStringText(original, ss);

    Stack<std::string> loaded;
    Serializer::loadStackStringText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(original.pop(), loaded.pop());
    EXPECT_EQ(original.pop(), loaded.pop());
    EXPECT_EQ(original.pop(), loaded.pop());
}

TEST(TextSerializerTest, QueueIntRoundTrip) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);
    original.enqueue(40);

    std::stringstream ss;
    Serializer::saveQueueIntText(original, ss);

    Queue<int> loaded;
    Serializer::loadQueueIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    // Проверяем FIFO-порядок
    while (!original.isEmpty() && !loaded.isEmpty()) {
        EXPECT_EQ(original.dequeue(), loaded.dequeue());
    }
}

TEST(TextSerializerTest, QueueIntEmpty) {
    Queue<int> original;
    std::stringstream ss;
    Serializer::saveQueueIntText(original, ss);

    Queue<int> loaded;
    Serializer::loadQueueIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(TextSerializerTest, QueueStringRoundTrip) {
    Queue<std::string> original;
    original.enqueue("alpha");
    original.enqueue("beta");
    original.enqueue("gamma");

    std::stringstream ss;
    Serializer::saveQueueStringText(original, ss);

    Queue<std::string> loaded;
    Serializer::loadQueueStringText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
    EXPECT_EQ(original.dequeue(), loaded.dequeue());
}

TEST(TextSerializerTest, HashTableIntIntRoundTrip) {
    HashTable<int, int> original;
    original.insert(1, 100);
    original.insert(2, 200);
    original.insert(3, 300);
    original.insert(100, 1000);

    std::stringstream ss;
    Serializer::saveHashIntIntText(original, ss);

    HashTable<int, int> loaded;
    Serializer::loadHashIntIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(100, loaded.at(1));
    EXPECT_EQ(200, loaded.at(2));
    EXPECT_EQ(300, loaded.at(3));
    EXPECT_EQ(1000, loaded.at(100));
}

TEST(TextSerializerTest, HashTableIntIntEmpty) {
    HashTable<int, int> original;
    std::stringstream ss;
    Serializer::saveHashIntIntText(original, ss);

    HashTable<int, int> loaded;
    Serializer::loadHashIntIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
}

TEST(TextSerializerTest, HashTableStringIntRoundTrip) {
    HashTable<std::string, int> original;
    original.insert("alice", 20);
    original.insert("bob", 25);
    original.insert("charlie", 30);

    std::stringstream ss;
    Serializer::saveHashStringIntText(original, ss);

    HashTable<std::string, int> loaded;
    Serializer::loadHashStringIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    EXPECT_EQ(20, loaded.at("alice"));
    EXPECT_EQ(25, loaded.at("bob"));
    EXPECT_EQ(30, loaded.at("charlie"));
}

TEST(TextSerializerTest, CompleteBinaryTreeIntRoundTrip) {
    CompleteBinaryTree<int> original;
    for (int i = 1; i <= 7; ++i) {
        original.insert(i);
    }

    std::stringstream ss;
    Serializer::saveTreeIntText(original, ss);

    CompleteBinaryTree<int> loaded;
    Serializer::loadTreeIntText(loaded, ss);

    ASSERT_EQ(original.getSize(), loaded.getSize());
    for (std::size_t i = 0; i < original.getSize(); ++i) {
        EXPECT_EQ(original.at(i), loaded.at(i));
    }
    EXPECT_EQ(1, loaded.at(0));
    EXPECT_EQ(2, loaded.at(1));
    EXPECT_EQ(3, loaded.at(2));
    EXPECT_EQ(4, loaded.at(3));
}

TEST(TextSerializerTest, CompleteBinaryTreeIntEmpty) {
    CompleteBinaryTree<int> original;
    std::stringstream ss;
    Serializer::saveTreeIntText(original, ss);

    CompleteBinaryTree<int> loaded;
    Serializer::loadTreeIntText(loaded, ss);

    EXPECT_EQ(0u, loaded.getSize());
    EXPECT_TRUE(loaded.isEmpty());
}

TEST(TextSerializerTest, BadTextHeaderThrows) {
    std::stringstream ss;
    ss << "WRONG_TAG 3\n1\n2\n3\n";

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayIntText(loaded, ss), std::runtime_error);
}

TEST(TextSerializerTest, BadTextHeaderFormatThrows) {
    std::stringstream ss;
    ss << "garbage\n";

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayIntText(loaded, ss), std::runtime_error);
}

TEST(TextSerializerTest, EmptyStreamThrows) {
    std::stringstream ss;  // пустой поток

    Array<int> loaded;
    EXPECT_THROW(Serializer::loadArrayIntText(loaded, ss), std::runtime_error);
}

TEST(TextSerializerTest, BadHashLineThrows) {
    std::stringstream ss;
    ss << "HASH_INT_INT 1\n123\n";

    HashTable<int, int> loaded;
    EXPECT_THROW(Serializer::loadHashIntIntText(loaded, ss), std::runtime_error);
}

TEST(TextSerializerTest, LoadTextClearsExistingData) {
    Array<int> original;
    original.pushBack(10);
    original.pushBack(20);

    Array<int> existing;
    existing.pushBack(999);
    existing.pushBack(888);
    existing.pushBack(777);

    std::stringstream ss;
    Serializer::saveArrayIntText(original, ss);

    Serializer::loadArrayIntText(existing, ss);

    ASSERT_EQ(2u, existing.getSize());
    EXPECT_EQ(10, existing.at(0));
    EXPECT_EQ(20, existing.at(1));
}