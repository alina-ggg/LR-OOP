#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "serializer.h"

namespace {
const std::string kDir = "data/output/";
} // namespace

TEST(SerializerTest, ArrayRoundTripBinary) {
  Array<int> src;
  for (int i = 1; i <= 5; ++i)
    src.pushBack(i * 10);
  {
    std::ofstream os(kDir + "array.bin", std::ios::binary);
    saveBinary(src, os);
  }
  Array<int> dst;
  {
    std::ifstream is(kDir + "array.bin", std::ios::binary);
    loadBinary(dst, is);
  }
  ASSERT_EQ(dst.getSize(), src.getSize());
  for (std::size_t i = 0; i < src.getSize(); ++i)
    EXPECT_EQ(dst[i], src[i]);
  // компактность бинарного формата: заголовок 8 + size 8 + 5*4 = 36 байт
  EXPECT_EQ(std::filesystem::file_size(kDir + "array.bin"), 36u);
}

TEST(SerializerTest, StringArrayRoundTrip) {
  Array<std::string> src;
  src.pushBack("hello");
  src.pushBack("");
  src.pushBack("world!");
  {
    std::ofstream os(kDir + "str.bin", std::ios::binary);
    saveBinary(src, os);
  }
  Array<std::string> dst;
  {
    std::ifstream is(kDir + "str.bin", std::ios::binary);
    loadBinary(dst, is);
  }
  ASSERT_EQ(dst.getSize(), 3u);
  EXPECT_EQ(dst[0], "hello");
  EXPECT_EQ(dst[1], "");
  EXPECT_EQ(dst[2], "world!");
}

TEST(SerializerTest, ListsRoundTrip) {
  SinglyLinkedList<int> s;
  DoublyLinkedList<int> d;
  for (int i = 0; i < 4; ++i) {
    s.pushBack(i);
    d.pushBack(i);
  }
  {
    std::ofstream os(kDir + "slist.bin", std::ios::binary);
    saveBinary(s, os);
  }
  {
    std::ofstream os(kDir + "dlist.bin", std::ios::binary);
    saveBinary(d, os);
  }
  SinglyLinkedList<int> s2;
  DoublyLinkedList<int> d2;
  {
    std::ifstream is(kDir + "slist.bin", std::ios::binary);
    loadBinary(s2, is);
  }
  {
    std::ifstream is(kDir + "dlist.bin", std::ios::binary);
    loadBinary(d2, is);
  }
  ASSERT_EQ(s2.getSize(), 4u);
  ASSERT_EQ(d2.getSize(), 4u);
  for (std::size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(s2.at(i), s.at(i));
    EXPECT_EQ(d2.at(i), d.at(i));
  }
}

TEST(SerializerTest, StackQueueRoundTrip) {
  Stack<int> st;
  Queue<int> q;
  for (int i = 1; i <= 3; ++i) {
    st.push(i);
    q.enqueue(i);
  }
  {
    std::ofstream os(kDir + "stack.bin", std::ios::binary);
    saveBinary(st, os);
  }
  {
    std::ofstream os(kDir + "queue.bin", std::ios::binary);
    saveBinary(q, os);
  }
  Stack<int> st2;
  Queue<int> q2;
  {
    std::ifstream is(kDir + "stack.bin", std::ios::binary);
    loadBinary(st2, is);
  }
  {
    std::ifstream is(kDir + "queue.bin", std::ios::binary);
    loadBinary(q2, is);
  }
  EXPECT_EQ(st2.pop(), 3); // LIFO-порядок
  EXPECT_EQ(st2.pop(), 2);
  EXPECT_EQ(st2.pop(), 1);
  EXPECT_EQ(q2.dequeue(), 1); // FIFO-порядок
  EXPECT_EQ(q2.dequeue(), 2);
  EXPECT_EQ(q2.dequeue(), 3);
}

TEST(SerializerTest, HashTableRoundTrip) {
  HashTable<std::string, int> src;
  src.insert("alice", 20);
  src.insert("bob", 25);
  src.insert("carol", 30);
  {
    std::ofstream os(kDir + "hash.bin", std::ios::binary);
    saveBinary(src, os);
  }
  HashTable<std::string, int> dst;
  {
    std::ifstream is(kDir + "hash.bin", std::ios::binary);
    loadBinary(dst, is);
  }
  EXPECT_EQ(dst.getSize(), 3u);
  EXPECT_EQ(dst.at("alice"), 20);
  EXPECT_EQ(dst.at("bob"), 25);
  EXPECT_EQ(dst.at("carol"), 30);
}

TEST(SerializerTest, TreeRoundTrip) {
  CompleteBinaryTree<int> src;
  for (int i = 1; i <= 7; ++i)
    src.insert(i);
  {
    std::ofstream os(kDir + "tree.bin", std::ios::binary);
    saveBinary(src, os);
  }
  CompleteBinaryTree<int> dst;
  {
    std::ifstream is(kDir + "tree.bin", std::ios::binary);
    loadBinary(dst, is);
  }
  ASSERT_EQ(dst.getSize(), 7u);
  EXPECT_EQ(dst.root(), 1);
  EXPECT_EQ(dst.leftChildOf(0), 2);
  EXPECT_EQ(dst.rightChildOf(2), 7);
  EXPECT_EQ(dst.getHeight(), 3u);
}

TEST(SerializerTest, BadMagicThrows) {
  {
    std::ofstream os(kDir + "garbage.bin", std::ios::binary);
    os << "this is text, not our binary format";
  }
  Array<int> dst;
  std::ifstream is(kDir + "garbage.bin", std::ios::binary);
  EXPECT_THROW(loadBinary(dst, is), std::runtime_error);
}