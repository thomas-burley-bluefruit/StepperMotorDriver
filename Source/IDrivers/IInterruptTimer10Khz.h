#pragma once

#include "IExternalInterruptReceiver.h"

namespace driver
{

class IInterruptTimer10Khz
{
public:
  virtual void RegisterCallback(IExternalInterruptReceiver* callback) = 0;
  virtual ~IInterruptTimer10Khz() = default;
};

}