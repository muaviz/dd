#include "object.h"
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>

object blob_from_file(const std::filesystem::path &path) {
  std::string path_str = path.string();
  const char *path_cstr = path_str.c_str();

  FILE *file = fopen(path_cstr, "rb");
  if (file == NULL) {
    throw std::runtime_error("Couldn't open file");
  }
  size_t size = std::filesystem::file_size(path);

  std::vector<uint8_t> buffer;
  buffer.resize(size);

  if (fread(buffer.data(), sizeof(uint8_t), size, file) != size) {
    fclose(file);
    throw std::runtime_error("Under read error");
  }
  fclose(file);

  object obj;
  obj.type = "blob";
  obj.data = buffer;

  return obj;
}
