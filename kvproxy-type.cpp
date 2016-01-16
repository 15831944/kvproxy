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
#include "kvproxy/kvproxy-type.h"
#include <string>
#include "thirdparty/glog/logging.h"

namespace kvproxy {

KvProxyType::~KvProxyType() {
}

KvProxyTypeManager::~KvProxyTypeManager() {
  for (KvProxyType* type : type_list_) {
    delete type;
  }
  type_list_.clear();
}

KvProxy * KvProxyTypeManager::NewKvProxy(
    const std::string &type_name) {
  KvProxyType* type = Find(type_name);
  if (type == NULL) {
    return NULL;
  }
  return type->NewKvProxy();
}

void KvProxyTypeManager::Register(KvProxyType *new_type) {
  KvProxyType* type = Find(new_type->type_name());
  if (type != NULL) {  // 已经存在则删除
    LOG(WARNING) << "KvProxy Type [" << type->type_name() << "] aready exists.";
    delete new_type;
    return;
  }
  type_list_.push_front(new_type);
}

KvProxyType * KvProxyTypeManager::Find(
    const std::string& type_name) {
  for (KvProxyType* type : type_list_) {
    if (type->type_name() == type_name) {
      return type;
    }
  }
  return NULL;
}

}  // namespace kvproxy
