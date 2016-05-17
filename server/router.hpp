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

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <regex>
#include <stdexcept>
#include <functional>

#include "request.hpp"
#include "response.hpp"

namespace server {

  //-------------------------------
  // This class is used to provide
  // route resolution
  //-------------------------------
  class Router {
  private:
    //-------------------------------
    // Internal class type aliases
    //-------------------------------
    using Route_expr = std::regex;
    using Callback   = std::function<void(Request_ptr, Response_ptr)>;

    struct Route {
      Route_expr expr;
      Callback   callback;

      explicit Route(const char* ex, Callback c)
        : expr{ex}
        , callback{c}
      {}
    }; //< struct Route

    using Span        = gsl::span<char>;
    using Route_table = std::unordered_map<http::Method, std::vector<Route>>;

  public:
    //-------------------------------
    // Default constructor to set up
    // default routes
    //-------------------------------
    explicit Router() = default;

    //-------------------------------
    // Default destructor
    //-------------------------------
    ~Router() noexcept = default;

    //-------------------------------
    // Default move constructor
    //-------------------------------
    Router(Router&&) = default;

    //-------------------------------
    // Default move assignment operator
    //-------------------------------
    Router& operator = (Router&&) = default;

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_options(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_get(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_head(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_post(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_put(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_delete(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_trace(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_connect(Routee&& route, Callback result);

    //-------------------------------
    // Add a route mapping for route
    // resolution upon request
    //
    // @tparam (std::string) route - The route to map unto a
    //                               resulting destination
    //
    // @param result - The route mapping
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee>
    Router& on_patch(Routee&& route, Callback result);

    //-------------------------------
    // Install a new route table for
    // route resolutions
    //
    // @tparam (http::Router) new_routes - The new route table
    //                                     to install
    //
    // @return - The object that invoked this method
    //-------------------------------
    template <typename Routee_Table>
    Router& install_new_configuration(Routee_Table&& new_routes);


    /**
     * Get the route callback where Route_expr matched a given path
     *
     * @param path : the route path
     * @note : not const becuase it uses index operator to a map
     **/
    Callback match(const http::Method, const std::string& path);

  private:
    Router(const Router&) = delete;
    Router& operator = (const Router&) = delete;

    Route_table route_table_;
  }; //< class Router


  class Router_error : public std::runtime_error {
    using runtime_error::runtime_error;
  };


  /**--v----------- Implementation Details -----------v--**/

  template <typename Routee>
  inline Router& Router::on_options(Routee&& route, Callback result) {
    route_table_[http::OPTIONS].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_get(Routee&& route, Callback result) {
    route_table_[http::GET].emplace_back(std::forward<Routee>(route),  result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_head(Routee&& route, Callback result) {
    route_table_[http::HEAD].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_post(Routee&& route, Callback result) {
    route_table_[http::POST].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_put(Routee&& route, Callback result) {
    route_table_[http::PUT].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }


  template <typename Routee>
  inline Router& Router::on_delete(Routee&& route, Callback result) {
    route_table_[http::DELETE].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_trace(Routee&& route, Callback result) {
    route_table_[http::TRACE].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_connect(Routee&& route, Callback result) {
    route_table_[http::CONNECT].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee>
  inline Router& Router::on_patch(Routee&& route, Callback result) {
    route_table_[http::PATCH].emplace_back(std::forward<Routee>(route), result);
    return *this;
  }

  template <typename Routee_Table>
  inline Router& Router::install_new_configuration(Routee_Table&& new_routes) {
    route_table_ = std::forward<Routee_Table>(new_routes).route_table_;
    return *this;
  }


  inline Router::Callback Router::match(const http::Method method, const std::string& path) {
    auto routes = route_table_[method];

    if (routes.empty()) {
      throw Router_error("No routes for method: [" + http::method::str(method) + "]");
    }

    for (auto& route : routes) {
      if (std::regex_match(path.begin(), path.end(), route.expr)) {
        return route.callback;
      }
    }

    throw Router_error("No matching route for " + http::method::str(method) + " " + path);
  }

  /**--^----------- Implementation Details -----------^--**/

} //< namespace server

#endif //< ROUTER_HPP
