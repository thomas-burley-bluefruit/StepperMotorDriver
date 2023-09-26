#pragma once

#include "IInterruptTimer10Khz.h"

namespace driver
{

class InterruptTimer10Khz final : public IInterruptTimer10Khz
{
public:
  void Init();
  void RegisterCallback(ITimerInterruptReceiver* callback) override;
};

}

extern "C" void InterruptTimer10Khz_PeriodElapsed();
