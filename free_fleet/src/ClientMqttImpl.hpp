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

#ifndef FREE_FLEET__SRC__CLIENTMQTTIMPL_HPP
#define FREE_FLEET__SRC__CLIENTMQTTIMPL_HPP

#include <free_fleet/messages/RobotState.hpp>
#include <free_fleet/messages/ModeRequest.hpp>
#include <free_fleet/messages/PathRequest.hpp>
#include <free_fleet/messages/DestinationRequest.hpp>
#include <free_fleet/ClientMqtt.hpp>
#include <free_fleet/ClientMqttConfig.hpp>

#include "messages/FleetMessages.pb.h"
#include "mqtt_utils/MQTTPublishHandler.hpp"
#include "mqtt_utils/MQTTSubscribeHandler.hpp"

namespace free_fleet {

class ClientMqtt::ClientMqttImpl
{
public:

  /// MQTT related fields required for the client to operate
  struct Fields
  {
    // TODO: Figure out if the mqtt clients should be added to fields.
    // and how to handle disconnecting if they are not added into fields
    /// MQTT publisher that handles sending out current robot states to the
    /// server
    mqtt::MQTTPublishHandler<free_fleet_data::RobotState>::SharedPtr
        state_pub;

    /// MQTT subscriber for mode requests coming from the server
    mqtt::MQTTSubscribeHandler<free_fleet_data::ModeRequest>::SharedPtr
        mode_request_sub;

    /// MQTT subscriber for path requests coming from the server
    mqtt::MQTTSubscribeHandler<free_fleet_data::PathRequest>::SharedPtr
        path_request_sub;

    /// MQTT subscriber for destination requests coming from the server
    mqtt::MQTTSubscribeHandler<free_fleet_data::DestinationRequest>::SharedPtr
        destination_request_sub;
  };

  ClientMqttImpl(const ClientMqttConfig& config);

  ~ClientMqttImpl();

  void start(Fields fields);

  bool send_robot_state(const messages::RobotState& new_robot_state);

  bool read_mode_request(messages::ModeRequest& mode_request);

  bool read_path_request(messages::PathRequest& path_request);

  bool read_destination_request(
      messages::DestinationRequest& destination_request);

private:

  Fields fields;

  ClientMqttConfig client_config;

};

} // namespace free_fleet

#endif // FREE_FLEET__SRC__CLIENTIMPL_HPP
