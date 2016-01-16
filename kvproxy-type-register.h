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
// kvproxy-type-register.h提供用于对KvProxy类型的注册，
// 使用宏REGISTER_KVPROXY_TYPE(type_class, type_name)进行类型注册,
// 注册过后就可以使用了
#ifndef KVPROXY_KVPROXY_TYPE_REGISTER_H_
#define KVPROXY_KVPROXY_TYPE_REGISTER_H_

#include <string>
#include "kvproxy/kvproxy-type.h"

namespace kvproxy {

// 对KvProxy类型进行注册的接口,每一种类型只需要注册一次就可以
// @param type_class为要注册的KvProxy实现，如FileKvProxy
// @param type_name为KvProxy的类型名称,如FileKvProxy类型名称为KVFILE
#define REGISTER_KVPROXY_TYPE(type_class, type_name) \
    KvProxyTypeRegister<type_class> __ ## type_class ## _register(type_name)


template<typename KvProxyClass>
struct KvProxyTypeRegister {
  explicit KvProxyTypeRegister(const std::string& type_name) {
    KvProxyType *type = new KvProxyTypeImpl<KvProxyClass>(type_name);
    KvProxyTypeManager::Instance().Register(type);
  }
};

// 注册所有内建的KvProxy类型
void RegisterAllBuiltinKvProxyTypes();

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_TYPE_REGISTER_H_
