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
#include "kvproxy/kvproxy-manager.h"
#include <boost/algorithm/string.hpp>
#include <boost/cast.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <fstream>  // NOLINT
#include <map>
#include <string>
#include "kvproxy/kvproxy-status-decorator.h"
#include "kvproxy/kvproxy-type.h"
#include "kvproxy/kvproxy-type-register.h"
#include "kvproxy/utils.h"
#include "thirdparty/glog/logging.h"

namespace kvproxy {

KvProxyManager::KvProxyManager() {
  RegisterAllBuiltinKvProxyTypes();
}

KvProxyManager::~KvProxyManager() {
  Close();
}

void KvProxyManager::CreateKvProxy(const std::string &type,
                                   const std::string &name,
                                   const std::map<std::string, std::string> &param_map) {
  if (type.size() == 0 || name.size() == 0) {
    return;
  }

  LOG_IF(FATAL, (proxy_map_.find(name) != proxy_map_.end())) << "named kvproxy object is exists: name=" << name;

  KvProxy *proxy = KvProxyTypeManager::Instance().NewKvProxy(type);
  LOG_IF(FATAL, proxy == nullptr) << "KVProxy type [" << type << "] is not supported.";
  std::string status = "status";
  auto mit = param_map.find(status);
  if (mit != param_map.end() && boost::iequals(mit->second, "1")) {
    LOG(INFO) << "use status in kvproxy:type=" << type << ",name=" << name;
    proxy = new KvProxyStatusDecorator(proxy, name);
  }
  if (proxy != NULL) {
    if (!proxy->Open(param_map)) {
      LOG(FATAL) << "kvproxy open error: type=" << type << ",name=" << name;
    } else {
      LOG(INFO) << "kvproxy open success: type=" << type << ",name=" << name;
      proxy_map_.insert(std::make_pair(name, proxy));
    }
  }
}

void KvProxyManager::Open(const std::string & config_file) {
  using namespace boost::xpressive;  // NOLINT
  std::ifstream in(config_file.c_str());
  LOG_IF(FATAL, !in) << "open config_file error. maybe config_file is not eixsts:" << config_file;
  LOG_IF(INFO, in) << "open kvproxy file success:" << config_file;
  sregex proxy_regex = sregex::compile("^\\[\\s*(.+?)_(\\w+)\\s*\\]");
  sregex kv_regex = sregex::compile("^([^=]+)=([^#]+)");
  smatch proxy_match;
  smatch kv_match;
  std::string line;
  std::string proxy_name, proxy_type;
  std::map<std::string, std::string> config_map;
  while (std::getline(in, line)) {
    std::string::size_type idx = line.find("#");
    if (idx != std::string::npos) {
      line = line.substr(0, idx);
    }
    boost::trim(line);
    if (regex_search(line, proxy_match, proxy_regex)) {
      CreateKvProxy(proxy_type, proxy_name, config_map);
      config_map.clear();
      proxy_type = boost::trim_copy(proxy_match[1].str());
      proxy_name = boost::trim_copy(proxy_match[2].str());
      config_map["type"] = proxy_type;
      config_map["name"] = proxy_name;
    } else if (regex_search(line, kv_match, kv_regex)) {
      std::string key = kv_match[1].str();
      boost::trim(key);
      std::string value = kv_match[2].str();
      boost::trim_if(value, [](char ch) {
                     if (ch == '\"' ||
                         ch == '\r' ||
                         ch == '\n' ||
                         ch == '\t') {
                     return true;
                     }
                     return false;
                     });
      if (key.size() > 0) {
        config_map.insert(make_pair(key, value));
      }
    }
  }

  if (proxy_name != "") {
    CreateKvProxy(proxy_type, proxy_name, config_map);
  }
}

void KvProxyManager::Close() {
  while (!proxy_map_.empty()) {
    auto mit = proxy_map_.begin();
    mit->second->Close();
    delete mit->second;
    proxy_map_.erase(mit);
  }
}

KvProxy * KvProxyManager::GetKvProxy(const std::string & name) {
  auto mit = proxy_map_.find(name);
  if (mit != proxy_map_.end()) {
    return mit->second;
  }
  return nullptr;
}

ExtKvProxy* KvProxyManager::GetExtKvProxy(const std::string & name) {
  KvProxy* proxy = GetKvProxy(name);
  if (proxy == nullptr) {
    return nullptr;
  }
  ExtKvProxy* ext_proxy = boost::polymorphic_downcast<ExtKvProxy*>(proxy);
  LOG_IF(ERROR, ext_proxy == nullptr) <<  "can not downcast to ExtKvProxy";
  return ext_proxy;
}

}  // namespace kvproxy
