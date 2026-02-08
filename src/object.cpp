#include "object.h"
#include "hash.h"
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<uint8_t> serialise_obj(const object &obj) {
  std::vector<uint8_t> buffer;
  buffer.reserve(obj.type.size() + 1 + obj.data.size());
  for (const auto &x : obj.type) {
    buffer.push_back((uint8_t)x);
  }
  buffer.push_back('\0');
  for (const auto &x : obj.data) {
    buffer.push_back(x);
  }

  return buffer;
}

std::filesystem::path object_path_from_hash(const std::string &hash) {
  std::string dir_name = hash.substr(0, 2);
  std::string file_name = hash.substr(2, hash.size() - 2);

  std::filesystem::path objs_path = ".dd/objects/";
  objs_path /= dir_name;
  objs_path /= file_name;

  return objs_path;
}

std::string write_object(const object &obj) {
  std::filesystem::path DD_DIR = ".dd/";
  if (!std::filesystem::exists(DD_DIR)) {
    throw std::runtime_error("Not a dd repository");
  }
  std::vector<uint8_t> to_hash = serialise_obj(obj);
  std::string hash = hash_bytes(to_hash);
  std::filesystem::path obj_path = object_path_from_hash(hash);
  std::filesystem::path p_path = obj_path.parent_path();

  if (!std::filesystem::exists(p_path)) {
    if (!std::filesystem::create_directories(p_path)) {
      throw std::runtime_error("Couldn't create directories");
    }
  }

  if (!std::filesystem::exists(obj_path)) {
    std::string obj_path_str = obj_path.string();
    const char *obj_path_cstr = obj_path_str.c_str();
    FILE *OBJ = fopen(obj_path_cstr, "wb");
    if (OBJ == NULL) {
      throw std::runtime_error("Couldn't open file");
    }

    fwrite(to_hash.data(), 1, to_hash.size(), OBJ);
    fclose(OBJ);
  }

  return hash;
}

object read_object(const std::string &hash) {
  object obj;
  std::filesystem::path obj_path = object_path_from_hash(hash);

  std::string obj_path_str = obj_path.string();
  const char *obj_path_cstr = obj_path_str.c_str();

  FILE *OBJ = fopen(obj_path_cstr, "rb");
  if (OBJ == NULL) {
    throw std::runtime_error("Couldn't open file");
  }
  size_t size = std::filesystem::file_size(obj_path);

  std::vector<uint8_t> buffer;
  buffer.resize(size);

  if (fread(buffer.data(), sizeof(uint8_t), size, OBJ) != size) {
    fclose(OBJ);
    throw std::runtime_error("Under read error");
  }
  fclose(OBJ);

  int null_index = -1;
  for (size_t i = 0; i < buffer.size(); i++) {
    if (buffer[i] == '\0') {
      null_index = (int)i;
      break;
    }
  }
  if (null_index == -1) {
    throw std::runtime_error("Invalid buffer");
  }

  std::vector<uint8_t> type(buffer.begin(), buffer.begin() + null_index);
  std::vector<uint8_t> data(buffer.begin() + null_index + 1, buffer.end());
  std::string type_s(type.begin(), type.end());

  obj.type = type_s;
  obj.data = data;

  std::vector<uint8_t> to_hash = serialise_obj(obj);
  std::string re_hash = hash_bytes(to_hash);

  if (re_hash != hash) {
    throw std::runtime_error("Object integrity check failed");
  }

  return obj;
}
