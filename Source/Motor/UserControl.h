#pragma once

#include "IButtonPressCallback.h"
#include "IStepper.h"
#include "IUserButton.h"

namespace motor
{

class UserControl final : public userinput::IButtonPressCallback
{
public:
  UserControl(IStepper& stepper, userinput::IUserButton& button);

  // IButtonPressCallback
  void OnButtonPress() override;

private:
  IStepper& mStepper;
};

}