#pragma once

#include "gpio.h"
#include "IGpioDriver.h"

namespace driver
{

  class GpioDriver final : public IGpioDriver
  {
  public:
    bool Get(GpioPin pin) const override;
    void Set(GpioPin pin, bool state) const override;

  private:
    uint16_t GetHalPin(GpioPin pin) const;
    GPIO_TypeDef *GetHalPort(GpioPin pin) const;
  };

}
