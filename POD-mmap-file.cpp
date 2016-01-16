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
#include "kvproxy/POD-mmap-file.h"
#include <boost/assert.hpp>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "common/utility/utility.h"
#include "thirdparty/glog/logging.h"

namespace common {
namespace kvproxy {

struct FileLockGuard {
  FileLockGuard(int fd) : fd_(fd) {
    flock(fd_, LOCK_EX);
  }

  ~FileLockGuard() {
    flock(fd_, LOCK_UN);
  }
private:
 int fd_;
};

RawMMapFile::Entry *RawMMapFile::Find(const void *key, uint32_t key_size) {
  BOOST_ASSERT(key != NULL && key_size > 0);

  uint32_t key_buff_size = AlignOf(key_size, sizeof(uint64_t));
  void *key_buff = calloc(1, key_buff_size);
  memcpy(key_buff, key, key_size);
  Entry * cur = Next(NULL);
  while (cur->key_size != 0) {
    if ((cur->key_size == key_buff_size) && (memcmp(cur->Key(), key_buff, key_buff_size) == 0)) {
      break;
    }
    cur = Next(cur);
  }
  free(key_buff);
  return cur->key_size == 0 ? NULL : cur;
}

RawMMapFile::Entry *RawMMapFile::Next(Entry *from) {
  BOOST_ASSERT(from < end_);
  if (from == NULL) {
    return static_cast<Entry *>(base_);
  }
  uint8_t * next = reinterpret_cast<uint8_t *>(from) + from->total_size;
  return reinterpret_cast<Entry *>(next);
}

RawMMapFile::Entry *RawMMapFile::AddEntry(const void *key,
                                          uint32_t key_size,
                                          const void *value,
                                          uint32_t value_size) {
  BOOST_ASSERT(key != NULL && key_size > 0 && value != NULL && value_size > 0);
  FileLockGuard lock(fd_);
  Entry *tmp = Find(key, key_size);
  if (tmp != NULL) {
    return tmp;
  }
  Entry * cur = Next(NULL);
  while (cur->key_size != 0) {  // move to end
    cur = Next(cur);
  }
  cur->key_size = AlignOf(key_size, sizeof(uint64_t));
  cur->value_size = AlignOf(value_size, sizeof(uint64_t));
  cur->total_size = cur->key_size  + cur->value_size + sizeof(Entry);
  memcpy(cur->Key(), key, key_size);
  memcpy(cur->Value(), value, value_size);
  return cur;
}

RawMMapFile::RawMMapFile(const char *file_name, uint32_t max_size):
    base_(NULL), end_(NULL), fd_(-1) {
  BOOST_ASSERT(file_name != NULL);
  mode_t pre_mode = umask(0);
  fd_ = open(file_name, O_RDWR | O_CREAT, 0666);
  umask(pre_mode);
  LOG_IF(FATAL, fd_ == -1) << "open file error:" << strerror(errno);
  FileLockGuard lock(fd_);
  struct stat st;
  fstat(fd_, &st);
  if (st.st_size == 0) {
    ftruncate(fd_, max_size);
    st.st_size = max_size;
  }
  BOOST_ASSERT(st.st_size == max_size);
  base_ = mmap(NULL, max_size, PROT_READ | PROT_WRITE , MAP_POPULATE | MAP_SHARED, fd_, 0);
  LOG_IF(FATAL, base_ == MAP_FAILED) << "mmap error:" << strerror(errno);
  end_ = static_cast<uint8_t *>(base_) + max_size;
}

RawMMapFile::~RawMMapFile() {
  munmap(base_, static_cast<uint8_t *>(end_) - static_cast<uint8_t *>(base_));
  base_ = NULL;
  end_ = NULL;
  if (fd_ == -1) {
    close(fd_);
  }
}

}  // namespace kvproxy
}  // namespace common
