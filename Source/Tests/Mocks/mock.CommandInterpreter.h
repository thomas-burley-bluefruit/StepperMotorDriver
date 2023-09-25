#pragma once

#include "ICommandInterpreter.h"
#include <cstring>

namespace command
{

class MockCommandIntepreter final : public ICommandInterpreter
{
public:
  void Interpret(CommandData::RawInput& command, size_t length) override
  {
    memcpy(CommandReceived.data(), command.data(), length);
    Length = length;
    ++InterpretCallCount;
  }

  void ResetMock()
  {
    *this = MockCommandIntepreter();
  }

  CommandData::RawInput CommandReceived {};
  size_t InterpretCallCount = 0;
  size_t Length = 0;
};

}
