#pragma once

#include "IExternalInterruptReceiver.h"

namespace driver
{

class IInterruptTimer1Khz
{
public:
  virtual void RegisterCallback(IExternalInterruptReceiver* callback) = 0;
  virtual ~IInterruptTimer1Khz() = default;
};

}