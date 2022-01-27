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

#ifndef FREE_FLEET__SRC__MQTT_UTILS__MQTTSUBSCRIBEHANDLER_HPP
#define FREE_FLEET__SRC__MQTT_UTILS__MQTTSUBSCRIBEHANDLER_HPP

#include <memory>
#include <vector>
#include <mqtt/async_client.h>
#include <mqtt/client.h>

using async_client = mqtt::async_client;
using async_client_ptr = mqtt::async_client_ptr;
using client = mqtt::client;
using client_ptr = mqtt::client_ptr;
using connect_options_builder = mqtt::connect_options_builder;
using message = mqtt::message;
using const_message_ptr = mqtt::const_message_ptr;
using token_ptr = mqtt::token_ptr;
using connect_response = mqtt::connect_response;
using exception = mqtt::exception;
using delivery_token_ptr = mqtt::delivery_token_ptr;


namespace free_fleet {
namespace mqtt {

template <typename Message, int MaxSamplesNum = 1>
class MQTTSubscribeHandler
{
public:

  using SharedPtr = std::shared_ptr<MQTTSubscribeHandler>;

private:

  std::string topic_name;

  client_ptr cli_ptr;

  std::vector<std::string> info;

  std::array<std::shared_ptr<Message>, MaxSamplesNum> shared_msgs;

  std::vector<Message> ff_data_msgs;

  /* This bolean pointer is passed in when the subscriber handler is created
  When update state is called it should be set to true and the call back function
  would process the received mesages (i.e deserialize them) and store then in a
  std::vector container. This is done as I have yet to find a way to read from the
  */

  // bool* process_msg;

  bool ready;

public:

  MQTTSubscribeHandler(
      const client_ptr _cli_ptr,
      const std::string& _topic_name) :
  cli_ptr(_cli_ptr), topic_name(_topic_name)
  {
    // TODO: Find out if there is anything to check?
    // TODO: Find out if should do the connection here
    ready = true;
  }

  // NOTE: Since fields of the Clients and Servers no longer contain The mqtt
  // client pointers perhaps we could disconnect using the subscribe handler's
  // deconstructor. This would be fine at least for subscribers since they have
  // a different client for every topic but this would pose and issue for
  // publishers since they share the same client.
  ~MQTTSubscribeHandler()
  {}

  bool is_ready()
  {
    return ready;
  }

  std::vector<std::shared_ptr<const Message>> read()
  {
    std::vector<std::shared_ptr<const Message>> msgs;
    if (!is_ready())
      return msgs;
    info.clear();

    for (int i = 0; i < MaxSamplesNum; i ++){
      const_message_ptr msg =
          cli_ptr->consume_message();
      std::string payload = msg->get_payload();
      // We do not deserialize here as it may be time consuming.
      info.push_back(payload);
    }

    if (info.empty())
    {
      // TODO: Find out what to check and how to handle
      // errors
      msgs.clear();
      return msgs;
    }

    if (!info.empty())
    {
      for (int i = 0; i < MaxSamplesNum; ++i)
      {
        Message ff_data;
        std::string data = info[i];
        bool parsed = ff_data.ParseFromString(data);
        if (parsed)
          ff_data_msgs.push_back(ff_data);
      }
      for (int i = 0; i < MaxSamplesNum; ++i)
      {
        // NOTE: We for another for loop because the address may change
        // while doing push_back()
        // TODO: Find a more memory safe way to do this!!!
        auto ff_data_ptr = std::make_shared<Message>(ff_data_msgs[i]);
        msgs.push_back(ff_data_ptr);
      }
      return msgs;
    }
    msgs.clear();
    return msgs;
  }

};

} // namespace dds
} // namespace free_fleet


#endif // FREE_FLEET__SRC__DDS_UTILS__DDSSUBSCRIBEHANDLER_HPP
