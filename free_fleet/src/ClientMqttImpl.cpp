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

#include "ClientMqttImpl.hpp"
#include "messages/mqtt_message_utils.hpp"
#include "messages/FleetMessages.pb.h"

namespace free_fleet {

ClientMqtt::ClientMqttImpl::ClientMqttImpl(const ClientMqttConfig& _config) :
  client_config(_config)
{}

ClientMqtt::ClientMqttImpl::~ClientMqttImpl()
{

}

void ClientMqtt::ClientMqttImpl::start(Fields _fields)
{
  fields = std::move(_fields);
}

bool ClientMqtt::ClientMqttImpl::send_robot_state(
    const messages::RobotState& _new_robot_state)
{
  free_fleet_data::RobotState new_rs;
  convert(_new_robot_state, new_rs);
  bool sent = fields.state_pub->write(&new_rs);
  return sent;
}

bool ClientMqtt::ClientMqttImpl::read_mode_request
    (messages::ModeRequest& _mode_request)
{
  auto mode_requests = fields.mode_request_sub->read();
  if (!mode_requests.empty())
  {
    convert(*(mode_requests[0]), _mode_request);
    return true;
  }
  return false;
}

bool ClientMqtt::ClientMqttImpl::read_path_request(
    messages::PathRequest& _path_request)
{
  auto path_requests = fields.path_request_sub->read();
  if (!path_requests.empty())
  {
    convert(*(path_requests[0]), _path_request);
    return true;
  }
  return false;
}

bool ClientMqtt::ClientMqttImpl::read_destination_request(
    messages::DestinationRequest& _destination_request)
{
  auto destination_requests = fields.destination_request_sub->read();
  if (!destination_requests.empty())
  {
    convert(*(destination_requests[0]), _destination_request);
    return true;
  }
  return false;
}

} // namespace free_fleet
