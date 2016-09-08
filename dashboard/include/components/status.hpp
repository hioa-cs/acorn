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
#ifndef DASHBOARD_COMPONENTS_STATUS_HPP
#define DASHBOARD_COMPONENTS_STATUS_HPP

#include "../component.hpp"

#include <os>
#include <rtc>

namespace dashboard {

/**
 * This class represents a component that can be installed into the
 * dashboard. It provides information relating to the status of the
 * VM's operating system such as the following:
 * - Operating system's version number
 * - The name of the running service
 * - Real-time information about the service's uptime
 * - The CPU's frequency
 * - Real-time information about the service's heap usage (memory related)
 * - A real-time clock providing the current date and time
 */
class Status : public Component {
public:

  /**
   * Get the sole instance of this class
   *
   * @return The sole instance of this class
   */
  static Status& instance() {
    static Status s;
    return s;
  }

  /**
   * Get the component identifier
   *
   * @return The component identifier as a {std::string} object
   */
  std::string key() const override
  { return "status"; }

  /**
   *
   */
  void serialize(Writer& writer) const override {
    writer.StartObject();

    writer.Key("version");
    writer.String(OS::version());

    writer.Key("service");
    writer.String(Service::name());

    writer.Key("uptime");
    writer.Int64(OS::uptime());

    writer.Key("heap_usage");
    writer.Uint64(OS::heap_usage());

    writer.Key("cpu_freq");
    writer.Double(OS::cpu_freq().count());

    writer.Key("current_time");
    long hest = RTC::now();
    struct tm* tt =
      gmtime (&hest);
    char datebuf[32];
    strftime(datebuf, sizeof datebuf, "%FT%TZ", tt);
    writer.String(datebuf);

    writer.EndObject();
  }

private:
  Status() {};
}; //< class Status

} //< namespace dashboard

#endif //< DASHBOARD_COMPONENTS_STATUS_HPP
