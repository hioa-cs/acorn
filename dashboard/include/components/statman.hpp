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
#ifndef DASHBOARD_COMPONENTS_STATMAN_HPP
#define DASHBOARD_COMPONENTS_STATMAN_HPP

#include "../component.hpp"

#include <statman>

namespace dashboard {

/**
 * This component provides VM statistics from various subsystems within
 * the operating system
 */
class Statman : public Component {
public:
  /**
   * Constructor that references the statistics provider from within
   * the operating system
   */
  Statman(::Statman& statman) noexcept
   : statman_{statman}
  {}

  /**
   * Get the component identifier
   *
   * @return The component identifier
   */
  std::string key() const override
  { return "statman"; }

  /**
   * Serialize this component to the specified writer as JSON
   *
   * @param
   * The writer to serialize the component to
   */
  void serialize(Writer& writer) const override;
private:
  ::Statman& statman_;
}; //< class Statman

/**--v----------- Implementation Details -----------v--**/

inline void Statman::serialize(Writer& writer) const {
  writer.StartArray();
  for(auto it = statman_.begin(); it != statman_.last_used(); ++it) {
    auto& stat = *it;
    writer.StartObject();

    writer.Key("name");
    writer.String(stat.name());

    writer.Key("value");
    std::string type = "";

    switch(stat.type()) {
      case Stat::UINT64:  writer.Uint64(stat.get_uint64());
                          type = "UINT64";
                          break;
      case Stat::UINT32:  writer.Uint(stat.get_uint32());
                          type = "UINT32";
                          break;
      case Stat::FLOAT:   writer.Double(stat.get_float());
                          type = "FLOAT";
                          break;
    }

    writer.Key("type");
    writer.String(type);

    writer.Key("index");
    writer.Int(stat.index());

    writer.EndObject();
  }

  writer.EndArray();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace dashboard

#endif //< DASHBOARD_COMPONENTS_STATMAN_HPP
