#include "GpioDriver.h"
#include <cassert>

using namespace ::driver;

bool GpioDriver::Get(GpioPin pin) const
{
  const auto halPort = GetHalPort(pin);
  const auto halPin = GetHalPin(pin);

  auto pinState = HAL_GPIO_ReadPin(halPort, halPin);

  bool pinHigh = (pinState == GPIO_PIN_SET);
  return pinHigh;
}

void GpioDriver::Set(GpioPin pin, bool state) const
{
  const auto halPort = GetHalPort(pin);
  const auto halPin = GetHalPin(pin);
  const GPIO_PinState halState = state ? GPIO_PIN_SET : GPIO_PIN_RESET;

  HAL_GPIO_WritePin(halPort, halPin, halState);
}

uint16_t GpioDriver::GetHalPin(GpioPin pin) const
{
  const bool gpioPinValid = (0 <= pin.pin && pin.pin <= 15);
  assert(gpioPinValid);

  return 1 << pin.pin;
}

GPIO_TypeDef* GpioDriver::GetHalPort(GpioPin pin) const
{
  switch (pin.port)
  {
  case GpioPort::PortA:
    return GPIOA;
  case GpioPort::PortB:
    return GPIOB;
  case GpioPort::PortC:
    return GPIOC;
  case GpioPort::PortD:
    return GPIOD;

  default:
    const bool UnconfiguredPort = false;
    assert(UnconfiguredPort);
  }
}
