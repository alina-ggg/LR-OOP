#include "serializer.h"
#include <stdexcept>
#include <sstream>
#include <vector>

void Serializer::writeHeader(std::ostream& os, const std::string& tag, std::size_t size) {
    os.write(reinterpret_cast<const char*>(&kMagic), sizeof(kMagic));
    os.write(reinterpret_cast<const char*>(&kVersion), sizeof(kVersion));
    std::uint32_t tagLen = static_cast<std::uint32_t>(tag.size());
    os.write(reinterpret_cast<const char*>(&tagLen), sizeof(tagLen));
    os.write(tag.data(), tagLen);
    std::uint64_t size64 = static_cast<std::uint64_t>(size);
    os.write(reinterpret_cast<const char*>(&size64), sizeof(size64));
}

std::size_t Serializer::readHeader(std::istream& is, const std::string& expectedTag) {
    std::uint32_t magic;
    is.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (magic != kMagic) {
        throw std::runtime_error("Bad magic number");
    }
    std::uint32_t version;
    is.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != kVersion) {
        throw std::runtime_error("Bad version");
    }
    std::uint32_t tagLen;
    is.read(reinterpret_cast<char*>(&tagLen), sizeof(tagLen));
    std::string tag(tagLen, '\0');
    is.read(&tag[0], tagLen);
    if (tag != expectedTag) {
        throw std::runtime_error("Bad tag: expected " + expectedTag + ", got " + tag);
    }
    std::uint64_t size64;
    is.read(reinterpret_cast<char*>(&size64), sizeof(size64));
    return static_cast<std::size_t>(size64);
}

void Serializer::writeString(std::ostream& os, const std::string& str) {
    std::uint32_t len = static_cast<std::uint32_t>(str.size());
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(str.data(), len);
}

std::string Serializer::readString(std::istream& is) {
    std::uint32_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::string str(len, '\0');
    is.read(&str[0], len);
    return str;
}

void Serializer::writeTextHeader(std::ostream& os, const std::string& tag, std::size_t size) {
    os << tag << " " << size << "\n";
}

std::size_t Serializer::readTextHeader(std::istream& is, const std::string& expectedTag) {
    std::string line;
    if (!std::getline(is, line)) {
        throw std::runtime_error("EOF while reading text header");
    }
    std::istringstream iss(line);
    std::string tag;
    std::size_t size;
    if (!(iss >> tag >> size) || tag != expectedTag) {
        throw std::runtime_error("Bad text header: expected " + expectedTag);
    }
    return size;
}

std::string Serializer::quoteString(const std::string& s) {
    std::string res = "\"";
    for (char c : s) {
        if (c == '"')
            res += "\\\"";
        else if (c == '\\')
            res += "\\\\";
        else if (c == '\n')
            res += "\\n";
        else if (c == '\t')
            res += "\\t";
        else
            res += c;
    }
    return res + "\"";
}

std::string Serializer::unquoteString(const std::string& s) {
    if (s.size() < 2 || s.front() != '"' || s.back() != '"')
        return s;
    std::string res;
    for (std::size_t i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == '\\' && i + 2 < s.size()) {
            char next = s[++i];
            if (next == 'n')
                res += '\n';
            else if (next == 't')
                res += '\t';
            else if (next == '"')
                res += '"';
            else if (next == '\\')
                res += '\\';
            else
                res += next;
        } else {
            res += s[i];
        }
    }
    return res;
}

void Serializer::saveArrayInt(const Array<int>& arr, std::ostream& os) {
    writeHeader(os, "ARRAY_INT", arr.getSize());
    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        int val = arr.at(i);
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Serializer::loadArrayInt(Array<int>& arr, std::istream& is) {
    std::size_t size = readHeader(is, "ARRAY_INT");
    arr.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        arr.pushBack(val);
    }
}

void Serializer::saveArrayString(const Array<std::string>& arr, std::ostream& os) {
    writeHeader(os, "ARRAY_STR", arr.getSize());
    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        writeString(os, arr.at(i));
    }
}

void Serializer::loadArrayString(Array<std::string>& arr, std::istream& is) {
    std::size_t size = readHeader(is, "ARRAY_STR");
    arr.clear();
    for (std::size_t i = 0; i < size; ++i) {
        arr.pushBack(readString(is));
    }
}

void Serializer::saveArrayIntText(const Array<int>& arr, std::ostream& os) {
    writeTextHeader(os, "ARRAY_INT", arr.getSize());
    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        os << arr.at(i) << "\n";
    }
}

