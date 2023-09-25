#pragma once

#include "ICommandHandler.h"

namespace terminal
{

class ITerminalIn
{
public:
  virtual void Start() = 0;
  virtual void Update(uint32_t time) = 0;
  virtual ~ITerminalIn() = default;
};

}
