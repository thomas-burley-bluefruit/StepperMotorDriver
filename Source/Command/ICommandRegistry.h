#pragma once

#include "ICommandHandler.h"

namespace command
{

class ICommandRegistry
{
public:
  virtual void RegisterHandler(ICommandHandler* handler) = 0;
  virtual bool Run(ICommandData& command) = 0;
  virtual ~ICommandRegistry() = default;
};

}