#pragma once

#include <cstdint>

namespace driver
{

enum class GpioPort : uint8_t
{
  PortA,
  PortB,
  PortC,
  PortD
};

struct GpioPin
{
  GpioPort port;
  uint8_t pin;

  bool operator==(const GpioPin& rhs) const
  {
    return this->port == rhs.port && this->pin == rhs.pin;
  }
};

}
