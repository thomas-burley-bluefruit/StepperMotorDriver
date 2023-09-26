#pragma once

#include "ITimerInterruptReceiver.h"

namespace driver
{

class IInterruptTimer10Khz
{
public:
  virtual void RegisterCallback(ITimerInterruptReceiver* callback) = 0;
  virtual ~IInterruptTimer10Khz() = default;
};

}