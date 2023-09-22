#pragma once

#include "IButtonPressCallback.h"

namespace userinput
{

class IUserButton
{
public:
  ~IUserButton() = default;
  virtual void RegisterCallback(IButtonPressCallback* callback) = 0;
};

}