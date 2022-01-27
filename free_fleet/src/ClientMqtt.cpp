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


#include <free_fleet/ClientMqtt.hpp>

#include <mqtt/async_client.h>
#include <mqtt/client.h>

#include "ClientMqttImpl.hpp"

#include "messages/FleetMessages.pb.h"
#include "mqtt_utils/MQTTPublishHandler.hpp"
#include "mqtt_utils/MQTTSubscribeHandler.hpp"

using mqtt_async_client = mqtt::async_client;
using mqtt_async_client_ptr = mqtt::async_client_ptr;
using mqtt_client = mqtt::client;
using mqtt_client_ptr = mqtt::client_ptr;

using connect_options_builder = mqtt::connect_options_builder;
using message = mqtt::message;
using message_ptr = mqtt::message_ptr;
using token_ptr = mqtt::token_ptr;
using connect_response = mqtt::connect_response;
using exception = mqtt::exception;
using delivery_token_ptr = mqtt::delivery_token_ptr;

namespace free_fleet {

ClientMqtt::SharedPtr ClientMqtt::make(const ClientMqttConfig& _config)
{
  SharedPtr client = SharedPtr(new ClientMqtt(_config));

  std::string state_pub_id = _config.mqtt_client_id +
      _config.mqtt_robot_state_topic +"_pub";
  mqtt_async_client_ptr state_pub_cli_ptr = std::make_shared<mqtt_async_client>(
    _config.mqtt_server_address, state_pub_id);

  // TODO: Compress all subscribers into a single client
  std::string mode_request_sub_id = _config.mqtt_client_id +
      _config.mqtt_mode_request_topic + "_sub";
  mqtt_client_ptr mode_req_cli_ptr = std::make_shared<mqtt_client>(
      _config.mqtt_server_address, mode_request_sub_id);

  std::string path_request_sub_id = _config.mqtt_client_id +
      _config.mqtt_path_request_topic + "_sub";
  mqtt_client_ptr path_req_cli_ptr = std::make_shared<mqtt_client>(
      _config.mqtt_server_address, path_request_sub_id);

  std::string destination_request_sub_id = _config.mqtt_client_id +
      _config.mqtt_destination_request_topic + "_sub";
  mqtt_client_ptr dest_req_cli_ptr = std::make_shared<mqtt_client>(
      _config.mqtt_server_address, path_request_sub_id);

  // connect mqtt clients to mqtt server
  token_ptr async_connect_tok = state_pub_cli_ptr->connect();
  async_connect_tok->wait();
  connect_response mode_req_connect_resp = mode_req_cli_ptr->connect();
  mode_req_cli_ptr->subscribe(_config.mqtt_mode_request_topic, _config.QOS);

  connect_response path_req_connect_resp = path_req_cli_ptr->connect();
  path_req_cli_ptr->subscribe(_config.mqtt_path_request_topic, _config.QOS);

  connect_response dest_req_connect_resp = dest_req_cli_ptr->connect();
  dest_req_cli_ptr->subscribe(_config.mqtt_destination_request_topic,
      _config.QOS);





  // TODO: Handle failure to connect to mqtt server

  mqtt::MQTTPublishHandler<free_fleet_data::RobotState>::SharedPtr state_pub(
      new mqtt::MQTTPublishHandler<free_fleet_data::RobotState>(
          state_pub_cli_ptr, _config.mqtt_robot_state_topic));

  mqtt::MQTTSubscribeHandler<free_fleet_data::ModeRequest>::SharedPtr
      mode_request_sub(
          new mqtt::MQTTSubscribeHandler<free_fleet_data::ModeRequest>(
              mode_req_cli_ptr, _config.mqtt_mode_request_topic));

  mqtt::MQTTSubscribeHandler<free_fleet_data::PathRequest>::SharedPtr
      path_request_sub(
          new mqtt::MQTTSubscribeHandler<free_fleet_data::PathRequest>(
              path_req_cli_ptr, _config.mqtt_path_request_topic));

  mqtt::MQTTSubscribeHandler<free_fleet_data::DestinationRequest>::SharedPtr
      destination_request_sub(
          new mqtt::MQTTSubscribeHandler<free_fleet_data::DestinationRequest>(
              dest_req_cli_ptr, _config.mqtt_destination_request_topic));

  // TODO: Handle THIS!! multiple sync clients for subscribtion
  client->impl->start(ClientMqttImpl::Fields{
    //   async_cli_ptr,
    //   mode_req_cli_ptr,
    //   path_req_cli_ptr,
    //   dest_req_cli_ptr,
      std::move(state_pub),
      std::move(mode_request_sub),
      std::move(path_request_sub),
      std::move(destination_request_sub)});
  return client;
}

ClientMqtt::ClientMqtt(const ClientMqttConfig& _config)
{
  impl.reset(new ClientMqttImpl(_config));
}

ClientMqtt::~ClientMqtt()
{}

bool ClientMqtt::send_robot_state(const messages::RobotState& _new_robot_state)
{
  return impl->send_robot_state(_new_robot_state);
}

bool ClientMqtt::read_mode_request(messages::ModeRequest& _mode_request)
{
  return impl->read_mode_request(_mode_request);
}

bool ClientMqtt::read_path_request(messages::PathRequest& _path_request)
{
  return impl->read_path_request(_path_request);
}

bool ClientMqtt::read_destination_request(
    messages::DestinationRequest& _destination_request)
{
  return impl->read_destination_request(_destination_request);
}

} // namespace free_fleet
