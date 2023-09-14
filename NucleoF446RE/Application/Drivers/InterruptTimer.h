#pragma once

#include "IInterruptTimer.h"

namespace driver
{

class InterruptTimer final : public IInterruptTimer
{
public:
  void Init();
  void RegisterCallback(IExternalInterruptReceiver* callback) override;
};

}