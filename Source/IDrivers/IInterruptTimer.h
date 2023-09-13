#pragma once

#include "IExternalInterruptReceiver.h"

namespace driver
{

  class IInterruptTimer
  {
  public:
    virtual void RegisterCallback(IExternalInterruptReceiver *callback) = 0;
    virtual ~IInterruptTimer() = default;
  };

}