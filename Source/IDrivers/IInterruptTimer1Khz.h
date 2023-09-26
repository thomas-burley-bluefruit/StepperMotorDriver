#pragma once

#include "ITimerInterruptReceiver.h"

namespace driver
{

class IInterruptTimer1Khz
{
public:
  virtual void RegisterCallback(ITimerInterruptReceiver* callback) = 0;
  virtual ~IInterruptTimer1Khz() = default;
};

}