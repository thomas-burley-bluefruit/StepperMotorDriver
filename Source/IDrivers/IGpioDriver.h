#pragma once

#include "GpioPin.h"
#include <cstdbool>

namespace driver
{

  class IGpioDriver
  {
  public:
    virtual bool Get(GpioPin pin) const = 0;
    virtual void Set(GpioPin pin, bool state) const = 0;
    virtual ~IGpioDriver() = default;
  };

}
