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
// kvproxy.h提供KvProxy的接口,client只需要依赖KvProxy接口，
// 不需要关注具体实现细节
#ifndef KVPROXY_KVPROXY_H_
#define KVPROXY_KVPROXY_H_

#include <map>
#include <string>
#include <vector>
#include "kvproxy/normal-error.h"

namespace kvproxy {

class KvProxy {
 public:
  virtual ~KvProxy() { }

  // 打开数据源，只在打开数据源的时候调用一起
  // @param config_map 特定于数据源的配置
  virtual bool Open(const std::map<std::string, std::string>& config_map) = 0;

  // 关闭kvproxy，只有在销毁kvproxy的时候才调用，
  // 用于进行资源释放
  virtual void Close() = 0;

  // 查询指定key的数据
  // @param key 查询的key
  // @param result 返回结果
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int Get(const std::string& key, std::string* result) = 0;

  // 存入或修改指定key的数据
  // @param key 要添加的对应数据的key
  // @param value 指定的数据
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int Put(const std::string& key, const std::string& value) = 0;

  // 删除指定的key的数据
  // @param key 所要删除的key
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int Delete(const std::string& key) = 0;

  // 查询所有key_vec中的所有key，也就是muti
  // Get版本,有的后端kv存储系统提供多次查询的优化实现
  // @param key_vec key vector
  // @param result_map 返回结果
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int MGet(const std::vector<std::string>& key_vec, 
                   std::map<std::string, std::string>* result_map) = 0;

  // 存入或者修改key_map中的所有数据,也就是muti
  // Put版本，有的后端kv存储系统提供put的优化实现
  // @param kv_map 要存入或修改的key/value对
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int MPut(const std::map<std::string, std::string>& kv_map) = 0;

  // 删除key_vec中对应的所有key的数据
  // @param key_vec 要删除的所有key vector
  // @return 如果成功返回0，其它为错误代码，
  //         使用GetError()获取错误描述
  virtual int MDelete(const std::vector<std::string>& key_vec) = 0;

  // 获取错误码err_code对应的错误信息
  // @param err_code 错误码
  // @return 如果对应的错误码存在则返回错误描述，
  //         否则的字符串""
  virtual const char * GetError(int err_code) = 0;

};

}  // namespace kvproxy

#endif  // KVPROXY_KVPROXY_H_
