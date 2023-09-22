#pragma once

namespace terminal
{

class ITerminalOut
{
public:
  virtual void TextOut(const char* text, ...) = 0;
  virtual ~ITerminalOut() = default;
};

}