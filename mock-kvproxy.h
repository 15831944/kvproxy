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
// Date: 2012-05-04
// 
#ifndef KVPROXY_MOCK_KVPROXY_H_
#define KVPROXY_MOCK_KVPROXY_H_

#include <map>
#include <string>

#include "kvproxy/kvproxy-adapter.h"
#include "common/utility/lexical-cast.h"

namespace kvproxy {

class MockKvProxy: public KvProxyAdapter {
 public:
  virtual bool Open(const std::map<std::string, std::string>& config_map) {
    error_code_ = KV_EC_OK;
    return true;
  }

  virtual void Close() {}

  virtual int Get(const std::string& key, std::string* result) {
    auto mit = data_map_.find(key);    
    if (mit != data_map_.end()) {
      *result = mit->second;
    }
    return error_code_;
  }

  virtual int Put(const std::string &key, const std::string &value) {
    data_map_[key] = value;
    return error_code_;
  }

  virtual int Delete(const std::string &key) {
    data_map_.erase(key);
    return error_code_;
  }

  virtual int AddCount(const std::string &key, int64_t count, int64_t *ret_count,
                       int64_t init_value = 0) {
    auto mit = data_map_.find(key);
    if (mit != data_map_.end()) {
      count += lexical_cast<int64_t>(mit->second);
    } else {
      count += init_value;
    }
    *ret_count = count;
    return Put(key, lexical_cast<std::string>(*ret_count));
  }

  /*
   * mock 返回结果
   */
  virtual void SetErrorCode(int error_code) {
    error_code_ = error_code;
  }

protected:
  int error_code_;
  std::map<std::string, std::string> data_map_;
};

}  // namespace kvproxy

#endif  // KVPROXY_MOCK_KVPROXY_H_
