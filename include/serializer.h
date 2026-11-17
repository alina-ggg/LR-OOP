#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

#include "array.h"
#include "complete_binary_tree.h"
#include "doubly_linked_list.h"
#include "hash_table.h"
#include "queue.h"
#include "singly_linked_list.h"
#include "stack.h"

// заголовок бинарного файла
void writeHeader(std::ostream &os);
void readHeader(std::istream &is);

// скаляры и строки
inline void saveValue(std::ostream &os, int v) {
  os.write(reinterpret_cast<const char *>(&v), sizeof(v));
}
inline void loadValue(std::istream &is, int &v) {
  is.read(reinterpret_cast<char *>(&v), sizeof(v));
}
inline void saveValue(std::ostream &os, const std::string &s) {
  std::uint64_t len = s.size();
  os.write(reinterpret_cast<const char *>(&len), sizeof(len));
  os.write(s.data(), static_cast<std::streamsize>(len));
}
inline void loadValue(std::istream &is, std::string &s) {
  std::uint64_t len = 0;
  is.read(reinterpret_cast<char *>(&len), sizeof(len));
  s.assign(len, '\0');
  is.read(&s[0], static_cast<std::streamsize>(len));
}
inline void saveSize(std::ostream &os, std::size_t n) {
  std::uint64_t v = n;
  os.write(reinterpret_cast<const char *>(&v), sizeof(v));
}
inline std::size_t loadSize(std::istream &is) {
  std::uint64_t v = 0;
  is.read(reinterpret_cast<char *>(&v), sizeof(v));
  return static_cast<std::size_t>(v);
}

// контейнеры
template <typename T> void saveBinary(const Array<T> &a, std::ostream &os) {
  writeHeader(os);
  saveSize(os, a.getSize());
  for (std::size_t i = 0; i < a.getSize(); ++i)
    saveValue(os, a[i]);
}
template <typename T> void loadBinary(Array<T> &a, std::istream &is) {
  readHeader(is);
  a.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    a.pushBack(v);
  }
}

template <typename T>
void saveBinary(const SinglyLinkedList<T> &l, std::ostream &os) {
  writeHeader(os);
  saveSize(os, l.getSize());
  for (std::size_t i = 0; i < l.getSize(); ++i)
    saveValue(os, l.at(i));
}
template <typename T>
void loadBinary(SinglyLinkedList<T> &l, std::istream &is) {
  readHeader(is);
  l.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    l.pushBack(v);
  }
}

template <typename T>
void saveBinary(const DoublyLinkedList<T> &l, std::ostream &os) {
  writeHeader(os);
  saveSize(os, l.getSize());
  for (std::size_t i = 0; i < l.getSize(); ++i)
    saveValue(os, l.at(i));
}
template <typename T>
void loadBinary(DoublyLinkedList<T> &l, std::istream &is) {
  readHeader(is);
  l.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    l.pushBack(v);
  }
}

template <typename T> void saveBinary(const Stack<T> &st, std::ostream &os) {
  writeHeader(os);
  saveSize(os, st.getSize());
  Stack<T> tmp(st);
  Array<T> reversed;
  while (!tmp.isEmpty())
    reversed.pushBack(tmp.pop());
  for (std::size_t i = reversed.getSize(); i-- > 0;) {
    saveValue(os, reversed[i]); // ...и пишем снизу вверх
  }
}
template <typename T> void loadBinary(Stack<T> &st, std::istream &is) {
  readHeader(is);
  st.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    st.push(v);
  }
}

template <typename T> void saveBinary(const Queue<T> &q, std::ostream &os) {
  writeHeader(os);
  saveSize(os, q.getSize());
  Queue<T> tmp(q);
  while (!tmp.isEmpty())
    saveValue(os, tmp.dequeue()); // FIFO-порядок
}
template <typename T> void loadBinary(Queue<T> &q, std::istream &is) {
  readHeader(is);
  q.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    q.enqueue(v);
  }
}

template <typename K, typename V>
void saveBinary(const HashTable<K, V> &t, std::ostream &os) {
  writeHeader(os);
  saveSize(os, t.getSize());
  t.forEach([&](const K &k, const V &v) {
    saveValue(os, k);
    saveValue(os, v);
  });
}
template <typename K, typename V>
void loadBinary(HashTable<K, V> &t, std::istream &is) {
  readHeader(is);
  t.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    K k{};
    V v{};
    loadValue(is, k);
    loadValue(is, v);
    t.insert(k, v);
  }
}

template <typename T>
void saveBinary(const CompleteBinaryTree<T> &t, std::ostream &os) {
  writeHeader(os);
  saveSize(os, t.getSize());
  for (std::size_t i = 0; i < t.getSize(); ++i)
    saveValue(os, t.at(i));
}
template <typename T>
void loadBinary(CompleteBinaryTree<T> &t, std::istream &is) {
  readHeader(is);
  t.clear();
  std::size_t n = loadSize(is);
  for (std::size_t i = 0; i < n; ++i) {
    T v{};
    loadValue(is, v);
    t.insert(v);
  }
}

#endif