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
#include "kvproxy/kvproxy-type-register.h"
#include "kvproxy/file-kvproxy.h"
#include "kvproxy/mock-kvproxy.h"

namespace common {
namespace kvproxy {

void RegisterAllBuiltinKvProxyTypes() {
  REGISTER_KVPROXY_TYPE(FileKvProxy, "KVFILE");
  REGISTER_KVPROXY_TYPE(MockKvProxy, "MOCK");
}

}  // namespace kvproxy
}  // namespace common
