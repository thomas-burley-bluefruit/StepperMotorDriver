#pragma once

#include "IInterruptTimer1Khz.h"

namespace driver
{

class MockInterruptTimer1Khz final : public IInterruptTimer1Khz
{
public:
  void RegisterCallback(ITimerInterruptReceiver* callback)
  {
    RegisterCallbackCalled = true;
    Callback = callback;
  }

  bool RegisterCallbackCalled = false;
  ITimerInterruptReceiver* Callback = nullptr;
};

}