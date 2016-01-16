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
#include "kvproxy/kvproxy-adapter.h"
#include <map>
#include <string>
#include <vector>
#include "kvproxy/normal-error.h"

namespace kvproxy {

int KvProxyAdapter::MGet(const std::vector<std::string> &key_vec,
                         std::map<std::string, std::string> * result_map) {
  std::string value;
  uint32_t succ_count = 0;
  for (auto &key : key_vec) {
    int err_code = Get(key, &value);
    if (err_code != KV_EC_OK &&
        err_code != KV_EC_NOT_EXISTS &&
        err_code != KV_EC_EXPIRED) {
      return err_code;
    }
    if (err_code == KV_EC_OK) {
      result_map->insert(make_pair(key, value));
      ++succ_count;
    }
  }

  if (succ_count == key_vec.size()) {
    return KV_EC_OK;
  }
  return KV_EC_PARTIAL_SUCCESS;
}

int KvProxyAdapter::MPut(const std::map<std::string, std::string> & kv_map) {
  uint32_t succ_count = 0;
  for (auto it = kv_map.begin(); it != kv_map.end();  ++it) {
    int err_code = Put(it->first, it->second);
    if (err_code != KV_EC_OK &&
        err_code != KV_EC_NOT_EXISTS &&
        err_code != KV_EC_EXPIRED) {
      return err_code;
    }
    ++succ_count;
  }

  if (succ_count == kv_map.size()) {
    return KV_EC_OK;
  }
  return KV_EC_PARTIAL_SUCCESS;
}

int KvProxyAdapter::MDelete(const std::vector<std::string> & key_vec) {
  uint32_t succ_count = 0;
  for (auto &key : key_vec) {
    int err_code = Delete(key);
    if (err_code != KV_EC_OK &&
        err_code != KV_EC_NOT_EXISTS &&
        err_code != KV_EC_EXPIRED) {
      return err_code;
    }
    ++succ_count;
  }

  if (succ_count == key_vec.size()) {
    return KV_EC_OK;
  }
  return KV_EC_PARTIAL_SUCCESS;
}

const char * KvProxyAdapter::GetError(int err_code) {
  return NormalError::GetNormalError(err_code);
}

int KvProxyAdapter::AddCount(const std::string &key, int64_t count, int64_t *ret_count, int64_t init_value) {
  return KV_EC_NOT_IMPLEMENT;
}

}  // namespace kvproxy
