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

#include <free_fleet/ServerMqtt.hpp>

#include <mqtt/async_client.h>
#include <mqtt/client.h>

#include "ServerMqttImpl.hpp"

#include "messages/FleetMessages.h"
#include "messages/FleetMessages.pb.h"

using async_client = mqtt::async_client;
using async_client_ptr = mqtt::async_client_ptr;
using client = mqtt::client;
using client_ptr = mqtt::client_ptr;
using connect_options_builder = mqtt::connect_options_builder;
using message = mqtt::message;
using message_ptr = mqtt::message_ptr;
using token_ptr = mqtt::token_ptr;
using connect_response = mqtt::connect_response;
using exception = mqtt::exception;
using delivery_token_ptr = mqtt::delivery_token_ptr;

namespace free_fleet {

ServerMqtt::SharedPtr ServerMqtt::make(const ServerMqttConfig& _config)
{
  SharedPtr server = SharedPtr(new ServerMqtt(_config));

  const std::string mqtt_persist_dir = "./persist";

  // TODO: Consider changing to a single publisher/subscriber to  a single
  // client model.

  // Note: May need to use a shared pointer for client
  // instead as we will be calling move on it.
  // async_client async_client(_config.mqtt_server_address,
  //     _config.mqtt_client_id, mqtt_persist_dir);
  std::string pub_id = _config.mqtt_client_id + "_pub";
  async_client_ptr async_cli_ptr = std::make_shared<async_client>(
    _config.mqtt_server_address, pub_id);

  /* we have to make new clients for each subsciber
    The subscribers are using a synchronous client and
    mqtt v3 does not support callback functions for different
    topics.
  */
  // client client(_config.mqtt_server_address,
  //     _config.mqtt_client_id);

  std::string sub_id = _config.mqtt_client_id + "_sub";
  client_ptr cli_ptr = std::make_shared<client>(
      _config.mqtt_server_address, sub_id);

  bool failure = false;
  if (failure)
  {
    //TODO: handle failure
  }

  // NOTE: This may not be needed
  // auto connOpts = connect_options_builder()
  //   .clean_session()
  //   .will(message(_config.mqtt_robot_state_topic,
  //       _config.LWT_PAYLOAD.c_str(),_config.QOS));

  token_ptr async_connect_tok = async_cli_ptr->connect();
  async_connect_tok->wait();
  connect_response connect_resp = cli_ptr->connect();

  cli_ptr->subscribe(_config.mqtt_robot_state_topic, _config.QOS);

  // if (!connect_resp || !async_connect_tok)
  //   return nullptr;

  // TODO: Decide if we need a multiple clients for every subscriber
  // and publisher

  mqtt::MQTTSubscribeHandler<free_fleet_data::RobotState, 10>::SharedPtr state_sub(
      new mqtt::MQTTSubscribeHandler<free_fleet_data::RobotState, 10>(
          cli_ptr, _config.mqtt_robot_state_topic));

  mqtt::MQTTPublishHandler<free_fleet_data::ModeRequest>::SharedPtr
      mode_request_pub(
          new mqtt::MQTTPublishHandler<free_fleet_data::ModeRequest>(
              async_cli_ptr, _config.mqtt_mode_request_topic));

  mqtt::MQTTPublishHandler<free_fleet_data::PathRequest>::SharedPtr
      path_request_pub(
          new mqtt::MQTTPublishHandler<free_fleet_data::PathRequest>(
              async_cli_ptr, _config.mqtt_path_request_topic));

  mqtt::MQTTPublishHandler<free_fleet_data::DestinationRequest>::SharedPtr
      destination_request_pub(
          new mqtt::MQTTPublishHandler<free_fleet_data::DestinationRequest>(
              async_cli_ptr, _config.mqtt_destination_request_topic));


  server->impl->start(ServerMqttImpl::Fields{
      // cli_ptr,
      // async_cli_ptr,
      std::move(state_sub),
      std::move(mode_request_pub),
      std::move(path_request_pub),
      std::move(destination_request_pub)});

  return server;
}

ServerMqtt::ServerMqtt(const ServerMqttConfig& _config)
{
  impl.reset(new ServerMqttImpl(_config));
}

ServerMqtt::~ServerMqtt()
{}

bool ServerMqtt::read_robot_states(
    std::vector<messages::RobotState>& _new_robot_states)
{
  return impl->read_robot_states(_new_robot_states);
}

bool ServerMqtt::send_mode_request(const messages::ModeRequest& _mode_request)
{
  return impl->send_mode_request(_mode_request);
}

bool ServerMqtt::send_path_request(const messages::PathRequest& _path_request)
{
  return impl->send_path_request(_path_request);
}

bool ServerMqtt::send_destination_request(
    const messages::DestinationRequest& _destination_request)
{
  return impl->send_destination_request(_destination_request);
}

} // namespace free_fleet
