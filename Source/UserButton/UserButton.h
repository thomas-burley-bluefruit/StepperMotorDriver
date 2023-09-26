#pragma once

#include "IButtonPressCallback.h"
#include "IGpioDriver.h"
#include "IInterruptTimer1Khz.h"
#include "ITimerInterruptReceiver.h"
#include "IUserButton.h"

#include <cstddef>

namespace userinput
{

class UserButton : public IUserButton, public driver::ITimerInterruptReceiver
{
public:
  UserButton(driver::IGpioDriver& gpio,
    driver::IInterruptTimer1Khz& interruptTimer);
  void RegisterCallback(IButtonPressCallback* callback) override;
  void OnTimerInterrupt() override;

  static constexpr size_t MinimumOnTimeMs = 10;
  static constexpr size_t MinimumIntervalMs = 50;
  static constexpr size_t MaxCallbacks = 8;

private:
  bool Debounce();

  driver::IGpioDriver& mGpio;
  driver::IInterruptTimer1Khz& mInterruptTimer;

  static constexpr driver::GpioPin ButtonGpio = {
    .port = driver::GpioPort::PortC, .pin = 13};

  IButtonPressCallback* mCallbacks[MaxCallbacks] {nullptr};
  size_t mCallbackCount = 0;

  uint32_t mHeldTimeMs = 0;
  uint32_t mTickMs = 0;
  uint32_t mLastPressTimeMs = MinimumIntervalMs;
  bool mHeld = false;
};

}