// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015-2016 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#ifndef DASHBOARD_COMPONENTS_MEMMAP_HPP
#define DASHBOARD_COMPONENTS_MEMMAP_HPP

#include <os>

#include "../component.hpp"

namespace dashboard {

/**
 *
 */
class Memmap : public Component {
public:

  /**
   *
   */
  static Memmap& instance() {
    static Memmap m;
    return m;
  }

  /**
   *
   */
  std::string key() const override
  { return "memmap"; }

  /**
   *
   */
  void serialize(Writer& writer) const override {
    writer.StartArray();
    for (auto i : OS::memory_map())
    {
      auto& entry = i.second;
      writer.StartObject();

      writer.Key("name");
      writer.String(entry.name());

      writer.Key("addr_start");
      writer.Uint(entry.addr_start());

      writer.Key("addr_end");
      writer.Uint(entry.addr_end());

      writer.Key("in_use");
      writer.Uint(entry.in_use());

      writer.Key("description");
      writer.String(entry.description());

      writer.EndObject();
    }
    writer.EndArray();
  }

private:
  Memmap() {}
}; //< class Memmap

} //< namespace dashboard

#endif //< DASHBOARD_COMPONENTS_MEMMAP_HPP
