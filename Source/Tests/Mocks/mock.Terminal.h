#pragma once

#include "ITerminalIn.h"
#include "TerminalIn.h"
#include <string>
#include <vector>

namespace terminal
{

class MockTerminalIn final : public ITerminalIn
{
public:
  void Start() override {}

  void Update(uint32_t time) override {}
};

}
