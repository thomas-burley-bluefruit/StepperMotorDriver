#pragma once

#include "ITerminalReceiveCharacterCallback.h"

#include <cstddef>
#include <cstdint>

namespace driver
{

class IUartDriver
{
public:
  virtual void Init() = 0;
  virtual void Transmit(const uint8_t* txData, size_t size) const = 0;
  virtual void SetCallback(
    ITerminalReceiveCharacterCallback* callback) const = 0;
  virtual ~IUartDriver() = default;
};

}
