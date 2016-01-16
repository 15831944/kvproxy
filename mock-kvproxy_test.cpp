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
#include <string>
#include "kvproxy/kvproxy-manager.h"
#include "kvproxy/mock-kvproxy.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"
#include "thirdparty/perftools/heap-checker.h"

using std::string;
using std::map;
using kvproxy::KvProxy;
using kvproxy::MockKvProxy;
using kvproxy::KvProxyManager;

void SetMockData(MockKvProxy* kv_proxy, const string& key, const string& value) {
  kv_proxy->Put(key, value);
  kv_proxy->SetErrorCode(KV_EC_OK);
}

TEST(MockKvProxyTest, Get) {
  std::string kv_proxy_name = "mock_example";
  KvProxy* kv_proxy = KvProxyManager::Instance().GetKvProxy(kv_proxy_name);
  LOG_IF(FATAL, (kv_proxy == NULL)) << "find kv proxy fails:" << kv_proxy_name;

  MockKvProxy* mock_kv_proxy = dynamic_cast<MockKvProxy*>(kv_proxy);
  LOG_ASSERT(mock_kv_proxy != nullptr);
  mock_kv_proxy->SetErrorCode(KV_EC_NOT_EXISTS);
  string key = "key";
  string value;
  int ec = mock_kv_proxy->Get(key, &value);
  EXPECT_EQ(KV_EC_NOT_EXISTS, ec);
  SetMockData(mock_kv_proxy, key, "123");
  ec = mock_kv_proxy->Get(key, &value);
  EXPECT_EQ(KV_EC_OK, ec);
  EXPECT_EQ("123", value);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  KvProxyManager::Instance().Open("./kvproxy.conf");  // 加载kvproxy对象
  HeapLeakChecker checker("Test with memory leak detection");
  return RUN_ALL_TESTS() || !checker.NoLeaks();
}
