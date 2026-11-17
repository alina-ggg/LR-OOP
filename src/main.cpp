#include "array.h"
#include "complete_binary_tree.h"
#include "doubly_linked_list.h"
#include "hash_table.h"
#include "queue.h"
#include "singly_linked_list.h"
#include "stack.h"

#include <iostream>

int main() {
  Array<int> arr;
  arr.pushBack(10);
  arr.pushBack(20);
  arr.insertAt(1, 15);

  for (std::size_t i = 0; i < arr.getSize(); ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << "\nsize=" << arr.getSize() << " capacity=" << arr.getCapacity()
            << "\n";

  arr.removeAt(0);
  std::cout << "after remove: " << arr[0] << ", " << arr[1] << "\n";

  SinglyLinkedList<int> list;
  list.pushBack(1);
  list.pushBack(3);
  list.pushFront(0);
  list.insertAt(2, 2); // 0 1 2 3
  for (std::size_t i = 0; i < list.getSize(); ++i) {
    std::cout << list.at(i) << " ";
  }
  std::cout << "\n";
  list.removeBack();  // 0 1 2
  list.removeFront(); // 1 2
  std::cout << "contains(2): " << std::boolalpha << list.contains(2) << "\n";

  DoublyLinkedList<int> dl;
  dl.pushBack(2);
  dl.pushFront(1);
  dl.pushBack(3);    // 1 2 3
  dl.removeBack();   // 1 2 — O(1)
  dl.insertAt(1, 9); // 1 9 2
  for (std::size_t i = 0; i < dl.getSize(); ++i) {
    std::cout << dl.at(i) << " ";
  }
  std::cout << "\n";

  Stack<int> st;
  st.push(1);
  st.push(2);
  st.push(3);
  std::cout << "stack pop: " << st.pop() << " " << st.pop()
            << "\n"; // LIFO: 3 2

  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  q.enqueue(3);
  std::cout << "queue dequeue: " << q.dequeue() << " " << q.dequeue()
            << "\n";                                 // FIFO: 1 2
  std::cout << "queue front: " << q.front() << "\n"; // 3

  HashTable<std::string, int> ages;
  ages.insert("alice", 20);
  ages.insert("bob", 25);
  ages.insert("alice", 21); // обновление существующего ключа
  std::cout << "alice=" << ages.at("alice") << " bob=" << ages.at("bob")
            << " size=" << ages.getSize() << "\n";
  std::cout << "contains(carol)=" << std::boolalpha << ages.contains("carol")
            << "\n";
  ages.remove("bob");
  std::cout << "after remove size=" << ages.getSize()
            << " buckets=" << ages.getBucketCount() << "\n";

  CompleteBinaryTree<int> tree;
  for (int v = 1; v <= 7; ++v)
    tree.insert(v); // 7 узлов = 3 уровня
  std::cout << "root=" << tree.root() << " left=" << tree.leftChildOf(0)
            << " right=" << tree.rightChildOf(0)
            << " parent(3)=" << tree.parentOf(3) << "\n";
  std::cout << "height=" << tree.getHeight() << " size=" << tree.getSize()
            << " contains(5)=" << std::boolalpha << tree.contains(5) << "\n";
  tree.removeLast();
  std::cout << "after removeLast size=" << tree.getSize()
            << " height=" << tree.getHeight() << "\n";
  return 0;
}