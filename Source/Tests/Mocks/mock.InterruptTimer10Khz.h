#pragma once

#include "IInterruptTimer10Khz.h"

namespace driver
{

class MockInterruptTimer10Khz final : public IInterruptTimer10Khz
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