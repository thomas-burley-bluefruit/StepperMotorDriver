#pragma once

#include "CommandData.h"

#include <string>

namespace command
{

constexpr CommandData::RawInput CreateRawCommand(std::string uartString)
{
  CommandData::RawInput command {};
  std::copy(uartString.begin(), uartString.end(), command.begin());
  return command;
}
}