void Serializer::loadArrayIntText(Array<int>& arr, std::istream& is) {
    std::size_t size = readTextHeader(is, "ARRAY_INT");
    arr.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        arr.pushBack(std::stoi(line));
    }
}

void Serializer::saveArrayStringText(const Array<std::string>& arr, std::ostream& os) {
    writeTextHeader(os, "ARRAY_STR", arr.getSize());
    for (std::size_t i = 0; i < arr.getSize(); ++i) {
        os << quoteString(arr.at(i)) << "\n";
    }
}

void Serializer::loadArrayStringText(Array<std::string>& arr, std::istream& is) {
    std::size_t size = readTextHeader(is, "ARRAY_STR");
    arr.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        arr.pushBack(unquoteString(line));
    }
}

void Serializer::saveSinglyInt(const SinglyLinkedList<int>& list, std::ostream& os) {
    writeHeader(os, "SINGLY_INT", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        int val = list.at(i);
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Serializer::loadSinglyInt(SinglyLinkedList<int>& list, std::istream& is) {
    std::size_t size = readHeader(is, "SINGLY_INT");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        list.pushBack(val);
    }
}

void Serializer::saveSinglyString(const SinglyLinkedList<std::string>& list, std::ostream& os) {
    writeHeader(os, "SINGLY_STR", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        writeString(os, list.at(i));
    }
}

void Serializer::loadSinglyString(SinglyLinkedList<std::string>& list, std::istream& is) {
    std::size_t size = readHeader(is, "SINGLY_STR");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        list.pushBack(readString(is));
    }
}

void Serializer::saveSinglyIntText(const SinglyLinkedList<int>& list, std::ostream& os) {
    writeTextHeader(os, "SINGLY_INT", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        os << list.at(i) << "\n";
    }
}

void Serializer::loadSinglyIntText(SinglyLinkedList<int>& list, std::istream& is) {
    std::size_t size = readTextHeader(is, "SINGLY_INT");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        list.pushBack(std::stoi(line));
    }
}

void Serializer::saveSinglyStringText(const SinglyLinkedList<std::string>& list, std::ostream& os) {
    writeTextHeader(os, "SINGLY_STR", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        os << quoteString(list.at(i)) << "\n";
    }
}

void Serializer::loadSinglyStringText(SinglyLinkedList<std::string>& list, std::istream& is) {
    std::size_t size = readTextHeader(is, "SINGLY_STR");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        list.pushBack(unquoteString(line));
    }
}

void Serializer::saveDoublyInt(const DoublyLinkedList<int>& list, std::ostream& os) {
    writeHeader(os, "DOUBLY_INT", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        int val = list.at(i);
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Serializer::loadDoublyInt(DoublyLinkedList<int>& list, std::istream& is) {
    std::size_t size = readHeader(is, "DOUBLY_INT");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        list.pushBack(val);
    }
}

void Serializer::saveDoublyString(const DoublyLinkedList<std::string>& list, std::ostream& os) {
    writeHeader(os, "DOUBLY_STR", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        writeString(os, list.at(i));
    }
}

void Serializer::loadDoublyString(DoublyLinkedList<std::string>& list, std::istream& is) {
    std::size_t size = readHeader(is, "DOUBLY_STR");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        list.pushBack(readString(is));
    }
}
void Serializer::saveDoublyIntText(const DoublyLinkedList<int>& list, std::ostream& os) {
    writeTextHeader(os, "DOUBLY_INT", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        os << list.at(i) << "\n";
    }
}

void Serializer::loadDoublyIntText(DoublyLinkedList<int>& list, std::istream& is) {
    std::size_t size = readTextHeader(is, "DOUBLY_INT");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        list.pushBack(std::stoi(line));
    }
}

void Serializer::saveDoublyStringText(const DoublyLinkedList<std::string>& list, std::ostream& os) {
    writeTextHeader(os, "DOUBLY_STR", list.getSize());
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        os << quoteString(list.at(i)) << "\n";
    }
}

void Serializer::loadDoublyStringText(DoublyLinkedList<std::string>& list, std::istream& is) {
    std::size_t size = readTextHeader(is, "DOUBLY_STR");
    list.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        list.pushBack(unquoteString(line));
    }
}

void Serializer::saveStackInt(const Stack<int>& stack, std::ostream& os) {
    std::size_t size = stack.getSize();
    writeHeader(os, "STACK_INT", size);

    Stack<int> temp = stack;
    std::vector<int> elems;
    while (!temp.isEmpty()) {
        elems.push_back(temp.pop());
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        os.write(reinterpret_cast<const char*>(&(*it)), sizeof(*it));
    }
}

