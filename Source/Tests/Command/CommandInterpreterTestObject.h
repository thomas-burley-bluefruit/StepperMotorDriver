#pragma once

#include "CommandInterpreter.h"

namespace command
{

class CommandInterpreterTestObject final : public CommandInterpreter
{
public:
  CommandInterpreterTestObject(terminal::ITerminalOut& terminal,
    ICommandRegistry& commandRegistry) :
    CommandInterpreter(terminal, commandRegistry)
  {
  }

  CommandData& GetCommandData()
  {
    return CommandInterpreter::GetCommandData();
  }
};

}