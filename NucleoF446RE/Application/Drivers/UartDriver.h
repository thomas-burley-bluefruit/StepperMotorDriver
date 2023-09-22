#pragma once

#include "IUartDriver.h"

namespace driver
{

class UartDriver final : public IUartDriver
{
public:
  void Init() override;
  void Transmit(const uint8_t* txData, size_t size) const override;
  void SetCallback(ITerminalReceiveCharacterCallback* callback) const override;
};

}
