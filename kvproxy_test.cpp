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
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>  // NOLINT
#include <string>
#include "kvproxy/kvproxy-manager.h"
#include "thirdparty/glog/logging.h"

using std::string;
using std::map;
using kvproxy::KvProxy;
using kvproxy::KvProxyManager;

static void SimpleDataExample(const string &type_name, const string &key) {
  std::string kv_proxy_name = "example";
  KvProxy* example_kvproxy = KvProxyManager::Instance().GetKvProxy(kv_proxy_name);
  LOG_IF(FATAL, (example_kvproxy == NULL)) << "find kv proxy fails:" << kv_proxy_name;

  string value = "12345";
  if (example_kvproxy->Put(key, value) == KV_EC_OK) {  // put
    LOG(INFO) << "put " << key << " ok.";
  } else {
    LOG(INFO) << "put " << key << " fail.";
    return;
  }

  if (example_kvproxy->Get(key, &value)== KV_EC_OK) {  // get
    LOG(INFO) << "get " << key << " ok, value is " << value;
  } else {
    LOG(INFO) << "get " << key << " fail.";
  }
  
  if (example_kvproxy->Put("key2", "6789") == KV_EC_OK) {  // put
    LOG(INFO) << "put " << "key2" << " ok.";
  } else {
    LOG(INFO) << "put " << "key2" << " fail.";
    return;
  }

  //example_kvproxy->Delete(key);  // delete

  if (boost::lexical_cast<uint32_t>(value) == 12345) {
    LOG(INFO) << "lexical_cast<uint32_t>() ok.";
  }

  kv_proxy_name = "example2";
  example_kvproxy = KvProxyManager::Instance().GetKvProxy(kv_proxy_name);
  LOG_IF(FATAL, (example_kvproxy == NULL)) << "find kv proxy fails:" << kv_proxy_name;

  if (example_kvproxy->Put(key, value) == KV_EC_OK) {  // put
    LOG(INFO) << "put " << key << " ok.";
  } else {
    LOG(INFO) << "put " << key << " fail.";
    return;
  }

}

int main() {
  KvProxyManager::Instance().Open("./kvproxy.conf");  // 加载kvproxy对象
  SimpleDataExample("SimpleDataExample", "key");
  KvProxyManager::Instance().Close();  // 关闭kvproxy对象
  return 0;
}