void Serializer::loadStackInt(Stack<int>& stack, std::istream& is) {
    std::size_t size = readHeader(is, "STACK_INT");
    stack.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        stack.push(val);
    }
}

void Serializer::saveStackString(const Stack<std::string>& stack, std::ostream& os) {
    std::size_t size = stack.getSize();
    writeHeader(os, "STACK_STR", size);

    Stack<std::string> temp = stack;
    std::vector<std::string> elems;
    while (!temp.isEmpty()) {
        elems.push_back(temp.pop());
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        writeString(os, *it);
    }
}

void Serializer::loadStackString(Stack<std::string>& stack, std::istream& is) {
    std::size_t size = readHeader(is, "STACK_STR");
    stack.clear();
    for (std::size_t i = 0; i < size; ++i) {
        stack.push(readString(is));
    }
}

void Serializer::saveStackIntText(const Stack<int>& stack, std::ostream& os) {
    std::size_t size = stack.getSize();
    writeTextHeader(os, "STACK_INT", size);
    Stack<int> temp = stack;
    std::vector<int> elems;
    while (!temp.isEmpty()) {
        elems.push_back(temp.pop());
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        os << *it << "\n";
    }
}

void Serializer::loadStackIntText(Stack<int>& stack, std::istream& is) {
    std::size_t size = readTextHeader(is, "STACK_INT");
    stack.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        stack.push(std::stoi(line));
    }
}

void Serializer::saveStackStringText(const Stack<std::string>& stack, std::ostream& os) {
    std::size_t size = stack.getSize();
    writeTextHeader(os, "STACK_STR", size);
    Stack<std::string> temp = stack;
    std::vector<std::string> elems;
    while (!temp.isEmpty()) {
        elems.push_back(temp.pop());
    }
    for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
        os << quoteString(*it) << "\n";
    }
}

void Serializer::loadStackStringText(Stack<std::string>& stack, std::istream& is) {
    std::size_t size = readTextHeader(is, "STACK_STR");
    stack.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        stack.push(unquoteString(line));
    }
}

void Serializer::saveQueueInt(const Queue<int>& queue, std::ostream& os) {
    std::size_t size = queue.getSize();
    writeHeader(os, "QUEUE_INT", size);

    Queue<int> temp = queue;
    while (!temp.isEmpty()) {
        int val = temp.dequeue();
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Serializer::loadQueueInt(Queue<int>& queue, std::istream& is) {
    std::size_t size = readHeader(is, "QUEUE_INT");
    queue.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        queue.enqueue(val);
    }
}

void Serializer::saveQueueString(const Queue<std::string>& queue, std::ostream& os) {
    std::size_t size = queue.getSize();
    writeHeader(os, "QUEUE_STR", size);

    Queue<std::string> temp = queue;
    while (!temp.isEmpty()) {
        writeString(os, temp.dequeue());
    }
}

void Serializer::loadQueueString(Queue<std::string>& queue, std::istream& is) {
    std::size_t size = readHeader(is, "QUEUE_STR");
    queue.clear();
    for (std::size_t i = 0; i < size; ++i) {
        queue.enqueue(readString(is));
    }
}

void Serializer::saveQueueIntText(const Queue<int>& queue, std::ostream& os) {
    std::size_t size = queue.getSize();
    writeTextHeader(os, "QUEUE_INT", size);
    Queue<int> temp = queue;
    while (!temp.isEmpty()) {
        os << temp.dequeue() << "\n";
    }
}

void Serializer::loadQueueIntText(Queue<int>& queue, std::istream& is) {
    std::size_t size = readTextHeader(is, "QUEUE_INT");
    queue.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        queue.enqueue(std::stoi(line));
    }
}

void Serializer::saveQueueStringText(const Queue<std::string>& queue, std::ostream& os) {
    std::size_t size = queue.getSize();
    writeTextHeader(os, "QUEUE_STR", size);
    Queue<std::string> temp = queue;
    while (!temp.isEmpty()) {
        os << quoteString(temp.dequeue()) << "\n";
    }
}

void Serializer::loadQueueStringText(Queue<std::string>& queue, std::istream& is) {
    std::size_t size = readTextHeader(is, "QUEUE_STR");
    queue.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        queue.enqueue(unquoteString(line));
    }
}

