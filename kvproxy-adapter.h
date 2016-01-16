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
// kvproxy-adapter.h提供默认的muti operators实现，
// 如果没有特殊的实现方式子类不需要关注mutiop的实现，依赖KvProxyAdapter即可
#ifndef KVPROXY_KVPROXY_ADAPTER_H_
#define KVPROXY_KVPROXY_ADAPTER_H_

#include <map>
#include <string>
#include <vector>
#include "kvproxy/ext-kvproxy.h"

namespace kvproxy {

// 使用类的适配器模式,提供默认的MGet() MPut MDelete实现，
// 如果对应的后端存储没有特殊的muti版本的实现，
// 则在实现KvProxy的时候可以从KvProxyAdapter进行继承,减少实现的负担
// 具体使用可参考FileKvProxy的实现
class KvProxyAdapter: public ExtKvProxy {
 public:
  virtual int MGet(const std::vector<std::string>& key_vec, 
                   std::map<std::string, std::string>* result_map);

  virtual int MPut(const std::map<std::string, std::string>& kv_map);

  virtual int MDelete(const std::vector<std::string>& key_vec);

  virtual const char * GetError(int err_code);

  virtual int AddCount(const std::string &key, int64_t count, int64_t *ret_count,
                         int64_t init_value = 0);
};

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_ADAPTER_H_
