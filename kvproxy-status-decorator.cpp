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
#include "kvproxy/kvproxy-status-decorator.h"
#include <boost/assert.hpp>
#include <map>
#include <string>
#include <vector>
#include "kvproxy/normal-error.h"
#include "kvproxy/POD-mmap-file.h"
#include "common/time/clock.h"
#include "thirdparty/glog/logging.h"

#define STATISTICS(idx, func, cnt)\
    do {\
      volatile Status * pstatus = &(status_[idx]);\
      uint64_t start = common::Clock::NowUS();\
      int rc = func;\
      if ( rc >= KV_EC_NORMAL_COUNT) {\
        rc = KV_EC_ERROR;\
      }\
      pstatus->count[rc] += (cnt);\
      uint64_t end = common::Clock::NowUS();\
      uint64_t len = end - start;\
      uint32_t slot = len/kTimeSlice;\
      if (slot >= kTimeSlot) {\
        slot = kTimeSlot - 1;\
      }\
      pstatus->total_time += len;\
      pstatus->time_slice_count[slot] += 1;\
      return rc;\
    } while (0)

namespace common {
namespace kvproxy {

KvProxyStatusDecorator::KvProxyStatusDecorator(KvProxy *kvproxy,
                                               const std::string &kvproxy_name) :
    kvproxy_(kvproxy),
    kvproxy_name_(kvproxy_name),
    is_ext_kvproxy_(false) {
      if (dynamic_cast<ExtKvProxy *>(kvproxy_) != NULL) {
        is_ext_kvproxy_ = true;
      }
}

KvProxyStatusDecorator::~KvProxyStatusDecorator() {
  if (kvproxy_) {
    delete kvproxy_;
  }
}

int KvProxyStatusDecorator::Delete(const std::string& key) {
  STATISTICS(kStatusDelIdx, kvproxy_->Delete(key), 1);
}

void KvProxyStatusDecorator::Close() {
  kvproxy_->Close();
}

int KvProxyStatusDecorator::Put(const std::string& key,
                                const std::string& value) {
  STATISTICS(kStatusPutIdx, kvproxy_->Put(key, value), 1);
}

int KvProxyStatusDecorator::Get(const std::string& key,
                                std::string* result) {
  STATISTICS(kStatusGetIdx, kvproxy_->Get(key, result), 1);
}

int KvProxyStatusDecorator::MGet(const std::vector<std::string>& key_vec,
                                 std::map<std::string, std::string>* result_map) {
  STATISTICS(kStatusGetIdx, kvproxy_->MGet(key_vec, result_map), key_vec.size());
}
int KvProxyStatusDecorator::MPut(const std::map<std::string, std::string>& kv_map) {
  STATISTICS(kStatusPutIdx, kvproxy_->MPut(kv_map), kv_map.size());
}

int KvProxyStatusDecorator::MDelete(const std::vector<std::string>& key_vec) {
  STATISTICS(kStatusDelIdx, kvproxy_->MDelete(key_vec), key_vec.size());
}

int KvProxyStatusDecorator::AddCount(const std::string& key, int64_t count,
                                 int64_t* ret_count, int64_t init_value) {
  // AddCount计入Get统计里面
  BOOST_ASSERT(is_ext_kvproxy_ && "kvproxy is not a ExtKvProxy and not support add_count function.");
  ExtKvProxy *ext_kvproxy = static_cast<ExtKvProxy * >(kvproxy_);
  STATISTICS(kStatusGetIdx, ext_kvproxy->AddCount(key, count, ret_count, init_value), 1);
}

Status *KvProxyStatusDecorator::ObtainStatus() {
  static StatusArrayPODMMapFile status_map_file(kStatusFile);
  StatusArray *statusArray = status_map_file.Add(kvproxy_name_, StatusArray());
  return statusArray->status;
}

bool KvProxyStatusDecorator::Open(const std::map<std::string, std::string> &config_map) {
  status_ = ObtainStatus();
  if (status_ == NULL) {
    LOG(ERROR) << "ObtainStatus error.";
    return false;
  }
  return kvproxy_->Open(config_map);
}

const char * KvProxyStatusDecorator::GetError(int err_code) {
  return NormalError::GetNormalError(err_code);
}

}  // namespace kvproxy
}  // namespace common
