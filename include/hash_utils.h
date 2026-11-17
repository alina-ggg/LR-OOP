#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <cstddef>
#include <string>

// целочисленный ключ
std::size_t hashInt(int key);
// строковый ключ
std::size_t hashString(const std::string &key);

#endif