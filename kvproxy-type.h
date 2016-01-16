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
// kvproxy-type.h提供用于对KvProxy类型的描述KvProxyType，
// 及对KvProxy类型进行管理的功能KvProxyTypeManager,
// KvProxy类型在开发出来后需要在kvproxy-type-register.cpp中
// 使用宏REGISTER_KVPROXY_TYPE(type_class, type_name)进行类型注册,
// 注册过后就可以使用了
#ifndef KVPROXY_KVPROXY_TYPE_H_
#define KVPROXY_KVPROXY_TYPE_H_

#include <boost/preprocessor.hpp>
#include <forward_list>
#include <string>
#include "common/base/singleton.h"
#include "kvproxy/kvproxy.h"

namespace kvproxy {

// 对KvProxy类型的描述,每一种KvProxy类型对应
// 一个KvProxyType对象，可以通过NewKvProxy()创建
// KvProxy对象，创建出来的KvProxy对象需要自行管理对象生命周期，
// 当不再使用的时候调用delete释放掉
class KvProxyType {
 public:
  KvProxyType(const std::string &type_name):
      type_name_(type_name) {
  }

  virtual ~KvProxyType();

  // 创建KvProxy对象，创建的对象在不在使用的时候需要
  // delete以释放资源
  virtual KvProxy * NewKvProxy() = 0;

  inline const std::string & type_name();
 private:
  std::string type_name_;
};

template <typename KvProxyClass>
class KvProxyTypeImpl: public KvProxyType {
 public:
  KvProxyTypeImpl(const std::string &type_name)
      : KvProxyType(type_name) { }

  virtual KvProxy * NewKvProxy() {
    return new KvProxyClass();
  }
};

// 对KvProxyType类型进行管理,
// 提供类型注册Regiester()方法,
// 从KvProxy类型名称创建KvProxy对象的NewKvProxy()方法
class KvProxyTypeManager : public common::SingletonBase<KvProxyTypeManager> {
  friend class SingletonBase<KvProxyTypeManager>;
 private:
  KvProxyTypeManager() {} 
  ~KvProxyTypeManager();
 public:
  // 把new_type注册给KvProxyTypeManager,
  // 同时所有权转移给了KvProxyTypeManager
  void Register(KvProxyType *new_type);

  // 创建一个type为type_name的KvProxy对象，
  // 此对象最后使用KvProxyType::DeleteKvProxy()进行销毁
  KvProxy * NewKvProxy(const std::string &type_name);

 private:
  KvProxyType * Find(const std::string &type_name);

 private:
  std::forward_list<KvProxyType *> type_list_;
};

inline const std::string & KvProxyType::type_name() {
  return type_name_;
}

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_TYPE_H_
