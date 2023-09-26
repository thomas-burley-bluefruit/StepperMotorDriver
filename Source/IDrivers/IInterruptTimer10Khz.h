#pragma once

#include "ITimerInterruptReceiver.h"

namespace driver
{

class IInterruptTimer10Khz
{
public:
  virtual void RegisterCallback(ITimerInterruptReceiver* callback) = 0;

  size_t GetInterruptRateHz() const
  {
    return 10'000;
  }

  virtual ~IInterruptTimer10Khz() = default;
};

}