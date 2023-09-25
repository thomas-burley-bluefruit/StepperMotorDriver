#include "TerminalIn.h"

#include <cstring>
#include <string>

using namespace ::terminal;
using namespace ::command;
using namespace ::utility;

TerminalIn::TerminalIn(driver::IUartDriver& uart,
  ICommandInterpreter& commandInterpreter) :
  mInputQueue(mInputQueueBuffer, InputQueueSize),
  mUart(uart),
  mCommandInterpreter(commandInterpreter)
{
}

void TerminalIn::Start()
{
  mUart.Init();
  mUart.SetCallback(this);
}

void TerminalIn::Update([[maybe_unused]] uint32_t time)
{
  const size_t lineLength = NextLineLength();
  if (lineLength == 0)
  {
    // Unblock queue if required
    if (mInputQueue.FreeCount() == 0)
      mInputQueue.Clear();

    return;
  }

  if (lineLength > CommandData::MaxRawInputLength)
  {
    mInputQueue.DiscardTail(lineLength);
    return;
  }

  LineReceived(lineLength);
}

void TerminalIn::OnReceiveCharacter(const char c)
{
  if (c == '\b')
  {
    mInputQueue.DiscardHead(1);
    return;
  }

  if (IsPrintable(c))
    mInputQueue.Write(static_cast<uint8_t>(c));
}

void TerminalIn::LineReceived(const size_t lineLength)
{
  size_t readSize = mInputQueue.ReadBlock(
    reinterpret_cast<uint8_t*>(mParseBuffer.data()), lineLength);
  mCommandInterpreter.Interpret(mParseBuffer, readSize);
}

bool TerminalIn::IsPrintable(char c) const
{
  return ((c >= ' ' && c <= '~') || c == '\n');
}

size_t TerminalIn::NextLineLength() const
{
  const size_t lineEndingPos = mInputQueue.Find(LineEnding, 0);
  return lineEndingPos == DataQueue::NotFound ? 0 : lineEndingPos + 1;
}
