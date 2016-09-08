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
#ifndef DASHBOARD_COMPONENTS_LOGGER_HPP
#define DASHBOARD_COMPONENTS_LOGGER_HPP

#include "../component.hpp"

#include <logger/logger.hpp>

namespace dashboard {

/**
 * This class represents a component that can be installed into the
 * dashboard. It provides information about the activity that's
 * occurring within the appliance
 */
class Logger : public Component {
public:

  /**
   *
   */
  Logger(::Logger& logger, size_t entries = 20)
   : logger_{logger}, entries_{entries}
  {}

  /**
   *
   */
  std::string key() const override
  { return "logger"; }

  /**
   *
   */
  void serialize(Writer& writer) const override {
    writer.StartArray();
    auto entries = logger_.entries(entries_);

    auto it = entries.begin();
    // Temporary hack to only send N latest
    const size_t N = 50;
    if(entries.size() > N)
      it += entries.size() - N;

    while(it != entries.end())
      writer.String(*it++);

    writer.EndArray();
  }

private:
  //------------------------------
  // Class data members
  const ::Logger& logger_;
  const size_t entries_;
  //------------------------------
}; //< class Logger

} //< namespace dashboard

#endif //< DASHBOARD_COMPONENTS_LOGGER_HPP
