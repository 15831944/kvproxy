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
// file-kvproxy.h提供KVFILE kvproxy type实现,将key/value对存在一个文本文件中
#ifndef KVPROXY_FILE_KVPROXY_H_
#define KVPROXY_FILE_KVPROXY_H_

#include "kvproxy/kvproxy-adapter.h"

namespace kvproxy {

class FileKvProxy: public KvProxyAdapter {
  friend class KvProxyManager;
 public:
  virtual bool Open(const std::map<std::string, std::string> & param_map);

  virtual void Close();

  virtual int Get(const std::string &key, std::string *result);

  virtual int Put(const std::string &key, const std::string &value);

  virtual int Delete(const std::string &key);

 protected:
  bool Load();  // 从文件中加载数据
  void Sync();  // 将修改同步到文件中 

 protected:
  std::string backend_file_;
  std::string kv_separator_; 
  std::map<std::string, std::string> data_map_[2];
  int current_data_map_idx_;
  uint64_t last_modified_time_;  // 文件最后修改时间
  bool is_changed_;  // 标示是否有修改
};

}  // namespace kvproxy

#endif  // KVPROXY_FILE_KVPROXY_H_
