#include "hash_utils.h"

std::size_t hashInt(int key) {
  // 2654435761 ~ 2^32 / phi: умножение "размазывает" биты равномерно
  return static_cast<std::size_t>(static_cast<unsigned int>(key)) * 2654435761u;
}

std::size_t hashString(const std::string &key) {
  // FNV-1a: offset basis, затем для каждого байта xor -> multiply
  std::size_t h = 14695981039346656037ull;
  for (unsigned char c : key) {
    h ^= c;
    h *= 1099511628211ull;
  }
  return h;
}