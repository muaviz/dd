#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

typedef struct {
  std::string type;
  std::vector<uint8_t> data;
} object;

std::vector<uint8_t> serialise_obj(const object &obj);
std::filesystem::path object_path_from_hash(const std::string &hash);
std::string write_object(const object &obj);
object read_object(const std::string &hash);
