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
#ifndef KVPROXY_UTILS_H_
#define KVPROXY_UTILS_H_

#include <boost/algorithm/string.hpp>
#include <string>
#include "common/utility/lexical-cast.h"
#include "thirdparty/glog/logging.h"

#define SET_PARAM_EMPTY_RETURN(to, param_name, param_map, err_msg)\
    do {\
      auto mit = param_map.find(param_name);\
      if (mit == param_map.end()) {\
        LOG(ERROR) << (err_msg);\
        return false;\
      }\
      std::string tmp = mit->second;\
      boost::trim(tmp);\
      if (tmp.empty()) {\
        LOG(ERROR) << (err_msg);\
        return false;\
      }\
      to = lexical_cast< decltype(to) >(tmp);\
    } while(0)

#define SET_PARAM_EMPTY_DEFAULT(to, param_name, param_map, def_value, err_msg)\
    do {\
      auto mit = param_map.find(param_name);\
      if (mit == param_map.end()) {\
        LOG(INFO) << (err_msg);\
        to = def_value;\
        break;\
      }\
      std::string tmp = mit->second;\
      boost::trim(tmp);\
      if (tmp.empty()) {\
        LOG(INFO) << (err_msg);\
        to = def_value;\
        break;\
      }\
      to = lexical_cast< decltype(to) >(tmp);\
    } while(0)

#endif  // KVPROXY_UTILS_H_
