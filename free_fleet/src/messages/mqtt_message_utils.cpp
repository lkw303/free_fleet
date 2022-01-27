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

#include "mqtt_message_utils.hpp"

namespace free_fleet {
namespace messages {

// TODO: Find out if should use const keyword for input
// in the template function
// NOTE: template class for setting modes for different classes
// since several classes have mode as their nested class
// template <typename T>
// void convert(const T& _input, RobotMode& _output)
// {
//   switch(int(_input.mode())){
//     case 0:
//       _output.mode = RobotMode::MODE_IDLE;
//       break;
//     case 1:
//       _output.mode = RobotMode::MODE_CHARGING;
//       break;
//     case 2:
//       _output.mode = RobotMode::MODE_MOVING;
//       break;
//     case 3:
//       _output.mode = RobotMode::MODE_PAUSED;
//       break;
//     case 4:
//       _output.mode = RobotMode::MODE_WAITING;
//       break;
//     case 5:
//       _output.mode = RobotMode::MODE_EMERGENCY;
//       break;
//     case 6:
//       _output.mode = RobotMode::MODE_GOING_HOME;
//       break;
//     case 7:
//       _output.mode = RobotMode::MODE_DOCKING;
//       break;
//     case 8:
//       _output.mode = RobotMode::MODE_REQUEST_ERROR;
//       break;
//   }
// }

template <typename T>
void convert(const T& _input, RobotMode& _output)
{
  _output.mode = uint32_t (_input.mode());
}

template <typename T>
void convert(const RobotMode& _input, T& _output)
{
  switch(_input.mode){
    case 0:
      _output.set_mode(free_fleet_data::RobotMode::MODE_IDLE);
      break;
    case 1:
      _output.set_mode(free_fleet_data::RobotMode::MODE_CHARGING);
      break;
    case 2:
      _output.set_mode(free_fleet_data::RobotMode::MODE_MOVING);;
      break;
    case 3:
      _output.set_mode(free_fleet_data::RobotMode::MODE_PAUSED);;
      break;
    case 4:
      _output.set_mode(free_fleet_data::RobotMode::MODE_WAITING);;
      break;
    case 5:
      _output.set_mode(free_fleet_data::RobotMode::MODE_EMERGENCY);
      break;
    case 6:
      _output.set_mode(free_fleet_data::RobotMode::MODE_GOING_HOME);
      break;
    case 7:
      _output.set_mode(free_fleet_data::RobotMode::MODE_DOCKING);
      break;
    case 8:
      _output.set_mode(free_fleet_data::RobotMode::MODE_REQUEST_ERROR);
      break;
  }
}

// NOTE: This function should be redundant
// void convert(const RobotMode& _input, free_fleet_data::RobotMode& _output)
// {
//   // Consequently, free fleet robot modes need to be ordered similarly as
//   // RMF robot modes.
//   _output.set_mode(_input.mode);
// }

// NOTE: This function should be redundant
// void convert(const free_fleet_data::RobotMode& _input, RobotMode& _output)
// {
//   // Consequently, free fleet robot modes need to be ordered similarly as
//   // RMF robot modes.
//   _output.mode = _input.mode();
// }

void convert(const Location& _input, free_fleet_data::Location& _output)
{
  _output.set_sec(_input.sec);
  _output.set_nanosec(_input.nanosec);
  _output.set_x(_input.x);
  _output.set_y(_input.y);
  _output.set_yaw(_input.yaw);
  _output.set_level_name(std::string(_input.level_name));
}

// use pointers for nested messages
void convert(const Location& _input, free_fleet_data::Location* _output)
{
  _output->set_sec(_input.sec);
  _output->set_nanosec(_input.nanosec);
  _output->set_x(_input.x);
  _output->set_y(_input.y);
  _output->set_yaw(_input.yaw);
  _output->set_level_name(std::string(_input.level_name));
}

void convert(const free_fleet_data::Location& _input, Location& _output)
{
  _output.sec = _input.sec();
  _output.nanosec = _input.nanosec();
  _output.x = _input.x();
  _output.y = _input.y();
  _output.yaw = _input.yaw();
  _output.level_name = std::string(_input.level_name());
}

void convert(const free_fleet_data::Location* _input, Location& _output)
{
  _output.sec = _input->sec();
  _output.nanosec = _input->nanosec();
  _output.x = _input->x();
  _output.y = _input->y();
  _output.yaw = _input->yaw();
  _output.level_name = std::string(_input->level_name());
}

// TODO: find a way to handle robot modes
void convert(const RobotState& _input, free_fleet_data::RobotState& _output)
{
  _output.set_name(std::string(_input.name));
  _output.set_model(std::string(_input.model));
  _output.set_task_id(std::string(_input.task_id));
  convert<free_fleet_data::RobotState>(_input.mode, _output);
  _output.set_battery_percent(_input.battery_percent);
  convert(_input.location, _output.mutable_location());
  _output.clear_path();
  size_t path_length = _input.path.size();
  for (uint32_t i = 0; i < path_length; ++i)
  {
    free_fleet_data::Location tmp;
    convert(_input.path[i], tmp);
    _output.add_path()->CopyFrom(tmp);
  }
}

void convert(const free_fleet_data::RobotState& _input, RobotState& _output)
{
  _output.name = std::string(_input.name());
  _output.model = std::string(_input.model());
  _output.task_id = std::string(_input.task_id());
  convert<free_fleet_data::RobotState>(_input, _output.mode);
  _output.battery_percent = _input.battery_percent();
  convert(_input.location(), _output.location);
  _output.path.clear();
  for (uint32_t i = 0; i < _input.path_size(); ++i)
  {
    Location tmp;
    convert(_input.path()[i], tmp);
    _output.path.push_back(tmp);
  }
}

void convert(const ModeParameter& _input, free_fleet_data::ModeParameter& _output)
{
  _output.set_name(std::string(_input.name));
  _output.set_value(std::string(_input.value));
}

void convert(const free_fleet_data::ModeParameter& _input, ModeParameter& _output)
{
  _output.name = std::string(_input.name());
  _output.value = std::string(_input.value());
}

void convert(const ModeRequest& _input, free_fleet_data::ModeRequest& _output)
{
  _output.set_fleet_name(std::string(_input.fleet_name));
  _output.set_robot_name(std::string(_input.robot_name));
  convert<free_fleet_data::ModeRequest>(_input.mode, _output);
  _output.set_task_id(std::string(_input.task_id));

  size_t mode_parameter_num = _input.parameters.size();
  for (size_t i = 0; i < mode_parameter_num; ++i){
    free_fleet_data::ModeParameter param;
    convert(_input.parameters[i],param);
    _output.add_parameters()->CopyFrom(param);
  }

}

// TODO: create pointer version of convert mode
void convert(const free_fleet_data::ModeRequest& _input, ModeRequest& _output)
{
  _output.fleet_name = std::string(_input.fleet_name());
  _output.robot_name = std::string(_input.robot_name());
  convert(_input, _output.mode);
  _output.task_id = std::string(_input.task_id());
  _output.parameters.clear();
  int param_size = _input.parameters_size();
  for (int i = 0; i < param_size ; ++i)
  {
    ModeParameter tmp;
    convert(_input.parameters(i), tmp);
    _output.parameters.push_back(tmp);
  }
}

void convert(const PathRequest& _input, free_fleet_data::PathRequest& _output)
{
  _output.set_fleet_name (std::string(_input.fleet_name));
  _output.set_robot_name(std::string(_input.robot_name));

  size_t path_length = _input.path.size();
  for (int i = 0; i < path_length; i ++){
    free_fleet_data::Location location;
    convert(_input.path[i], location);
    _output.add_path()->CopyFrom(location);
  }
  _output.set_task_id(std::string(_input.task_id));
}

void convert(const free_fleet_data::PathRequest& _input, PathRequest& _output)
{
  _output.fleet_name = std::string(_input.fleet_name());
  _output.robot_name = std::string(_input.robot_name());

  _output.path.clear();
  for (int i = 0; i < _input.path_size(); ++i)
  {
    Location tmp;
    convert(_input.path(i), tmp);
    _output.path.push_back(tmp);
  }

  _output.task_id = std::string(_input.task_id());
}

void convert(
    const DestinationRequest& _input,
    free_fleet_data::DestinationRequest& _output)
{
  _output.set_fleet_name(std::string(_input.fleet_name));
  _output.set_robot_name(std::string(_input.robot_name));
  convert(_input.destination, _output.mutable_destination());
  _output.set_task_id(std::string(_input.task_id));
}

void convert(
    const free_fleet_data::DestinationRequest& _input,
    DestinationRequest& _output)
{
  _output.fleet_name = std::string(_input.fleet_name());
  _output.robot_name = std::string(_input.robot_name());
  convert(_input.destination(), _output.destination);
  _output.task_id = std::string(_input.task_id());
}

} // namespace messages
} // namespace free_fleet
