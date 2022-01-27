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

#ifndef FREE_FLEET__INCLUDE__FREE_FLEET__SERVERCONFIGMQTT_HPP
#define FREE_FLEET__INCLUDE__FREE_FLEET__SERVERCONFIGMQTT_HPP

#include <string>

namespace free_fleet {

struct ServerMqttConfig
{
  std::string mqtt_server_address = "tcp://localhost:1883";
  std::string mqtt_client_id = "ff_server";
  int QOS = 1;
  std::string LWT_PAYLOAD = "Last will and testament.";
  std::string mqtt_robot_state_topic = "robot_state";
  std::string mqtt_mode_request_topic = "mode_request";
  std::string mqtt_path_request_topic = "path_request";
  std::string mqtt_destination_request_topic = "destination_request";

  void print_config() const;
};

} // namespace free_fleet

#endif // FREE_FLEET__INCLUDE__FREE_FLEET__SERVERCONFIG_HPP
