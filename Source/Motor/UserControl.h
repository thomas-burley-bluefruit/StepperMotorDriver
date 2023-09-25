#pragma once

#include "IButtonPressCallback.h"
#include "ICommandHandler.h"
#include "ICommandRegistry.h"
#include "IStepper.h"
#include "IUserButton.h"

namespace motor
{

class UserControl final :
  public userinput::IButtonPressCallback,
  public command::ICommandHandler
{
public:
  UserControl(IStepper& stepper, userinput::IUserButton& button,
    command::ICommandRegistry& commandRegistry);

  // IButtonPressCallback
  void OnButtonPress() override;

  // ICommandHandler
  command::ComponentName Name() const;
  bool Run(command::ICommandData& command);

private:
  IStepper& mStepper;
};

}