#pragma once

#include "ComponentName.h"
#include "ICommandData.h"

namespace command
{

class ICommandHandler
{
public:
  virtual ComponentName Name() const = 0;
  virtual bool Run(ICommandData& command) = 0;
  virtual ~ICommandHandler() = default;
};

}
