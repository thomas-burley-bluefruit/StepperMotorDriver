#include "TerminalOut.h"

#include <cstdarg>
#include <cstring>
#include <string>

using namespace ::terminal;

TerminalOut::TerminalOut(driver::IUartDriver& uart) :
  mUart(uart)
{
  // Send empty line to sync the receiver clock to the sender
  char const* syncBytes = "\n";
  TextOut(syncBytes);
}

void TerminalOut::TextOut(const char* formatText, ...)
{
  va_list args;
  va_start(args, formatText);
  vsnprintf(mTextOutBuffer.data(), MaxTextOutLength, formatText, args);
  va_end(args);

  const auto size = strnlen(mTextOutBuffer.data(), MaxTextOutLength);
  mUart.Transmit(reinterpret_cast<const uint8_t*>(mTextOutBuffer.data()), size);
}
