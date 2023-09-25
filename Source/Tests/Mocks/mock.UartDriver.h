#pragma once

#include "IUartDriver.h"
#include "TerminalIn.h"
#include "TerminalOut.h"
#include <vector>

namespace driver
{

class MockUartDriver final : public IUartDriver
{
public:
  void Init() override
  {
    InitCalled = true;
  }

  void Transmit(const uint8_t* txData, size_t size) const override
  {
    TransmitCalled = true;
    TransmitSize = size;

    if (size > terminal::TerminalOut::MaxTextOutLength)
    {
      return;
    }

    for (size_t i = 0; i < size; ++i)
    {
      TxData[i] = txData[i];
    }
  }

  void SetCallback(ITerminalReceiveCharacterCallback* callback) const override
  {
    SetCallbackCalled = true;
    Callback = callback;
  }

  void ResetMock()
  {
    *this = MockUartDriver();
  }

public:
  mutable bool SetCallbackCalled = false;
  mutable ITerminalReceiveCharacterCallback* Callback = nullptr;

  bool InitCalled = false;

  mutable bool TransmitCalled = false;
  mutable size_t TransmitSize = 0;
  mutable std::array<uint8_t, terminal::TerminalOut::MaxTextOutLength>
    TxData {};
};

}
