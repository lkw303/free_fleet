/*
 * Copyright (C) 2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef INCLUDE__FREE_FLEET__MANAGER__ROBOTHANDLE_HPP
#define INCLUDE__FREE_FLEET__MANAGER__ROBOTHANDLE_HPP

#include <memory>
#include <functional>

#include <rmf_utils/impl_ptr.hpp>

#include <free_fleet/manager/RobotInfo.hpp>

namespace free_fleet {
namespace manager {

class RobotHandle
{
public:

  /// Returns an immutable RobotInfo for this particular robot.
  const RobotInfo& info() const;

  ///
  void receive_updates(
    std::function<void(const RobotInfo& new_state)> callback);


  class Implementation;
private:
  rmf_utils::impl_ptr<Implementation> _pimpl;
  RobotHandle();
};

} // namespace manager
} // namespace free_fleet

#endif // INCLUDE__FREE_FLEET__MANAGER__ROBOTHANDLE_HPP
