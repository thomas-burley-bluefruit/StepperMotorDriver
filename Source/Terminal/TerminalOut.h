#pragma once

#include "ITerminalOut.h"
#include "IUartDriver.h"
#include <array>

namespace terminal
{

class TerminalOut final : public ITerminalOut
{
public:
  static constexpr size_t MaxTextOutLength = 256;

  TerminalOut(driver::IUartDriver& uart);
  void TextOut(const char* text, ...) override;

private:
  driver::IUartDriver& mUart;
  std::array<char, MaxTextOutLength> mTextOutBuffer {};
};

}