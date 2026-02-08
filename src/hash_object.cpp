#include "blob.h"
#include "hash.h"
#include "object.h"
#include <filesystem>
#include <string>

int hash_object(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  std::filesystem::path path = argv[1];
  object blob = blob_from_file(path);
  std::string hash = write_object(blob);

  const char *hash_cstr = hash.c_str();
  printf("%s\n", hash_cstr);

  read_object(hash);
  return 0;
}