void Serializer::saveHashIntInt(const HashTable<int, int>& table, std::ostream& os) {
    std::vector<std::pair<int, int>> pairs;
    table.forEach([&pairs](int key, int value) { pairs.push_back({key, value}); });

    writeHeader(os, "HASH_INT_INT", pairs.size());

    for (const auto& p : pairs) {
        os.write(reinterpret_cast<const char*>(&p.first), sizeof(p.first));
        os.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));
    }
}

void Serializer::loadHashIntInt(HashTable<int, int>& table, std::istream& is) {
    std::size_t count = readHeader(is, "HASH_INT_INT");
    table.clear();
    for (std::size_t i = 0; i < count; ++i) {
        int key, value;
        is.read(reinterpret_cast<char*>(&key), sizeof(key));
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
        table.insert(key, value);
    }
}

void Serializer::saveHashStringInt(const HashTable<std::string, int>& table, std::ostream& os) {
    std::vector<std::pair<std::string, int>> pairs;
    table.forEach([&pairs](const std::string& key, int value) { pairs.push_back({key, value}); });

    writeHeader(os, "HASH_STR_INT", pairs.size());

    for (const auto& p : pairs) {
        writeString(os, p.first);
        os.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));
    }
}

void Serializer::loadHashStringInt(HashTable<std::string, int>& table, std::istream& is) {
    std::size_t count = readHeader(is, "HASH_STR_INT");
    table.clear();
    for (std::size_t i = 0; i < count; ++i) {
        std::string key = readString(is);
        int value;
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
        table.insert(key, value);
    }
}

void Serializer::saveHashIntIntText(const HashTable<int, int>& table, std::ostream& os) {
    std::vector<std::pair<int, int>> pairs;
    table.forEach([&pairs](int key, int value) { pairs.push_back({key, value}); });
    writeTextHeader(os, "HASH_INT_INT", pairs.size());
    for (const auto& p : pairs) {
        os << p.first << "\t" << p.second << "\n";
    }
}

void Serializer::loadHashIntIntText(HashTable<int, int>& table, std::istream& is) {
    std::size_t size = readTextHeader(is, "HASH_INT_INT");
    table.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        std::size_t tabPos = line.find('\t');
        if (tabPos == std::string::npos) {
            throw std::runtime_error("Bad hash line: " + line);
        }
        int key = std::stoi(line.substr(0, tabPos));
        int value = std::stoi(line.substr(tabPos + 1));
        table.insert(key, value);
    }
}

void Serializer::saveHashStringIntText(const HashTable<std::string, int>& table, std::ostream& os) {
    std::vector<std::pair<std::string, int>> pairs;
    table.forEach([&pairs](const std::string& key, int value) { pairs.push_back({key, value}); });
    writeTextHeader(os, "HASH_STR_INT", pairs.size());
    for (const auto& p : pairs) {
        os << quoteString(p.first) << "\t" << p.second << "\n";
    }
}

void Serializer::loadHashStringIntText(HashTable<std::string, int>& table, std::istream& is) {
    std::size_t size = readTextHeader(is, "HASH_STR_INT");
    table.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        std::size_t tabPos = line.rfind('\t');
        if (tabPos == std::string::npos) {
            throw std::runtime_error("Bad hash line: " + line);
        }
        std::string keyStr = line.substr(0, tabPos);
        std::string valStr = line.substr(tabPos + 1);
        table.insert(unquoteString(keyStr), std::stoi(valStr));
    }
}

void Serializer::saveTreeInt(const CompleteBinaryTree<int>& tree, std::ostream& os) {
    writeHeader(os, "TREE_INT", tree.getSize());
    for (std::size_t i = 0; i < tree.getSize(); ++i) {
        int val = tree.at(i);
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Serializer::loadTreeInt(CompleteBinaryTree<int>& tree, std::istream& is) {
    std::size_t size = readHeader(is, "TREE_INT");
    tree.clear();
    for (std::size_t i = 0; i < size; ++i) {
        int val;
        is.read(reinterpret_cast<char*>(&val), sizeof(val));
        tree.insert(val);
    }
}

void Serializer::saveTreeIntText(const CompleteBinaryTree<int>& tree, std::ostream& os) {
    writeTextHeader(os, "TREE_INT", tree.getSize());
    for (std::size_t i = 0; i < tree.getSize(); ++i) {
        os << tree.at(i) << "\n";
    }
}

void Serializer::loadTreeIntText(CompleteBinaryTree<int>& tree, std::istream& is) {
    std::size_t size = readTextHeader(is, "TREE_INT");
    tree.clear();
    for (std::size_t i = 0; i < size; ++i) {
        std::string line;
        std::getline(is, line);
        tree.insert(std::stoi(line));
    }
}