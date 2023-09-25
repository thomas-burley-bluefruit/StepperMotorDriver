#pragma once

#include "CommandData.h"
#include "DataQueue.h"
#include "ICommandHandler.h"
#include "ICommandInterpreter.h"
#include "ITerminalIn.h"
#include "ITerminalReceiveCharacterCallback.h"
#include "IUartDriver.h"

namespace terminal
{

class TerminalIn final :
  public ITerminalIn,
  public ITerminalReceiveCharacterCallback
{
public:
  TerminalIn(driver::IUartDriver& uart,
    command::ICommandInterpreter& commandInterpreter);

  // ITerminalIn
  void Start() override;
  void Update(uint32_t time = 0) override;

  // ITerminalReceiveCharacterCallback
  void OnReceiveCharacter(const char byte) override;

  static constexpr size_t MaxCommandHandlers = 32;
  static constexpr size_t InputQueueSize = 1024;

private:
  void LineReceived(const size_t lineEndPos);
  bool IsPrintable(char c) const;
  size_t NextLineLength() const;

private:
  uint8_t mInputQueueBuffer[InputQueueSize] {};
  utility::DataQueue mInputQueue;
  command::CommandData::RawInput mParseBuffer {};
  driver::IUartDriver& mUart;
  command::ICommandInterpreter& mCommandInterpreter;
  static constexpr char LineEnding = '\n';
};

}
