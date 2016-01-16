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
// normal-error.h定义一些常见的错误类型,
// 如返回成功为KV_EC_OK=0, 查找的数据不存在EC_NOT_EXISTS=1,
// 数据过期EC_EXPIRED=2等
//
#ifndef KVPROXY_NORMAL_ERROR_H_
#define KVPROXY_NORMAL_ERROR_H_

namespace common {
namespace kvproxy {

// 通用错误定义
#define  KV_EC_OK  0              // ok
#define  KV_EC_NOT_EXISTS  1      // not exists
#define  KV_EC_EXPIRED  2         // data expired
#define  KV_EC_TIMEOUT  3         // access timeout
#define  KV_EC_NOT_IMPLEMENT  4   // method not implement
#define  KV_EC_PARTIAL_SUCCESS 5  // partial success
#define  KV_EC_ERROR 6            // unknow error
#define  KV_EC_NORMAL_COUNT 7     // normal error count

// 通用类型错误信息，err_code取值只可能为上面的
// 通用错误定义定义过的error code,
// 通过调用GetNormalError()方法返回错误描述
// 例如：
// KvProxy * proxy;
// ....
// int ec = proxy->Get(key, value);
// const char *err_msg = NormalError::GetNormalError(ec);
class NormalError {
 public:
  static const char* GetNormalError(int err_code);
};

}  // namespace kvproxy 
}  // namespace common 

#endif  // KVPROXY_NORMAL_ERROR_H_
