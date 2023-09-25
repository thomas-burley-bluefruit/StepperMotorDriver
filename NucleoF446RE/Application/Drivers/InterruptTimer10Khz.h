#pragma once

#include "IInterruptTimer10Khz.h"

namespace driver
{

class InterruptTimer10Khz final : public IInterruptTimer10Khz
{
public:
  void Init();
  void RegisterCallback(IExternalInterruptReceiver* callback) override;
};

}