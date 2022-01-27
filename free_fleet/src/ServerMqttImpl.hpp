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

#ifndef FREE_FLEET__SRC__SERVERIMPLMQTT_HPP
#define FREE_FLEET__SRC__SERVERIMPLMQTT_HPP

#include <free_fleet/messages/RobotState.hpp>
#include <free_fleet/messages/ModeRequest.hpp>
#include <free_fleet/messages/PathRequest.hpp>
#include <free_fleet/messages/DestinationRequest.hpp>
#include <free_fleet/ServerMqtt.hpp>
#include <free_fleet/ServerMqttConfig.hpp>
#include <mqtt/async_client.h>
#include <mqtt/client.h>

#include "messages/FleetMessages.h"
#include "messages/FleetMessages.pb.h"
#include "mqtt_utils/MQTTPublishHandler.hpp"
#include "mqtt_utils/MQTTSubscribeHandler.hpp"

using async_client_ptr = mqtt::async_client_ptr;
using client_ptr = mqtt::client_ptr;
using message_ptr = mqtt::message_ptr;

namespace free_fleet {

class ServerMqtt::ServerMqttImpl
{
public:

  /// MQTT related fields required for the server to operate
  struct Fields
  {
    // NOTE: Trying without participant as it may be redundant
    // client_ptr cli_ptr;

    /// MQTT participant that is tied to the configured dds_domain_id
    // async_client_ptr async_cli_ptr;

    /// MQTT subscribers for new incoming robot states from clients
    mqtt::MQTTSubscribeHandler<free_fleet_data::RobotState, 10>::SharedPtr
        robot_state_sub;

    /// MQTT publisher for mode requests to be sent to clients
    mqtt::MQTTPublishHandler<free_fleet_data::ModeRequest>::SharedPtr
        mode_request_pub;

    /// MQTT publisher for path requests to be sent to clients
    mqtt::MQTTPublishHandler<free_fleet_data::PathRequest>::SharedPtr
        path_request_pub;

    /// MQTT publisher for destination requests to be sent to clients
    mqtt::MQTTPublishHandler<free_fleet_data::DestinationRequest>::SharedPtr
        destination_request_pub;

  };

  ServerMqttImpl(const ServerMqttConfig& config);

  ~ServerMqttImpl();

  void start(Fields fields);

  bool read_robot_states(std::vector<messages::RobotState>& new_robot_states);

  bool send_mode_request(const messages::ModeRequest& mode_request);

  bool send_path_request(const messages::PathRequest& path_request);

  bool send_destination_request(
      const messages::DestinationRequest& destination_request);

private:

  Fields fields;

  ServerMqttConfig server_config;

};


} // namespace free_fleet

#endif // FREE_FLEET__SRC__SERVERIMPL_HPP
