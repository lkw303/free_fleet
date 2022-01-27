  /*
 * Copyright (C) 2019 Open Source Robotics Foundation
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

#include "ServerMqttImpl.hpp"
#include "messages/mqtt_message_utils.hpp"
#include "messages/FleetMessages.pb.h"

namespace free_fleet {

ServerMqtt::ServerMqttImpl::ServerMqttImpl(const ServerMqttConfig& _config) :
  server_config(_config)
{}

ServerMqtt::ServerMqttImpl::~ServerMqttImpl()
{
  // Disconnect
  //sync client does not have wait()
  // fields.cli_ptr->disconnect()->wait();
  // fields.async_cli_ptr->disconnect()->wait();
  // TODO: Find out how to handle failures.

  //TODO: Find a way to disconnect now that fields
  // do not contain any pointers to the mqtt clients.
}

void ServerMqtt::ServerMqttImpl::start(Fields _fields)
{
  fields = std::move(_fields);
}

// TODO: This function should be the call back function
// of the subscriber
bool ServerMqtt::ServerMqttImpl::read_robot_states(
    std::vector<messages::RobotState>& _new_robot_states)
{
  auto robot_states = fields.robot_state_sub->read();
  if (!robot_states.empty())
  {
    _new_robot_states.clear();
    for (size_t i = 0; i < robot_states.size(); ++i)
    {
      messages::RobotState tmp_robot_state;
      convert(*(robot_states[i]), tmp_robot_state);
      _new_robot_states.push_back(tmp_robot_state);
    }
    return true;
  }
  return false;
}

bool ServerMqtt::ServerMqttImpl::send_mode_request(
    const messages::ModeRequest& _mode_request)
{
  free_fleet_data::ModeRequest new_mr;
  convert(_mode_request, new_mr);
  bool sent = fields.mode_request_pub->write(&new_mr);
  return sent;
}

bool ServerMqtt::ServerMqttImpl::send_path_request(
    const messages::PathRequest& _path_request)
{
  free_fleet_data::PathRequest new_pr;
  convert(_path_request, new_pr);
  bool sent = fields.path_request_pub->write(&new_pr);
  return sent;
}

bool ServerMqtt::ServerMqttImpl::send_destination_request(
    const messages::DestinationRequest& _destination_request)
{
  free_fleet_data::DestinationRequest new_dr;
  convert(_destination_request, new_dr);
  bool sent = fields.destination_request_pub->write(&new_dr);
  return sent;
}

} // namespace free_fleet
