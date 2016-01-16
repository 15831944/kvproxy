// Copyright (c) CangKui <cangyingzhijia@126.com>
// All rights reserved.
//
// This library is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this library under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this library under a commercial
// license, as set out in <http://cesanta.com/>.
//
// Author: CangKui
//
// Date: 2012-05-03
//
#ifndef KVPROXY_POD_MMAP_FILE_H_
#define KVPROXY_POD_MMAP_FILE_H_

#include <boost/utility.hpp>
#include <map>
#include <string>

namespace kvproxy {

class RawMMapFile {
 public:
  struct Entry{
    uint32_t total_size;
    uint32_t key_size;
    uint32_t value_size;
    void * Key() {return reinterpret_cast<uint8_t *>(this) + sizeof(Entry);}
    void * Value() {return reinterpret_cast<uint8_t *>(this) + sizeof(Entry) + key_size;}
  };
  RawMMapFile(const char * file_name, uint32_t max_size);
  virtual ~RawMMapFile();
  Entry * AddEntry(const void *key, uint32_t key_size, const void *value, uint32_t value_size);
  Entry * Find(const void *key, uint32_t key_size);
  Entry * Next(Entry *from);

 private:
  void *base_;
  void *end_;
  int fd_;
};

template<typename POD_TYPE, uint32_t MAX_SIZE = 256*1024>
class PODMMapFile : public RawMMapFile{
 public:
  explicit PODMMapFile(const char * file_name) :
      RawMMapFile(file_name, MAX_SIZE) {
      }

  POD_TYPE *Add(const std::string &key, const POD_TYPE &value) {
    Entry * entry = AddEntry(key.c_str(), key.size() + 1, (void *)(&value), sizeof(POD_TYPE));  // NOLINT
    return static_cast<POD_TYPE *>(entry->Value());
  }

  POD_TYPE * Find(const std::string &key) {
    Entry *entry =  Find(key.c_str(), key.size() + 1);
    return static_cast<POD_TYPE *>(entry->Value());
  }

  std::map<std::string, volatile POD_TYPE *> AllElements() {
    Entry * cur = NULL;
    std::map<std::string, volatile POD_TYPE *> ret;
    while ((cur = Next(cur)) != NULL) {
      if (cur->key_size == 0) {
        break;
      }
      std::string key(static_cast<char *>(cur->Key()));
      volatile POD_TYPE * value = const_cast<volatile POD_TYPE *>(static_cast<POD_TYPE *>(cur->Value()));
      ret.insert(std::make_pair(key, value));
    }
    return ret;
  }
};

}  // namespace kvproxy

#endif  // KVPROXY_POD_MMAP_FILE_H_
