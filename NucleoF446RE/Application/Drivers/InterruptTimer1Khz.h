#pragma once

#include "IInterruptTimer1Khz.h"

namespace driver
{

class InterruptTimer1Khz final : public IInterruptTimer1Khz
{
public:
  void Init();
  void RegisterCallback(IExternalInterruptReceiver* callback) override;
};

}