#include "hash.h"
#include "sha1.hpp"
#include <cstdint>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

std::string hash_bytes(const std::vector<uint8_t> &bytes) {
  SHA1 checksum;

  // converting bytes to a stream for the hash API
  std::string tmp(bytes.begin(), bytes.end());
  std::istringstream stream(tmp, std::ios::binary);

  checksum.update(stream);
  return checksum.final();
}
