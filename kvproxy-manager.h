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
// kvproxy-manager.h提供对kvproxy对象的管理实现
#ifndef KVPROXY_KVPROXY_MANAGER_H_
#define KVPROXY_KVPROXY_MANAGER_H_

#include <map>
#include <string>
#include "common/base/singleton.h"
#include "kvproxy/ext-kvproxy.h"

namespace kvproxy {

class KvProxyManager: public common::Singleton<KvProxyManager> {
 public:
  KvProxyManager();
  ~KvProxyManager();

  // 初始化配置文件中配置的所有kvproxy
  // 在使用kvproxy之前需要先进行Open，但只需要调用一次即可
  // @param config_file kvproxy配置文件
  void Open(const std::string &config_file);

  // 获取为name的kvproxy对象
  // @param name kvproxy对象名称
  KvProxy * GetKvProxy(const std::string &name);

  ExtKvProxy * GetExtKvProxy(const std::string &name);

  // 关闭所有的kvproxy对象，释放占用的资源，
  // 在确定不在使用kvproxy的情况下调用，一般为进城结束时调用
  void Close();

 private:
  void CreateKvProxy(const std::string &type, const std::string &name,
                     const std::map<std::string, std::string> &param_map);

 private:
  std::map<std::string, KvProxy *> proxy_map_;
};

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_MANAGER_H_
