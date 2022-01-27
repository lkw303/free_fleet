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

#include <cstdio>

#include "ClientNodeMqttConfig.hpp"

namespace free_fleet
{
namespace ros1
{

void ClientNodeMqttConfig::get_param_if_available(
    const ros::NodeHandle& _node, const std::string& _key,
    std::string& _param_out)
{
  std::string tmp_param;
  if (_node.getParam(_key, tmp_param))
  {
    ROS_INFO("Found %s on the parameter server. Setting %s to %s",
        _key.c_str(), _key.c_str(), tmp_param.c_str());
    _param_out = tmp_param;
  }
}

void ClientNodeMqttConfig::get_param_if_available(
    const ros::NodeHandle& _node, const std::string& _key,
    int& _param_out)
{
  int tmp_param;
  if (_node.getParam(_key, tmp_param))
  {
    ROS_INFO("Found %s on the parameter server. Setting %s to %d.",
        _key.c_str(), _key.c_str(), tmp_param);
    _param_out = tmp_param;
  }
}

void ClientNodeMqttConfig::get_param_if_available(
    const ros::NodeHandle& _node, const std::string& _key,
    double& _param_out)
{
  double tmp_param;
  if (_node.getParam(_key, tmp_param))
  {
    ROS_INFO("Found %s on the parameter server. Setting %s to %.2f.",
        _key.c_str(), _key.c_str(), tmp_param);
    _param_out = tmp_param;
  }
}

void ClientNodeMqttConfig::print_config() const
{
  printf("ROS 1 CLIENT CONFIGURATION\n");
  printf("  fleet name: %s\n", fleet_name.c_str());
  printf("  robot name: %s\n", robot_name.c_str());
  printf("  robot model: %s\n", robot_model.c_str());
  printf("  level name: %s\n", level_name.c_str());
  printf("  wait timeout: %.1f\n", wait_timeout);
  printf("  update request frequency: %.1f\n", update_frequency);
  printf("  publish state frequency: %.1f\n", publish_frequency);
  printf("  maximum distance to first waypoint: %.1f\n",
      max_dist_to_first_waypoint);
  printf("  TOPICS\n");
  printf("    battery state: %s\n", battery_state_topic.c_str());
  printf("    move base server: %s\n", move_base_server_name.c_str());
  printf("    docking trigger server: %s\n", docking_trigger_server_name.c_str());
  printf("  ROBOT FRAMES\n");
  printf("    map frame: %s\n", map_frame.c_str());
  printf("    robot frame: %s\n", robot_frame.c_str());
  printf("CLIENT-SERVER DDS CONFIGURATION\n");
  printf("  mqtt server address: %s\n", mqtt_server_address.c_str());
  printf("  TOPICS\n");
  printf("    robot state: %s\n", mqtt_state_topic.c_str());
  printf("    mode request: %s\n", mqtt_mode_request_topic.c_str());
  printf("    path request: %s\n", mqtt_path_request_topic.c_str());
  printf("    destination request: %s\n",
      mqtt_destination_request_topic.c_str());
  printf("    QOS: %d\n", QOS);
  printf("    LWTPAYLOAD: %s\n", LWTPAYLOAD.c_str());

}

ClientMqttConfig ClientNodeMqttConfig::get_client_config() const
{
  ClientMqttConfig client_config;
  client_config.mqtt_server_address = mqtt_server_address;
  client_config.mqtt_client_id = mqtt_client_id;
  client_config.mqtt_robot_state_topic = mqtt_state_topic;
  client_config.mqtt_mode_request_topic = mqtt_mode_request_topic;
  client_config.mqtt_path_request_topic = mqtt_path_request_topic;
  client_config.mqtt_destination_request_topic = mqtt_destination_request_topic;
  client_config.LWTPAYLOAD = LWTPAYLOAD;
  client_config.QOS = QOS;
  return client_config;
}

ClientNodeMqttConfig ClientNodeMqttConfig::make()
{
  ClientNodeMqttConfig config;
  ros::NodeHandle node_private_ns("~");
  config.get_param_if_available(
      node_private_ns, "fleet_name", config.fleet_name);
  config.get_param_if_available(
      node_private_ns, "robot_name", config.robot_name);
  config.get_param_if_available(
      node_private_ns, "robot_model", config.robot_model);
  config.get_param_if_available(
      node_private_ns, "level_name", config.level_name);
  config.get_param_if_available(
      node_private_ns, "battery_state_topic", config.battery_state_topic);
  config.get_param_if_available(node_private_ns, "map_frame", config.map_frame);
  config.get_param_if_available(
      node_private_ns, "robot_frame", config.robot_frame);
  config.get_param_if_available(
      node_private_ns, "move_base_server_name", config.move_base_server_name);
  config.get_param_if_available(
      node_private_ns, "docking_trigger_server_name", config.docking_trigger_server_name);
  config.get_param_if_available(
      node_private_ns, "mqtt_domain", config.mqtt_server_address);
  config.get_param_if_available(
      node_private_ns, "mqtt_client_id", config.mqtt_client_id);
  config.get_param_if_available(
      node_private_ns, "mqtt_mode_request_topic", config.mqtt_mode_request_topic);
  config.get_param_if_available(
      node_private_ns, "mqtt_path_request_topic", config.mqtt_path_request_topic);
  config.get_param_if_available(
      node_private_ns, "mqtt_destination_request_topic",
      config.mqtt_destination_request_topic);
  config.get_param_if_available(
      node_private_ns, "QOS", config.QOS);
  config.get_param_if_available(
      node_private_ns, "LWTPAYLOAD", config.LWTPAYLOAD);
  config.get_param_if_available(
      node_private_ns, "wait_timeout", config.wait_timeout);
  config.get_param_if_available(
      node_private_ns, "update_frequency", config.update_frequency);
  config.get_param_if_available(
      node_private_ns, "publish_frequency", config.publish_frequency);
  config.get_param_if_available(
      node_private_ns, "max_dist_to_first_waypoint",
      config.max_dist_to_first_waypoint);
  return config;
}

} // namespace ros1
} // namespace free_fleet
