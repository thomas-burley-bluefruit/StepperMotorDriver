#pragma once

#include "ITerminalOut.h"
#include "TerminalOut.h"

#include <cstdarg>
#include <string>
#include <vector>

namespace terminal
{

class MockTerminalOut final : public ITerminalOut
{
public:
  void TextOut(const char* text, ...) override
  {
    char buffer[TerminalOut::MaxTextOutLength];
    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    TextOutStrings += buffer;
    TextOutCallCount++;
  }

  size_t TextOutCallCount = 0;
  std::string TextOutStrings;
};

}