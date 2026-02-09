#include "index.h"


index load_index() {
    index idx;
    const char* INDEX = ".dd/index";
    FILE *fp = fopen(INDEX, "r");
    //empty index
    if (!fp) {
        return index();
    }
    char buffer[512];
    //read line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        std::string line = buffer;
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }

        size_t space_pos = line.find(' ');
        //  no silent failure  
        if (space_pos == std::string::npos) {
            fclose(fp);
            throw std::runtime_error("Malformed line:no space");
        }
        //spilt at first place
        std::string hash = line.substr(0, space_pos);
        std::string path = line.substr(space_pos + 1);
        // invalid hash length
        if (hash.length() != 40) {
            fclose(fp);
            throw std::runtime_error("Invalid hash length");
        }

        index_entry entry;
        entry.hash = hash;
        entry.path = path;
        //index.entries
        idx.entries.push_back(entry);
    }
    fclose(fp);
    return idx;
}


void write_index(const index &index) {
  const char *path = ".dd/index";
  FILE *index_fp = fopen(path, "w");
  if (index_fp == NULL) {
    throw std::runtime_error("Couldn't open file");
  }

  for (const auto &x : index.entries) {
    if (fprintf(index_fp, "%s %s\n", x.hash.c_str(), x.path.c_str()) < 0) {
      throw std::runtime_error("Write to index failure");
    }
  }

  if (fclose(index_fp) == EOF) {
    throw std::runtime_error("Failed to close file for write");
  }
}

void index_add(index &index, const std::string &path,
               const std:string &hash) {
  for (auto &x : index.entries) {
    if (x.path == path) {
      x.hash = hash;
      return;
    }
  }
  index_entry entry = {path, hash};
  index.entries.push_back(entry);
}
