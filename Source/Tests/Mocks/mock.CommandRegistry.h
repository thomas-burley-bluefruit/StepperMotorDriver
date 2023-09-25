#pragma once

#include "CommandData.h"
#include "ICommandRegistry.h"

namespace command
{

class MockCommandRegistry final : public ICommandRegistry
{
public:
  void RegisterHandler(ICommandHandler* handler)
  {
    RegisterHandlerCalled = true;
    RegisteredHandler = handler;
  }

  bool Run(ICommandData& command)
  {
    RunCalled = true;
    CommandRun = *dynamic_cast<CommandData*>(&command);
    return RunReturnValue;
  }

  bool RegisterHandlerCalled = false;
  ICommandHandler* RegisteredHandler = nullptr;

  bool RunCalled = false;
  CommandData CommandRun {};
  bool RunReturnValue = true;
};

}