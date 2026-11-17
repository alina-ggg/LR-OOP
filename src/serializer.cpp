#include "serializer.h"

#include <stdexcept>

namespace {
constexpr std::uint32_t kMagic = 0x4C523353u;
constexpr std::uint32_t kVersion = 1u;
} // namespace

void writeHeader(std::ostream &os) {
  os.write(reinterpret_cast<const char *>(&kMagic), sizeof(kMagic));
  os.write(reinterpret_cast<const char *>(&kVersion), sizeof(kVersion));
}

void readHeader(std::istream &is) {
  std::uint32_t magic = 0;
  std::uint32_t version = 0;
  is.read(reinterpret_cast<char *>(&magic), sizeof(magic));
  is.read(reinterpret_cast<char *>(&version), sizeof(version));
  if (magic != kMagic) {
    throw std::runtime_error(
        "serializer: bad magic (file is not our binary format)");
  }
  if (version != kVersion) {
    throw std::runtime_error("serializer: unsupported format version");
  }
}