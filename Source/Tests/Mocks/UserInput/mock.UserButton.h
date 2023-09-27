#pragma once

#include "IUserButton.h"

namespace userinput
{

class MockUserButton final : public IUserButton
{
public:
  void RegisterCallback(IButtonPressCallback* callback) override
  {
    RegisterCallbackCalled = true;
    Callback = callback;
  }

  bool RegisterCallbackCalled = false;
  IButtonPressCallback* Callback = nullptr;
};

}