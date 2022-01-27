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

#include <free_fleet/ClientMqttConfig.hpp>

#include <cstdio>

namespace free_fleet {

void ClientMqttConfig::print_config() const
{
  printf("  CLIENT-SERVER DDS CONFIGURATION\n");
  printf("  MQTT server address: %s\n", mqtt_server_address.c_str());
  printf("  MQTT client idL %s\n", mqtt_client_id.c_str());
  printf("  QOS: %d\n", QOS);
  printf("  LWTPAYLOAD: %s\n", LWTPAYLOAD.c_str());
  printf("  TOPICS\n");
  printf("    robot state: %s\n", mqtt_robot_state_topic.c_str());
  printf("    mode request: %s\n", mqtt_mode_request_topic.c_str());
  printf("    path request: %s\n", mqtt_path_request_topic.c_str());
  printf("    destination request: %s\n",
      mqtt_destination_request_topic.c_str());
}

} // namespace free_fleet
