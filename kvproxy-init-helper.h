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
#ifndef KVPROXY_KVPROXY_INIT_HELPER_H_
#define KVPROXY_KVPROXY_INIT_HELPER_H_

#include "kvproxy/kvproxy-manager.h"
#include "thirdparty/glog/logging.h"

#define INIT_KVPROXY(var_name, kvproxy_name)\
do {\
  var_name = common::KvProxyManager::Instance().GetKvProxy(kvproxy_name);\
  if (var_name == nullptr) {\
    LOG(FATAL) << "Init KvProxy [" << #var_name << "]  fails, please to check kvproxy_name=" << kvproxy_name << " in kvproxy config files";\
  }\
} while(0)

#endif  // KVPROXY_KVPROXY_INIT_HELPER_H_
