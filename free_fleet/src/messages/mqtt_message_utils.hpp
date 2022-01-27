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

#ifndef FREE_FLEET__SRC__MESSAGES__MESSAGE_UTILS_HPP
#define FREE_FLEET__SRC__MESSAGES__MESSAGE_UTILS_HPP

#include <free_fleet/messages/Location.hpp>
#include <free_fleet/messages/RobotMode.hpp>
#include <free_fleet/messages/RobotState.hpp>
#include <free_fleet/messages/ModeParameter.hpp>
#include <free_fleet/messages/ModeRequest.hpp>
#include <free_fleet/messages/PathRequest.hpp>
#include <free_fleet/messages/DestinationRequest.hpp>

#include "FleetMessages.pb.h"

namespace free_fleet {
namespace messages {

template<typename T>
void convert(T& _input, RobotMode& _output);

template<typename T>
void convert(const RobotMode& _input, T& _output);

// void convert(const RobotMode& _input, free_fleet::RobotMode& _output);

// void convert(const free_fleet_data::RobotMode& _input, RobotMode& _output);

void convert(const Location& _input, free_fleet_data::Location& _output);

void convert(const Location& _input, free_fleet_data::Location* _output);

void convert(const free_fleet_data::Location& _input, Location& _output);

void convert(const free_fleet_data::Location* _input, Location& _output);

void convert(const RobotState& _input, free_fleet_data::RobotState& _output);

void convert(const free_fleet_data::RobotState& _input, RobotState& _output);

void convert(const ModeParameter& _input, free_fleet_data::ModeParameter& _output);

void convert(const free_fleet_data::ModeParameter& _input, ModeParameter& _output);

void convert(const ModeRequest& _input, free_fleet_data::ModeRequest& _output);

void convert(const free_fleet_data::ModeRequest& _input, ModeRequest& _output);

void convert(const PathRequest& _input, free_fleet_data::PathRequest& _output);

void convert(const free_fleet_data::PathRequest& _input, PathRequest& _output);

void convert(
    const DestinationRequest& _input,
    free_fleet_data::DestinationRequest& _output);

void convert(
    const free_fleet_data::DestinationRequest& _input,
    DestinationRequest& _output);

} // namespace
} // namespace free_fleet

#endif // FREE_FLEET__SRC__MESSAGES__MESSAGE_UTILS_HPP
