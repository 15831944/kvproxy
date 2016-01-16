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
#ifndef KVPROXY_KVPROXY_STATUS_DECORATOR_H_
#define KVPROXY_KVPROXY_STATUS_DECORATOR_H_

#include <map>
#include <string>
#include <vector>
#include "kvproxy/ext-kvproxy.h"
#include "kvproxy/POD-mmap-file.h"

namespace kvproxy {

const uint64_t kTimeSlice = 500;  // 500us
const uint32_t kTimeSlot = 10;
const char *const kStatusFile = "/tmp/kvproxy.status.dat";

enum StatusIdx {
  kStatusGetIdx = 0,
  kStatusPutIdx = 1,
  kStatusDelIdx = 2,
  kStatusDelCount,
};

struct Status {
  uint64_t count[KV_EC_NORMAL_COUNT];
  uint64_t total_time;
  uint64_t time_slice_count[kTimeSlot];  // 0.5ms、1.0ms、1.5ms ... 5.0ms
};

struct StatusArray {
  Status status[kStatusDelCount];
};

typedef PODMMapFile<StatusArray> StatusArrayPODMMapFile;

class KvProxyStatusDecorator: public ExtKvProxy {
  friend class KvProxyManager;
 public:

  virtual bool Open(const std::map<std::string, std::string> &config_map);

  virtual void Close();

  virtual int Get(const std::string& key, std::string* result) override;

  virtual int Put(const std::string &key, const std::string &value) override;

  virtual int Delete(const std::string& key) override;

  virtual int MGet(const std::vector<std::string>& key_vec, std::map<std::string, std::string>* result_map) override;

  virtual int MPut(const std::map<std::string, std::string>& kv_map) override;

  virtual int MDelete(const std::vector<std::string>& key_vec) override;

  virtual int AddCount(const std::string &key, int64_t count, int64_t *ret_count, int64_t init_value = 0) override;

  const char *GetError(int err_code);

 protected:
  KvProxyStatusDecorator(KvProxy* kvproxy, const std::string& kvproxy_name);
  virtual ~KvProxyStatusDecorator();

 private:
  Status *ObtainStatus();

 private:
  KvProxy *kvproxy_;
  volatile Status *status_;
  std::string kvproxy_name_;
  bool is_ext_kvproxy_;
};

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_STATUS_DECORATOR_H_
