#pragma once

#include "CommandData.h"

namespace command
{

class ICommandInterpreter
{
public:
  virtual void Interpret(CommandData::RawInput& rawInput, size_t length) = 0;
  virtual ~ICommandInterpreter() = default;
};

}
