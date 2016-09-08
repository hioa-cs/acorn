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
#ifndef DASHBOARD_DASHBOARD_HPP
#define DASHBOARD_DASHBOARD_HPP

#include <router.hpp>

#include "component.hpp"

namespace dashboard {

/**
 *
 */
class Dashboard {
private:
  using Component_collection = std::unordered_map<std::string, const Component*>;
public:
  /**
   *
   */
  Dashboard(size_t buffer_capacity = 4096);

  /**
   *
   */
  const server::Router& router() const
  { return router_; }

  /**
   *
   */
  void add(const Component&);

  /**
   *
   */
  template <typename Comp, typename... Args>
  inline void construct(Args&&...);

private:
  server::Router router_;
  WriteBuffer buffer_;
  Writer writer_;

  Component_collection components_;

  /**
   *
   */
  void setup_routes();

  /**
   *
   */
  void serve(server::Request_ptr, server::Response_ptr);

  /**
   *
   */
  void serialize(Writer&) const;

  /**
   *
   */
  void send_buffer(server::Response_ptr);

  /**
   *
   */
  void reset_writer();
}; //< class Dashboard

/**--v----------- Implementation Details -----------v--**/

inline void Dashboard::add(const Component& c) {
  components_.emplace(c.key(), &c);

  // A really simple way to setup routes, only supports read (GET)
  router_.on_get(std::string{"/"} + c.key(),
  [this, &c] (server::Request_ptr, server::Response_ptr res)
  {
    c.serialize(writer_);
    send_buffer(res);
  });
}

template <typename Comp, typename... Args>
inline void Dashboard::construct(Args&&... args) {
  static_assert(std::is_base_of<Component, Comp>::value, "Template type is not a Component");

  const Comp& c = *new Comp{std::forward<Args>(args)...};

  add(c);
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace dashboard

#endif //< DASHBOARD_DASHBOARD_HPP
