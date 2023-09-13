#pragma once

#include <cstdint>

namespace userinput
{

  class IButtonPressCallback
  {
  public:
    ~IButtonPressCallback() = default;
    virtual void OnButtonPress() = 0;
  };

}