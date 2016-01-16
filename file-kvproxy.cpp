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
#include "kvproxy/file-kvproxy.h"
#include <boost/algorithm/string.hpp>
#include <fstream>  // NOLINT
#include <map>
#include <string>
#include "common/file/file-utils.h"
#include "kvproxy/kvproxy-type.h"
#include "kvproxy/normal-error.h"
#include "kvproxy/utils.h"

namespace kvproxy {

using std::ios;

bool FileKvProxy::Open(const std::map<std::string, std::string> & param_map) {
  SET_PARAM_EMPTY_RETURN(backend_file_, "file", param_map, "file parameter is NULL.");
  SET_PARAM_EMPTY_DEFAULT(kv_separator_, "kv_separator", param_map, "=", "kv_separator is null. so use default kv_separator is '='");
  return Load();
}

void FileKvProxy::Close() {
  Sync();
  backend_file_ = "";
  kv_separator_ = "";
  data_map_[current_data_map_idx_].clear();
  current_data_map_idx_ = -1;
  last_modified_time_ = 0;
}

int FileKvProxy::Get(const std::string &key, std::string *result) {
  Load();
  int idx = current_data_map_idx_;
  auto mit = data_map_[idx].find(key);
  int ec = KV_EC_NOT_EXISTS;
  if (mit != data_map_[idx].end()) {
    *result = mit->second;
    ec = KV_EC_OK;
  }
  return ec;
}

int FileKvProxy::Put(const std::string &key, const std::string &value) {
  int idx = current_data_map_idx_;
  data_map_[idx][key] = value;
  is_changed_ = true;
  return KV_EC_OK;
}

int FileKvProxy::Delete(const std::string &key) {
  int idx = current_data_map_idx_;
  auto mit = data_map_[idx].find(key);
  if (mit != data_map_[idx].end()) {
    data_map_[idx].erase(key);
    is_changed_ = true;
  }
  return KV_EC_OK;
}

bool FileKvProxy::Load() {
  if (!common::FileExists(backend_file_)) {
    LOG(INFO) << "file not exists:" << backend_file_;
    return true;
  }
  uint64_t modify_time = 0;
  int ec = common::FileLastModified(backend_file_, &modify_time);
  if (ec != 0) {
    LOG(ERROR) << "find file last modify time fails:" << backend_file_ << ", " << strerror(ec);
    return false;
  }

  if (last_modified_time_ == modify_time) {
    // file is not modified
    return true;
  }
  last_modified_time_ = modify_time;

  std::ifstream ins(backend_file_.c_str());
  if (!ins) {
    LOG(ERROR) << "open backend file error:" << backend_file_;
    return false;
  }

  int map_idx = 1 - current_data_map_idx_;
  Sync();
  data_map_[map_idx].clear();
  auto kv_separator_len = kv_separator_.size();
  std::string line;
  while (std::getline(ins, line)) {
    auto idx = line.find(kv_separator_);
    if (idx != std::string::npos) {
      auto key = line.substr(0, idx);
      boost::trim(key);
      auto value = line.substr(idx + kv_separator_len);
      boost::trim(value);
      data_map_[map_idx].insert(make_pair(key, value));
    }
  }

  current_data_map_idx_ = map_idx;  // switch index
  LOG(INFO) << "load backend file success:" << backend_file_;
  return true;
}

void FileKvProxy::Sync() {
  if (!is_changed_) {
    return;
  }
  std::ofstream os(backend_file_.c_str(), ios::out | ios::binary);
  if (!os) {
    LOG(ERROR) << "open backend_file error:"  << backend_file_.c_str();
    return;
  }

  int map_idx = current_data_map_idx_;
  for (auto& it : data_map_[map_idx]) {
    os << it.first << kv_separator_ << it.second << std::endl;
  }
  is_changed_ = false;
}

}  // namespace kvproxy
