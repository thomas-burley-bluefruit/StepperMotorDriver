#pragma once

#include "IButtonPressCallback.h"

namespace userinput
{

class MockButtonPressCallback final : public IButtonPressCallback
{
public:
  void OnButtonPress()
  {
    OnButtonPressCalled = true;
  }

  bool OnButtonPressCalled = false;
};

}