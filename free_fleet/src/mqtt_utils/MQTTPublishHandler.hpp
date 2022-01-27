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

#ifndef FREE_FLEET__SRC__MQTT_UTILS__MQTTPUBLISHHANDLER_HPP
#define FREE_FLEET__SRC__MQTT_UTILS__MQTTPUBLISHHANDLER_HPP

#include <memory>
#include <mqtt/async_client.h>
#include <mqtt/client.h>

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
namespace mqtt {

template <typename Message>
class MQTTPublishHandler
{
public:

  using SharedPtr = std::shared_ptr<MQTTPublishHandler>;

private:

  std::string topic_name;

  async_client_ptr async_cli_ptr;

  bool ready;

public:

  MQTTPublishHandler(
      const async_client_ptr _async_cli_ptr,
      const std::string& _topic_name) :
    topic_name(_topic_name), async_cli_ptr(_async_cli_ptr)
  {
    // TODO: Find out if there is anything to check?
    // TODO: Find out if should do the connection here
    ready = true;
  }

  ~MQTTPublishHandler()
  {}

  bool is_ready()
  {
    return ready;
  }

  bool write(Message* msg)
  {
    std::string msg_string;
    msg->SerializeToString(&msg_string);
    char payload[msg_string.length()];
    std::memcpy(payload, msg_string.data(), msg_string.length());
    delivery_token_ptr pubtok;
    pubtok = async_cli_ptr->publish(topic_name, payload, msg_string.length());
    // TODO: find a way to check if the the publish fails
    // TODO: find if wait_for(TIMEOUT) is time consuming
    return true;
  }

};

} // namespace dds
} // namespace free_fleet

#endif // FREE_FLEET__SRC__DDS_UTILS__DDSPUBLISHHANDLER_HPP
