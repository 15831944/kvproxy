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
// ext-kvproxy.h: kvproxy的扩展接口
#ifndef KVPROXY_EXT_KVPROXY_H_
#define KVPROXY_EXT_KVPROXY_H_

#include <string>
#include "kvproxy/kvproxy.h"

namespace kvproxy {

class ExtKvProxy: public KvProxy {
 public:
  /**
   * add count
   * @param key key
   * @param count add (count) to original value.
   * @param ret_count the latest value
   * @param init_value initialize value
   */
  virtual int AddCount(const std::string &key, int64_t count, int64_t *ret_count,
                         int64_t init_value = 0) = 0;
};

}  // namespace kvproxy

#endif  // KVPROXY_EXT_KVPROXY_H_
