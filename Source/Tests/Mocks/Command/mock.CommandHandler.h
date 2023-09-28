#pragma once

#include "CommandData.h"
#include "ICommandHandler.h"

namespace command
{

class MockCommandHandler final : public ICommandHandler
{
public:
  ComponentName Name() const override
  {
    return NameReturnValue;
  }

  bool Run(ICommandData& command) override
  {
    RunCalled = true;
    CommandRun = *dynamic_cast<CommandData*>(&command);
    return RunReturnValue;
  }

  bool RunCalled = false;
  ComponentName NameReturnValue = ComponentName::none;
  CommandData CommandRun {};
  bool RunReturnValue = false;
};

}