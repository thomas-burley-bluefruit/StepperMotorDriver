#pragma once

#include "IInterruptTimer.h"

namespace driver
{

class MockInterruptTimer final : public IInterruptTimer1Khz
{
public:
  void RegisterCallback(IExternalInterruptReceiver* callback)
  {
    RegisterCallbackCalled = true;
    Callback = callback;
  }

  bool RegisterCallbackCalled = false;
  IExternalInterruptReceiver* Callback = nullptr;
};

}