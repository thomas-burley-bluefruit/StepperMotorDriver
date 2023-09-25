#include "UserControl.h"

using namespace motor;

UserControl::UserControl(IStepper& stepper, userinput::IUserButton& button,
  command::ICommandRegistry& commandRegistry) :
  mStepper(stepper)
{
  button.RegisterCallback(this);
  commandRegistry.RegisterHandler(this);
}

void UserControl::OnButtonPress()
{
  mStepper.Step(1);
}

command::ComponentName UserControl::Name() const
{
  return command::ComponentName::stepper;
}

bool UserControl::Run(command::ICommandData& command)
{
  mStepper.Step(1);
  return true;
}